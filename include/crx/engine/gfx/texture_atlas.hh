#pragma once
#include <crx/config.hh>
#include <crx/engine/gfx/texture.hh>
#include <crx/engine/math/rect.hh>
#include <crx/engine/math/vec2.hh>
#include <crx/engine/util/string_map.hh>
#include <vector>

//{
//	"texture": "resources/img.png"
//		"run" : {
//		"fps": 12,
//			"loop" : true,
//			"frames" : [
//		{
//			"position": [0, 0] ,
//				"size" : [120, 120] ,
//				"offsets" : [20, 10] ,
//				"scale" : [1, 1] ,
//				"pivot" : [0.5, 0.5] ,
//				"rotation" : 0
//		},
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
//			]
//	}
//}

namespace crx {

struct CRX_DLL texture_atlas final {
	struct CRX_DLL element final {
		rect<int> rect; // pixel bounds in atlas
		vec2<float> offsets; // draw-time positional adjustment
	};

	string_map<element> elements;
	texture base_texture;
};

}