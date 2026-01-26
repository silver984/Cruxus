#include <slv/types.hpp>
#include <raylib.h>

namespace slv
{
	texture::~texture()
	{
		if (id != 0u)
		{
			UnloadTexture(Texture{ id, width, height, mipmaps, format });
		}
	}
}