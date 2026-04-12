#include <slv/game/managers/AudioManager.hpp>
#include <slv/core/console/log.hpp>
#include <slv/core/audio_config.hpp>
#include <miniaudio/miniaudio.h>
#include <vector>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <cstdint>

namespace
{
    struct audio_sys
    {
        ma_device device{};
        std::vector<slv::audio_hnd> handles;
        std::vector<slv::audio_hnd> pending_handles;
        std::mutex pending_mutex;
        std::atomic<bool> is_initialized{ false };
    };

    audio_sys audio_system;

    void data_callback(ma_device* device, void* output, const void*, ma_uint32 frame_count)
    {
        auto* system = static_cast<audio_sys*>(device->pUserData);
        float* out = static_cast<float*>(output);

        const ma_uint32 channels = device->playback.channels;
        const size_t sample_count = static_cast<size_t>(frame_count) * channels;

        // clear output buffer
        std::fill(out, out + sample_count, 0.f);

        // move pending handles into active handles
        {
            std::lock_guard<std::mutex> lock(system->pending_mutex);

            if (!system->pending_handles.empty())
            {
                system->handles.insert(system->handles.end(),
                                       std::make_move_iterator(system->pending_handles.begin()),
                                       std::make_move_iterator(system->pending_handles.end()));
                system->pending_handles.clear();
            }
        }

        // mix active sounds
        for (auto it = system->handles.begin(); it != system->handles.end();)
        {
            auto& handle = *it;

            if (!handle.pcm || handle.pcm->empty())
            {
                it = system->handles.erase(it);
                continue;
            }

            const auto& pcm = *handle.pcm;
            const uint64_t total_frames = pcm.size() / channels;

            if (handle.cursor >= total_frames)
            {
                it = system->handles.erase(it);
                continue;
            }

            const uint64_t frames_remaining = total_frames - handle.cursor;
            const uint64_t frames_to_mix = std::min<uint64_t>(frames_remaining, frame_count);

            for (uint64_t frame = 0; frame < frames_to_mix; ++frame)
            {
                for (uint32_t ch = 0; ch < channels; ++ch)
                {
                    size_t src_index = (handle.cursor + frame) * channels + ch;
                    size_t dst_index = frame * channels + ch;
                    out[dst_index] += pcm[src_index];
                }
            }

            handle.cursor += frames_to_mix;

            if (handle.cursor >= total_frames)
            {
                it = system->handles.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (size_t i = 0; i < sample_count; ++i)
        {
            out[i] = std::clamp(out[i], -1.f, 1.f);
        }
    }

    void unload_sounds()
    {
        std::lock_guard<std::mutex> lock(audio_system.pending_mutex);
        audio_system.pending_handles.clear();
        audio_system.handles.clear();
    }
}

namespace slv
{
    bool AudioManager::init()
    {
        if (audio_system.is_initialized.load())
        {
            return true;
        }

        ma_device_config config = ma_device_config_init(ma_device_type_playback);

        config.playback.format = SLV_AUDIO_SAMPLEFORMAT;
        config.playback.channels = slv::AUDIO_CHANNELS;
        config.sampleRate = slv::AUDIO_SAMPLE_RATE;
        config.dataCallback = data_callback;
        config.pUserData = &audio_system;

        ma_result result = ma_device_init(nullptr, &config, &audio_system.device);

        if (result != MA_SUCCESS)
        {
            slv::log::error(M_NAME, "ma_device_init -> ma_result: {}", static_cast<int>(result));
            return false;
        }

        result = ma_device_start(&audio_system.device);

        if (result != MA_SUCCESS)
        {
            slv::log::error(M_NAME, "ma_device_start -> ma_result: {}", static_cast<int>(result));
            ma_device_uninit(&audio_system.device);
            return false;
        }

        audio_system.is_initialized.store(true);
        slv::log::info(M_NAME, "Audio initialized");

        return true;
    }

    void AudioManager::uninit() const
    {
        if (!audio_system.is_initialized.load())
        {
            return;
        }

        slv::log::info(M_NAME, "Audio exiting...");

        unload_sounds();
        ma_device_uninit(&audio_system.device);

        audio_system.is_initialized.store(false);
    }

    bool AudioManager::push_pcm_data(const slv::sptr<slv::pcm_data>& pcm) const
    {
        if (!audio_system.is_initialized.load() || !pcm)
        {
            return false;
        }

        std::lock_guard<std::mutex> lock(audio_system.pending_mutex);
        audio_system.pending_handles.push_back({ pcm, 0 });

        return true;
    }
}