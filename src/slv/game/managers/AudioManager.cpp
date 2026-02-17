#include <slv/game/managers/AudioManager.hpp>
#include <slv/core/console_log.hpp>
#include <miniaudio/miniaudio.h>
#include <vector>
#include <mutex>
#include <atomic>
#include <cstdint>

namespace
{
    struct audio_sys
    {
        ma_device device{};
        std::vector<ma_decoder> decoders;
        std::vector<ma_decoder> pending_decoders;
        std::vector<float> temp_buffer;
        std::mutex pending_mutex;
        std::atomic<bool> is_initialized{ false };
    };

    std::vector<ma_decoder> decoders;
    audio_sys audio_system;

    void data_callback(ma_device* device, void* output, const void*, ma_uint32 frame_count)
    {
        audio_sys* system = static_cast<audio_sys*>(device->pUserData);
        float* out = static_cast<float*>(output);
        size_t sample_count = static_cast<size_t>(frame_count) * device->playback.channels;
        std::fill(out, out + sample_count, 0.0F);

        {
            std::lock_guard<std::mutex> lock(system->pending_mutex);

            if (!system->pending_decoders.empty())
            {
                system->decoders.insert(system->decoders.end(),
                                        std::make_move_iterator(system->pending_decoders.begin()),
                                        std::make_move_iterator(system->pending_decoders.end()));
                system->pending_decoders.clear();
            }
        }

        for (auto it = system->decoders.begin(); it != system->decoders.end(); /**/)
        {
            ma_uint64 frames_read = 0;
            ma_result result = ma_decoder_read_pcm_frames(&*it, system->temp_buffer.data(), frame_count, &frames_read);

            // finished or error
            // cleanup and remove
            if (result != MA_SUCCESS || frames_read == 0)
            {
                ma_decoder_uninit(&*it);
                it = system->decoders.erase(it);
                continue;
            }

            ma_uint32 channels = 2;
            for (ma_uint64 i = 0; i < frames_read * channels; ++i)
            {
                out[i] += system->temp_buffer[i];
            }

            ++it;
        }
    }

    void unload_sounds()
    {
        {
            std::lock_guard<std::mutex> lock(audio_system.pending_mutex);

            for (auto& pending_decoders : audio_system.pending_decoders)
            {
                ma_decoder_uninit(&pending_decoders);
            }

            audio_system.pending_decoders.clear();
        }

        for (auto& decoder : audio_system.decoders)
        {
            ma_decoder_uninit(&decoder);
        }

        audio_system.decoders.clear();
    }
}

namespace slv
{
    // private
    bool AudioManager::init()
    {
        if (audio_system.is_initialized.load())
        {
            return true;
        }

        ma_device_config config = ma_device_config_init(ma_device_type_playback);
        ma_format sample_format = ma_format_f32;
        ma_uint32 channels = 2;
        ma_uint32 sample_rate = 48000;
        config.playback.format = sample_format;
        config.playback.channels = channels;
        config.sampleRate = sample_rate;
        config.dataCallback = data_callback;
        config.pUserData = &audio_system;

        ma_result result;

        result = ma_device_init(nullptr, &config, &audio_system.device);
        slv::console_log(slv::log::TRACE, M_NAME, "ma_device_init -> ma_result: {}", static_cast<int>(result));
        
        if (result != MA_SUCCESS)
        {
            return false;
        }

        audio_system.temp_buffer.resize(static_cast<size_t>(4096) * 2, 0.0F);

        result = ma_device_start(&audio_system.device);
        slv::console_log(slv::log::TRACE, M_NAME, "ma_device_start -> ma_result: {}", static_cast<int>(result));

        if (result != MA_SUCCESS)
        {
            ma_device_uninit(&audio_system.device);
            return false;
        }

        audio_system.is_initialized.store(true);

        slv::console_log(slv::log::INFO, M_NAME, "Audio initialized");

        return true;
    }

    // private
    void AudioManager::uninit() const
    {
        if (!audio_system.is_initialized.load())
        {
            return;
        }

        unload_sounds();
        ma_device_uninit(&audio_system.device);

        audio_system.is_initialized.store(false);
    }

    bool AudioManager::play_audio(const std::string& file_path) const
    {
        if (!audio_system.is_initialized.load())
        {
            return false;
        }

        ma_format sample_format = ma_format_f32;
        ma_uint32 channels = 2;
        ma_uint32 sample_rate = 48000;
        ma_decoder decoder{};
        ma_decoder_config decoder_config = ma_decoder_config_init(sample_format, channels, sample_rate);
        ma_result result = ma_decoder_init_file(file_path.c_str(), &decoder_config, &decoder);
        slv::console_log(slv::log::TRACE, M_NAME, "ma_decoder_config_init -> ma_result: {}", static_cast<int>(result));
        if (result != MA_SUCCESS)
        {
            return false;
        }

        {
            std::lock_guard<std::mutex> lock(audio_system.pending_mutex);
            audio_system.pending_decoders.push_back(decoder);
        }

        return true;
    }
}