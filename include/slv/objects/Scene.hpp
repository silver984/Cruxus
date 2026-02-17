#pragma once

#include <slv/objects/Vessel.hpp>

namespace slv
{
	class SceneManager; // forward declare

	class Scene : public slv::Vessel
	{
		friend class slv::SceneManager;

	public:
		inline std::string type() const override
		{
			return "Scene";
		}
	};
}