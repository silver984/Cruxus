#include <slv/handlers/audio_handler.hpp>
#include <slv/core/types/audio.hpp>
#include <slv/core/console_log.hpp>
#include <miniaudio/miniaudio.h>
#include <vector>

namespace
{
    constexpr ma_format SAMPLE_FORMAT = ma_format_f32;
    constexpr unsigned int CHANNELS = 2;
    constexpr unsigned int SAMPLE_RATE = 48000;

    struct audio_sys
    {
        ma_device device{};
        std::vector<slv::audio_hnd> handles;
        std::vector<float> temp_buffer;
        bool is_initialized = false;
    };

    audio_sys audio_system;

    void data_callback(ma_device* device, void* output, const void*, ma_uint32 frame_count)
    {
        auto* audio = static_cast<audio_sys*>(device->pUserData);
        float* out = static_cast<float*>(output);

        // clear output
        size_t sample_count = static_cast<size_t>(frame_count) * device->playback.channels;
        std::fill(out, out + sample_count, 0.0F);

        // mix each active sound
        for (auto& handle : audio->handles)
        {
            if (!handle.active)
            {
                continue;
            }

            ma_uint64 frames_read = 0;
            ma_result result = ma_decoder_read_pcm_frames(&handle.decoder, audio->temp_buffer.data(), frame_count, &frames_read);
            
            if (result != MA_SUCCESS || frames_read == 0)
            {
                handle.active = false;
                continue;
            }

            for (ma_uint64 i = 0; i < frames_read * CHANNELS; ++i)
            {
                out[i] += audio->temp_buffer[i] * handle.volume;
            }
        }
    }

    void unload_sounds()
    {
        for (auto& s : audio_system.handles)
        {
            if (s.active)
            {
                ma_decoder_uninit(&s.decoder);
                s.active = false;
            }
        }

        audio_system.handles.clear();
    }
}

namespace slv
{
    // private
    AudioHandler::~AudioHandler()
    {
        uninit();
    }

    // private
    bool AudioHandler::init()
    {
        if (audio_system.is_initialized)
        {
            return true;
        }

        ma_result result;

        ma_device_config config = ma_device_config_init(ma_device_type_playback);

        config.playback.format = SAMPLE_FORMAT;
        config.playback.channels = CHANNELS;
        config.sampleRate = SAMPLE_RATE;
        config.dataCallback = data_callback;
        config.pUserData = &audio_system;

        result = ma_device_init(nullptr, &config, &audio_system.device);
        
        if (result != MA_SUCCESS)
        {
            return false;
        }

        audio_system.temp_buffer.resize(4096Ui64 * CHANNELS, 0.0F);

        if (ma_device_start(&audio_system.device) != MA_SUCCESS)
        {
            ma_device_uninit(&audio_system.device);
            return false;
        }

        audio_system.is_initialized = true;

        slv::console_log(slv::LOG_INFO, M_CLASS_NAME,
                         "Audio initialized");

        return true;
    }

    // private
    void AudioHandler::uninit() const
    {
        if (!audio_system.is_initialized)
        {
            return;
        }

        unload_sounds();
        ma_device_uninit(&audio_system.device);

        audio_system.is_initialized = false;
    }

    bool AudioHandler::play_audio(const std::string& file_path) const
    {
        if (!audio_system.is_initialized)
        {
            return false;
        }

        slv::audio_hnd s;
        
        ma_decoder_config decoder_config = ma_decoder_config_init(SAMPLE_FORMAT, CHANNELS, SAMPLE_RATE);

        ma_result result = ma_decoder_init_file(file_path.c_str(), &decoder_config, &s.decoder);

        slv::console_log(slv::LOG_INFO, M_CLASS_NAME,
                         "result: {}", static_cast<int>(result));

        ma_backend backends[16];
        size_t backend_count = 0;

        ma_result r = ma_get_enabled_backends(
            backends,
            16,
            &backend_count
        );

        slv::console_log(slv::LOG_INFO, M_CLASS_NAME,
                         "ma_get_enabled_backends result: {}, count: {}",
                         static_cast<int>(r),
                         backend_count);

        if (result != MA_SUCCESS)
        {
            return false;
        }

        s.active = true;
        // s.volume = volume;
        audio_system.handles.push_back(s);

        return true;
    }
}