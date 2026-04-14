#include <slv/types/graphic.hpp>

namespace slv {

atlas_frame::atlas_frame() :
	is_valid(false),
	is_rotated(false)
{}

atlas_data::atlas_data() :
	format(atlas_format::NONE)
{}

texture::texture() :
	id(0),
	mipmaps(0),
	format(0)
{}

texture::texture(
	uint32_t id_val,
	size<int> bounds_val,
	int mipmaps_val,
	int format_val
) :
	id(id_val),
	bounds(bounds_val),
	mipmaps(mipmaps_val),
	format(format_val)
{}

}