#include <crx/engine/main/AudioSys.hh>
#include <crx/engine/main/Window.hh>
#include <crx/engine/debug/log.hh>
#include <fmt/format.h>
#include <miniaudio.h>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <cstdint>
#include <cmath>

namespace crx {

struct audio_hnd final {
    explicit audio_hnd(sptr<std::vector<float>>& pcm_val, float volume_val) :
        pcm(pcm_val),
        volume(volume_val),
        pitch(1.f),
        cursor(0)
    {}

    sptr<std::vector<float>> pcm;
    float volume;
    float pitch;
    uint32_t cursor;
};

struct audio_user_data final {
    audio_user_data() :
        device(),
        is_initialized(false),
        global_volume(1.f),
        window_ptr(nullptr)
    {}

    ma_device device;
    std::vector<audio_hnd> handles;
    std::vector<audio_hnd> pending_handles;
    std::mutex pending_mutex;
    std::atomic<bool> is_initialized;
    float global_volume;
    Window* window_ptr;
};

struct AudioSys::impl final {
    bool init(Window* window) {
        if (user_data.is_initialized.load()) {
            return true;
        }

        if (!window) {
            log::error("Failed to initialize | assed window pointer is nullptr");
            return false;
        }

        user_data.window_ptr = window;

        ma_device_config config = ma_device_config_init(ma_device_type_playback);
        config.playback.format = ma_format_f32;
        config.playback.channels = CRX_AUDIO_CHANNELS;
        config.sampleRate = CRX_AUDIO_SAMPLE_RATE;
        config.dataCallback = update;
        config.pUserData = &user_data;

        if (
            ma_result result = ma_device_init(nullptr, &config, &user_data.device);
            result != MA_SUCCESS
        ) {
            log::error(fmt::format("Failed to initialize | ma_device_init -> ma_result: {}", (int)result));
            return false;
        }

        if (
            ma_result result = ma_device_start(&user_data.device);
            result != MA_SUCCESS
        ) {
            log::error(fmt::format("Failed to initialize | ma_device_start -> ma_result: {}", (int)result));
            ma_device_uninit(&user_data.device);
            return false;
        }

        user_data.is_initialized.store(true);

        log::info("Initialized");

        return true;
    }

    void shutdown() {
        if (!user_data.is_initialized.load()) {
            return;
        }

        log::info("Exiting...");

        if (
            auto res = ma_device_stop(&user_data.device);
            res != MA_SUCCESS
        ) {
            log::warning(fmt::format("Failed to stop device | ma_device_stop -> {}", res));
        }
        
        // clear sounds
        {
            std::lock_guard<std::mutex> lock(user_data.pending_mutex);
            user_data.pending_handles.clear();
            user_data.handles.clear();
        }

        ma_device_uninit(&user_data.device);

        user_data.is_initialized.store(false);
    }

    void push_audio(sptr<std::vector<float>> pcm, float volume) {
        if (!user_data.is_initialized.load()) {
            return;
        }

        if (!pcm) {
            log::error("Attempted to push audio with nullptr pcm");
            return;
        }

        if (pcm->empty()) {
            log::error("Attempted to push audio with empty pcm");
            return;
        }

        if ((pcm->size() % CRX_AUDIO_CHANNELS) != 0) {
            log::error("Attempted to push audio with invalid pcm");
            return;
        }

        std::lock_guard<std::mutex> lock(user_data.pending_mutex);
        user_data.pending_handles.emplace_back(audio_hnd(pcm, volume));
    }

    static void update(ma_device* device, void* output, void const*, ma_uint32 frame_count) {
        auto* udata = static_cast<audio_user_data*>(device->pUserData);
        auto* out = static_cast<float*>(output);
        const ma_uint32 channels = device->playback.channels;
        const size_t sample_count = static_cast<size_t>(frame_count) * channels;

        // clear output
        if (
            !udata ||
            !udata->window_ptr ||
            udata->window_ptr->is_minimized() ||
            (udata->handles.empty() && udata->pending_handles.empty())
        ) {
            std::fill(out, out + sample_count, 0.f);
            return;
        }

        // move pending
        std::vector<audio_hnd> local_pending;
        {
            std::lock_guard<std::mutex> lock(udata->pending_mutex);
            if (!udata->pending_handles.empty()) {
                local_pending.swap(udata->pending_handles);
            }
        }

        if (!local_pending.empty()) {
            udata->handles.insert(
                udata->handles.end(),
                std::make_move_iterator(local_pending.begin()),
                std::make_move_iterator(local_pending.end())
            );
        }

        // mix handles
        size_t active_count = 0;
        for (auto it = udata->handles.begin(); it != udata->handles.end();) {
            auto& handle = *it;

            if (!handle.pcm || handle.pcm->empty()) {
                it = udata->handles.erase(it);
                continue;
            }

            const auto& pcm = *handle.pcm;

            if (pcm.size() % channels != 0) {
                it = udata->handles.erase(it);
                continue;
            }

            const uint64_t total_frames = pcm.size() / channels;

            if (handle.cursor >= total_frames) {
                it = udata->handles.erase(it);
                continue;
            }

            const uint64_t frames_remaining = total_frames - handle.cursor;
            const uint64_t frames_to_mix = std::min<uint64_t>(frames_remaining, frame_count);

            for (uint64_t frame = 0; frame < frames_to_mix; ++frame) {
                for (uint32_t ch = 0; ch < channels; ++ch) {
                    const size_t src = (handle.cursor + frame) * channels + ch;
                    const size_t dst = frame * channels + ch;
                    const float gain = udata->global_volume * handle.volume;
                    out[dst] += pcm[src] * gain;
                }
            }

            handle.cursor += frames_to_mix;
            ++active_count;

            if (handle.cursor >= total_frames) {
                it = udata->handles.erase(it);
            } else {
                ++it;
            }
        }

        // soft clip
        for (size_t i = 0; i < sample_count; ++i) {
            out[i] = std::tanh(out[i]);
        }
    }

    audio_user_data user_data;
};

// private
AudioSys::AudioSys() :
    impl_(unique<impl>())
{}

// private
AudioSys::~AudioSys() = default;

void AudioSys::push_audio(sptr<std::vector<float>> pcm, float volume) const {
    impl_->push_audio(pcm, volume);
}

// private
bool AudioSys::init(Window* Window) {
    return impl_->init(Window);
}

// private
void AudioSys::shutdown() const {
    impl_->shutdown();
}

}