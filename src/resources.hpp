#pragma once

#include <string>

namespace Resources
{
	namespace Kitsune
	{
		inline const std::string BASE_PATH = "resources/enemies/kitsune/";

		inline const std::string RUN_TEXTURE = BASE_PATH + "Run.png";
		inline const std::string IDLE_TEXTURE = BASE_PATH + "Idle.png";
		inline const std::string ATTACK_TEXTURE = BASE_PATH + "Attack_3.png";
		inline const std::string JUMP_TEXTURE = BASE_PATH + "Jump.png";

		constexpr int RUN_FRAMES = 8;
		constexpr int IDLE_FRAMES = 8;
		constexpr int ATTACK_FRAMES = 7;
		constexpr int JUMP_FRAMES = 10;
	} // namespace Kitsune
} // namespace Resources
