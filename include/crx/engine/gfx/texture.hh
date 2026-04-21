#pragma once
#include <crx/config.hh>
#include <crx/engine/gfx/texture.hh>
#include <crx/engine/math/rect.hh>
#include <crx/engine/math/vec2.hh>
#include <crx/engine/math/size.hh>
#include <crx/engine/util/string_map.hh>
#include <crx/engine/util/pointers.hh>
#include <vector>
#include <cstdint>

//{
//	"texture": "resources/img.png"
//	"run" : {
//		"fps": 12,
//		"loop" : true,
//		"frames" : [
//			{
//				"position": [0, 0] ,
//				"size" : [120, 120] ,
//				"offsets" : [20, 10] ,
//				"scale" : [1, 1] ,
//				"pivot" : [0.5, 0.5] ,
//				"rotation" : 0
//			},
//			{
//				"position": [120, 0] ,
//				"size" : [110, 120] ,
//				"offsets" : [15, 10] ,
//				"scale" : [1, 1] ,
//				"pivot" : [0.5, 0.5] ,
//				"rotation" : 0
//			},
//			{
//				"position": [230, 0] ,
//				"size" : [130, 110] ,
//				"offsets" : [20, 5] ,
//				"scale" : [1, 1] ,
//				"pivot" : [0.5, 0.5] ,
//				"rotation" : 0
//			}
//		]
//	}
//}

namespace crx {

// Raylib's PixelFormat enum
// Note from Raylib: Support depends on OpenGL version and platform
// This enum is simply for dependency decoupling
enum class pixel_format : int {
    uncompressed_grayscale = 1, // 8 bit per pixel (no alpha)
    uncompressed_gray_alpha,    // 8*2 bpp (2 channels)
    uncompressed_r5g6b5,        // 16 bpp
    uncompressed_r8g8b8,        // 24 bpp
    uncompressed_r5g5b5a1,      // 16 bpp (1 bit alpha)
    uncompressed_r4g4b4a4,      // 16 bpp (4 bit alpha)
    uncompressed_r8g8b8a8,      // 32 bpp
    uncompressed_r32,           // 32 bpp (1 channel - float)
    uncompressed_r32g32b32,     // 32*3 bpp (3 channels - float)
    uncompressed_r32g32b32a32,  // 32*4 bpp (4 channels - float)
    uncompressed_r16,           // 16 bpp (1 channel - half float)
    uncompressed_r16g16b16,     // 16*3 bpp (3 channels - half float)
    uncompressed_r16g16b16a16,  // 16*4 bpp (4 channels - half float)
    compressed_dxt1_rgb,        // 4 bpp (no alpha)
    compressed_dxt1_rgba,       // 4 bpp (1 bit alpha)
    compressed_dxt3_rgba,       // 8 bpp
    compressed_dxt5_rgba,       // 8 bpp
    compressed_etc1_rgb,        // 4 bpp
    compressed_etc2_rgb,        // 4 bpp
    compressed_etc2_eac_rgba,   // 8 bpp
    compressed_pvrt_rgb,        // 4 bpp
    compressed_pvrt_rgba,       // 4 bpp
    compressed_astc_4x4_rgba,   // 8 bpp
    compressed_astc_8x8_rgba    // 2 bpp
};

// TODO: constructors

// Raylib's Texture (aka Texture2D) construct
// This type is simply for dependency decoupling
struct CRX_DLL texture final {
    texture();
    texture(
        uint32_t id_val,
        size<int> bounds_val,
        int mipmaps_val,
        pixel_format format_val
    );

	uint32_t id; // OpenGL texture id
	size<int> bounds;
	int mipmaps;
    pixel_format format;
};

struct CRX_DLL texture_atlas final {
	struct CRX_DLL element final {
		struct CRX_DLL frame {
			rect<int> uv_rect;
			vec2<float> offsets;
			vec2<float> scale;
			vec2<float> pivot;
			float rotation;
		};

		float fps;
		bool is_looping;
		std::vector<frame> frames;
	};

	string_map<element> elements;
	sptr<texture> base_texture;
};

}