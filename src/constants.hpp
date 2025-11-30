#pragma once

namespace GameConstants
{
	// --- Design Resolution (1080p) ---
	constexpr float SCREEN_WIDTH = 1920.0f;
	constexpr float SCREEN_HEIGHT = 1080.0f;

	constexpr float MOVEMENT_SPEED = 4.0f;
	constexpr float GRAVITY = 0.75f;
	constexpr float JUMP_FORCE = -16.0f;
	constexpr float GROUND_Y = 800.0f;

	constexpr float BASE_SPRITE_SCALE = 3.0f;

	constexpr int FRAME_RATE = 8;
	constexpr int ACTION_FRAME_RATE = 4;

	// Gameplay tuning
	constexpr int PLAYER_ATTACK_DAMAGE = 40;	  // player deals more damage
	constexpr int ENEMY_ATTACK_DAMAGE = 15;		  // enemy damage
	constexpr float PLAYER_ATTACK_RANGE = 140.0f; // larger, forgiving range
	constexpr float ENEMY_AGGRO_RANGE = 480.0f;	  // enemy aggro range
	constexpr float ENEMY_ATTACK_RANGE = 64.0f;	  // melee range for enemy

	// Separate frame rates for attack animations (higher => faster)
	constexpr int PLAYER_ATTACK_FRAME_RATE = 12;
	constexpr int ENEMY_ATTACK_FRAME_RATE = 8;
}; // namespace GameConstants
