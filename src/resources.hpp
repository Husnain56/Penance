#pragma once

#include <string>

namespace Resources
{
	namespace KitsuneResource
	{
		inline const std::string BASE_PATH = "resources/enemies/kitsune/";

		inline const std::string RUN_TEXTURE = BASE_PATH + "run.png";
		inline const std::string IDLE_TEXTURE = BASE_PATH + "idle.png";
		inline const std::string ATTACK_TEXTURE = BASE_PATH + "attack_3.png";
		inline const std::string JUMP_TEXTURE = BASE_PATH + "jump.png";

		constexpr int RUN_FRAMES = 8;
		constexpr int IDLE_FRAMES = 8;
		constexpr int ATTACK_FRAMES = 7;
		constexpr int JUMP_FRAMES = 10;
	} // namespace KitsuneResource

	namespace PlayerResource
	{
		inline const std::string BASE_PATH = "resources/player/";
		inline const std::string RUN_TEXTURE = BASE_PATH + "run.png";
		inline const std::string IDLE_TEXTURE = BASE_PATH + "idle.png";
		inline const std::string ATTACK_TEXTURE = BASE_PATH + "attack_3.png";
		inline const std::string JUMP_TEXTURE = BASE_PATH + "jump.png";

		constexpr int RUN_FRAMES = 8;
		constexpr int IDLE_FRAMES = 5;
		constexpr int ATTACK_FRAMES = 7;
		constexpr int JUMP_FRAMES = 3;
	} // namespace PlayerResource

	namespace BlueSamuraiResource
	{
		inline const std::string BASE_PATH = "resources/enemies/blue_samurai/";
		inline const std::string RUN_TEXTURE = BASE_PATH + "run.png";
		inline const std::string IDLE_TEXTURE = BASE_PATH + "idle.png";
		inline const std::string ATTACK_TEXTURE = BASE_PATH + "attack.png";
		inline const std::string JUMP_TEXTURE = BASE_PATH + "jump.png";

		constexpr int RUN_FRAMES = 8;
		constexpr int IDLE_FRAMES = 14;
		constexpr int ATTACK_FRAMES = 11;
		constexpr int JUMP_FRAMES = 7;
	} // namespace BlueSamuraiResource

	namespace SkeletonWarriorResource
	{
		inline const std::string BASE_PATH = "resources/enemies/skeleton_warrior/";
		inline const std::string RUN_TEXTURE = BASE_PATH + "run.png";
		inline const std::string IDLE_TEXTURE = BASE_PATH + "idle.png";
		inline const std::string ATTACK_TEXTURE = BASE_PATH + "run_attack.png";

		constexpr int RUN_FRAMES = 8;
		constexpr int IDLE_FRAMES = 7;
		constexpr int ATTACK_FRAMES = 0;
	} // namespace BlueSamuraiResource

	namespace PurpleKnightResource
	{
		inline const std::string BASE_PATH = "resources/enemies/purple_knight/";

		inline const std::string RUN_TEXTURE = BASE_PATH + "run.png";
		inline const std::string IDLE_TEXTURE = BASE_PATH + "idle.png";
		inline const std::string ATTACK_TEXTURE = BASE_PATH + "run_attack.png";
		inline const std::string JUMP_TEXTURE = BASE_PATH + "jump.png";

		constexpr int RUN_FRAMES = 7;
		constexpr int IDLE_FRAMES = 4;
		constexpr int ATTACK_FRAMES = 6;
		constexpr int JUMP_FRAMES = 6;
	}

	namespace SilverKnightResource
	{
		inline const std::string BASE_PATH = "resources/enemies/silver_knight/";

		inline const std::string RUN_TEXTURE = BASE_PATH + "run.png";
		inline const std::string IDLE_TEXTURE = BASE_PATH + "idle.png";
		inline const std::string ATTACK_TEXTURE = BASE_PATH + "run_attack.png";
		inline const std::string JUMP_TEXTURE = BASE_PATH + "jump.png";

		constexpr int RUN_FRAMES = 7;
		constexpr int IDLE_FRAMES = 4;
		constexpr int ATTACK_FRAMES = 6;
		constexpr int JUMP_FRAMES = 6;
	}

	namespace SkeletonSpearmanResource
	{
		inline const std::string BASE_PATH = "resources/enemies/skeleton_spearman/";
		
		inline const std::string RUN_TEXTURE = BASE_PATH + "run.png";
		inline const std::string IDLE_TEXTURE = BASE_PATH + "idle.png";
		inline const std::string ATTACK_TEXTURE = BASE_PATH + "run_attack.png";

		constexpr int RUN_FRAMES = 6;
		constexpr int IDLE_FRAMES = 7;
		constexpr int ATTACK_FRAMES = 5;
	}

	namespace KarasuTenguResource
	{
		inline const std::string BASE_PATH = "resources/enemies/karasu_tengu/";
		inline const std::string RUN_TEXTURE = BASE_PATH + "run.png";
		inline const std::string IDLE_TEXTURE = BASE_PATH + "idle.png";
		inline const std::string ATTACK_TEXTURE = BASE_PATH + "attack_1.png";
		inline const std::string JUMP_TEXTURE = BASE_PATH + "jump.png";

		constexpr int RUN_FRAMES = 8;
		constexpr int IDLE_FRAMES = 6;
		constexpr int ATTACK_FRAMES = 6;
		constexpr int JUMP_FRAMES = 15;
	} // namespace KarasuTenguResource

	namespace YamaBushiResource
	{
		inline const std::string BASE_PATH = "resources/enemies/yamabushi_tengu/";
		
		inline const std::string RUN_TEXTURE = BASE_PATH + "run.png";
		inline const std::string IDLE_TEXTURE = BASE_PATH + "idle.png";
		inline const std::string ATTACK_TEXTURE = BASE_PATH + "attack_2.png";
		
		constexpr int RUN_FRAMES = 8;
		constexpr int IDLE_FRAMES = 6;
		constexpr int ATTACK_FRAMES = 6;
	} // namespace YamaBushiResource

	namespace MapResource
	{
		inline const std::string BASE_PATH = "resources/maps/";
		inline const std::string CASTLE_IMAGE = BASE_PATH + "castle/castle_tiles.png";
		inline const std::string CASTLE_CSV = BASE_PATH + "castle/castle_map.csv";
		inline const std::string BACKGROUND_IMAGE = BASE_PATH + "background/battleground.png";
	} // namespace MapResource

} // namespace Resources
