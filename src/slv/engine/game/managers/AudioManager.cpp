#include <slv/engine/game/managers/AudioManager.hpp>
#include <slv/engine/game/managers/WindowManager.hpp>
#include <slv/engine/log.hpp>
#include <fmt/format.h>
#include <miniaudio/miniaudio.h>
#include <vector>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <cstdint>
#include <cmath>

namespace slv {

struct audio_user_data final {
    audio_user_data() :
        device(),
        is_initialized(false),
        window_ptr(nullptr) {
        handles.reserve(64);
        pending_handles.reserve(64);
    }

    ma_device device;
    std::vector<audio_hnd> handles;
    std::vector<audio_hnd> pending_handles;
    std::mutex pending_mutex;
    std::atomic<bool> is_initialized;
    WindowManager* window_ptr;
};

struct AudioManager::impl final {
    bool init(WindowManager* window) {
        if (user_data.is_initialized.load()) {
            return true;
        }

        if (!window) {
            log::error("Window pointer is nullptr");
            return false;
        }

        user_data.window_ptr = window;

        ma_device_config config = ma_device_config_init(ma_device_type_playback);
        config.playback.format = ma_format_f32;
        config.playback.channels = SLV_AUDIO_CHANNELS;
        config.sampleRate = SLV_AUDIO_SAMPLE_RATE;
        config.dataCallback = update;
        config.pUserData = &user_data;

        if (
            ma_result result = ma_device_init(nullptr, &config, &user_data.device);
            result != MA_SUCCESS
        ) {
            log::error(
                fmt::format(
                    "ma_device_init -> ma_result: {}",
                    static_cast<int>(result)
                )
            );

            return false;
        }

        if (
            ma_result result = ma_device_start(&user_data.device);
            result != MA_SUCCESS
        ) {
            log::error(
                fmt::format(
                    "ma_device_start -> ma_result: {}",
                    static_cast<int>(result)
                )
            );

            ma_device_uninit(&user_data.device);

            return false;
        }

        user_data.is_initialized.store(true);

        log::info("Audio initialized");

        return true;
    }

    void uninit() {
        if (!user_data.is_initialized.load()) {
            return;
        }

        log::info("Audio exiting...");

        ma_device_stop(&user_data.device);
        
        // clear sounds
        {
            std::lock_guard<std::mutex> lock(user_data.pending_mutex);
            user_data.pending_handles.clear();
            user_data.handles.clear();
        }

        ma_device_uninit(&user_data.device);

        user_data.is_initialized.store(false);
    }

    void push_pcm_data(sptr<pcm_data> pcm) {
        if (
            !user_data.is_initialized.load() ||
            // validate pcm
            !pcm || pcm->empty() ||
            (pcm->size() % SLV_AUDIO_CHANNELS) != 0
        ) {
            return;
        }

        std::lock_guard<std::mutex> lock(user_data.pending_mutex);
        user_data.pending_handles.emplace_back(pcm, 0);
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

        // move pending to local buffer
        std::vector<audio_hnd> local_pending;
        {
            std::lock_guard<std::mutex> lock(udata->pending_mutex);
            if (!udata->pending_handles.empty()) {
                local_pending.swap(udata->pending_handles);
            }
        }

        // append outside lock
        if (!local_pending.empty()) {
            udata->handles.insert(
                udata->handles.end(),
                std::make_move_iterator(local_pending.begin()),
                std::make_move_iterator(local_pending.end())
            );
        }

        // mix handles/sounds
        size_t active_count = 0;
        for (auto it = udata->handles.begin(); it != udata->handles.end();) {
            auto& handle = *it;

            if (!handle.pcm || handle.pcm->empty()) {
                it = udata->handles.erase(it);
                continue;
            }

            const auto& pcm = *handle.pcm;

            // ensure valid layout
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
                    out[dst] += pcm[src];
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

        // normalize (prevent heavy clipping)
        if (active_count > 1) {
            const float inv = 1.f / static_cast<float>(active_count);
            for (size_t i = 0; i < sample_count; ++i) {
                out[i] *= inv;
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
AudioManager::AudioManager() :
    impl_(unique<impl>())
{}

// private
AudioManager::~AudioManager() = default;

void AudioManager::push_pcm_data(sptr<pcm_data> pcm) const {
    impl_->push_pcm_data(pcm);
}

// private
bool AudioManager::init(WindowManager* window) {
    return impl_->init(window);
}

// private
void AudioManager::uninit() const {
    impl_->uninit();
}

}