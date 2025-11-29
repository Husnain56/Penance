#pragma once

namespace GameConstants
{
	// --- Design Resolution (1080p) ---
	constexpr float SCREEN_WIDTH = 1920.0f;
	constexpr float SCREEN_HEIGHT = 1080.0f;

	constexpr float MOVEMENT_SPEED = 4.0f;
	constexpr float GRAVITY = 0.6f;
	constexpr float JUMP_FORCE = -16.0f;
	constexpr float GROUND_Y = 800.0f;

	constexpr float BASE_SPRITE_SCALE = 3.0f;

	constexpr int FRAME_RATE = 8;
	constexpr int ACTION_FRAME_RATE = 4;
}; // namespace GameConstants
