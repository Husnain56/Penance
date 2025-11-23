#include "player.hpp"
#include "constants.hpp"
#include "resources.hpp"

Player::Player(Vector2 pos) : Character(pos)
{
	scale = 2.5f;
}

void Player::init()
{
	using namespace Resources::PlayerResource;

	load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
	load_texture(STATE_IDLE, IDLE_TEXTURE.c_str(), IDLE_FRAMES);
	load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
	load_texture(STATE_JUMP, JUMP_TEXTURE.c_str(), JUMP_FRAMES);
}

void Player::update()
{
	using namespace GameConstants;

	// ==============================
	// 1. PHYSICS ENGINE
	// ==============================

	// Apply Gravity (Always pulls down)
	velocity_y += GRAVITY;

	// Apply Velocity to Position
	position.y += velocity_y;

	// Ground Collision (Floor Check)
	if (position.y >= GROUND_Y)
	{
		position.y = GROUND_Y; // Snap to floor
		velocity_y = 0;		   // Stop falling
		is_jumping = false;	   // We have landed

		// If we landed while in JUMP state, return to IDLE (or RUN if moving)
		if (current_state == STATE_JUMP)
		{
			// If keys are held, go to run, otherwise idle
			if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
				current_state = STATE_RUN;
			else
				current_state = STATE_IDLE;
		}
	}
	else
	{
		// If we are in the air (not on ground), we are technically jumping/falling
		is_jumping = true;
	}

	// ==============================
	// 2. INPUT HANDLING
	// ==============================

	// --- Jump Input ---
	// Only allow jump if we are on the ground (!is_jumping)
	if (IsKeyPressed(KEY_SPACE) && !is_jumping)
	{
		is_jumping = true;
		current_state = STATE_JUMP;
		velocity_y = JUMP_FORCE; // Instant upward force

		// Reset Jump Animation
		jump_anim.curr_frame = 0;
		jump_anim.frame_counter = 0;
		jump_anim.frame_rec.x = 0.f;
	}

	// --- Attack Input ---
	else if (IsKeyPressed(KEY_E) && !is_attacking)
	{
		current_state = STATE_ATTACK;
		is_attacking = true;
		attack_anim.frame_counter = 0;
		attack_anim.curr_frame = 0;
		attack_anim.frame_rec.x = 0.f;
	}

	// --- Horizontal Movement ---
	// We allow moving Left/Right even while in the air (Air Control)
	if (IsKeyDown(KEY_D))
	{
		// Interrupt Attack if moving
		if (is_attacking)
		{
			is_attacking = false;
		}

		position.x += MOVEMENT_SPEED;
		is_facing_right = true;

		// Only change state to RUN if we are on the ground
		if (!is_jumping)
			current_state = STATE_RUN;
	}
	else if (IsKeyDown(KEY_A))
	{
		if (is_attacking)
		{
			is_attacking = false;
		}

		position.x -= MOVEMENT_SPEED;
		is_facing_right = false;

		if (!is_jumping)
			current_state = STATE_RUN;
	}
	// If no keys pressed and on ground, go to IDLE
	else if (!is_jumping && !is_attacking)
	{
		current_state = STATE_IDLE;
	}

	// ==============================
	// 3. ANIMATION LOGIC
	// ==============================

	// NOTE: Animation logic is now purely visual. It does NOT affect position.

	if (is_attacking)
	{
		attack_anim.frame_counter++;
		if (attack_anim.frame_counter >= (60 / FRAME_RATE))
		{
			attack_anim.frame_counter = 0;
			attack_anim.curr_frame++;

			if (attack_anim.curr_frame >= attack_anim.total_frame)
			{
				is_attacking = false; // Attack finished
				attack_anim.curr_frame = 0;
			}
			else
			{
				attack_anim.frame_rec.x = (float)attack_anim.curr_frame * attack_anim.frame_width;
			}
		}
	}
	else if (is_jumping) // In the air
	{
		// Simply play the jump animation.
		// It doesn't need to control Y position anymore.
		jump_anim.frame_counter++;
		if (jump_anim.frame_counter >= (60 / FRAME_RATE))
		{
			jump_anim.frame_counter = 0;

			// Stop at the last frame so it doesn't loop while falling
			if (jump_anim.curr_frame < jump_anim.total_frame - 1)
			{
				jump_anim.curr_frame++;
			}
			jump_anim.frame_rec.x = (float)jump_anim.curr_frame * jump_anim.frame_width;
		}
	}
	else if (current_state == STATE_RUN)
	{
		run_anim.frame_counter++;
		if (run_anim.frame_counter >= (60 / FRAME_RATE))
		{
			run_anim.frame_counter = 0;
			run_anim.curr_frame = (run_anim.curr_frame + 1) % run_anim.total_frame;
			run_anim.frame_rec.x = (float)run_anim.curr_frame * run_anim.frame_width;
		}
	}
	else // Idle
	{
		idle_anim.frame_counter++;
		if (idle_anim.frame_counter >= (60 / FRAME_RATE))
		{
			idle_anim.frame_counter = 0;
			idle_anim.curr_frame = (idle_anim.curr_frame + 1) % idle_anim.total_frame;
			idle_anim.frame_rec.x = (float)idle_anim.curr_frame * idle_anim.frame_width;
		}
	}
}
