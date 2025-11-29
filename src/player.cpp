#include "player.hpp"
#include "constants.hpp"
#include "resources.hpp"
#include <cmath>

Player::Player(Vector2 pos) : Character(pos)
{
	scale = 2.5f;
	accel = 0.7f;
	decel = 0.85f;
	max_speed = 6.0f;
	draw_offset = {0.0f, 40.0f};
	velocity_x = 0.0f;
}

void Player::init()
{
	using namespace Resources::PlayerResource;

	load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
	load_texture(STATE_IDLE, IDLE_TEXTURE.c_str(), IDLE_FRAMES);
	load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
	load_texture(STATE_JUMP, JUMP_TEXTURE.c_str(), JUMP_FRAMES);
}

void Player::update(const Map &map)
{
	using namespace GameConstants;
	float tileSize = map.get_tile_size();

	// --- 1. HORIZONTAL MOVEMENT & COLLISION ---

	// Input Logic
	bool moving = false;
	if (IsKeyDown(KEY_D))
	{
		velocity_x += accel;
		is_facing_right = true;
		moving = true;
	}
	if (IsKeyDown(KEY_A))
	{
		velocity_x -= accel;
		is_facing_right = false;
		moving = true;
	}
	if (velocity_x > max_speed)
		velocity_x = max_speed;
	if (velocity_x < -max_speed)
		velocity_x = -max_speed;
	if (!moving)
	{
		velocity_x *= decel;
		if (std::fabs(velocity_x) < 0.05f)
			velocity_x = 0.0f;
	}

	// Apply X Move
	position.x += velocity_x;

	// Check Wall Collision
	// We define a "Hitbox" slightly smaller than the sprite
	float hitW = run_anim.frame_width * scale * 0.5f;		   // Width of hitbox
	float hitH = run_anim.frame_rec.height * scale * 0.9f;	   // Height of hitbox
	float offX = (run_anim.frame_width * scale - hitW) / 2.0f; // Center it
	float offY = (run_anim.frame_rec.height * scale - hitH);

	// Calculate grid coordinates of player's edges
	int leftTile = (int)((position.x + offX) / tileSize);
	int rightTile = (int)((position.x + offX + hitW) / tileSize);
	int topTile = (int)((position.y + offY) / tileSize);
	int bottomTile = (int)((position.y + offY + hitH - 1) / tileSize); // -1 buffer

	// Check corners for walls
	bool collisionX = false;
	if (velocity_x > 0) // Moving Right
	{
		if (map.get_tile_id(rightTile, topTile) > 0 || map.get_tile_id(rightTile, bottomTile) > 0)
		{
			position.x = (rightTile * tileSize) - hitW - offX - 0.1f; // Snap to left of wall
			velocity_x = 0;
			collisionX = true;
		}
	}
	else if (velocity_x < 0) // Moving Left
	{
		if (map.get_tile_id(leftTile, topTile) > 0 || map.get_tile_id(leftTile, bottomTile) > 0)
		{
			position.x = ((leftTile + 1) * tileSize) - offX + 0.1f; // Snap to right of wall
			velocity_x = 0;
			collisionX = true;
		}
	}

	// --- 2. VERTICAL MOVEMENT & COLLISION ---

	velocity_y += GRAVITY;
	position.y += velocity_y;

	// Recalculate grid coords after Y move
	leftTile = (int)((position.x + offX) / tileSize);
	rightTile = (int)((position.x + offX + hitW) / tileSize);
	topTile = (int)((position.y + offY) / tileSize);
	bottomTile = (int)((position.y + offY + hitH) / tileSize);

	if (velocity_y > 0) // Falling
	{
		// Check feet
		if (map.get_tile_id(leftTile, bottomTile) > 0 || map.get_tile_id(rightTile, bottomTile) > 0)
		{
			position.y = (bottomTile * tileSize) - offY - hitH; // Snap to floor
			velocity_y = 0;
			is_jumping = false;
		}
		else
		{
			is_jumping = true; // Falling into air
		}
	}
	else if (velocity_y < 0) // Jumping up
	{
		// Check head
		if (map.get_tile_id(leftTile, topTile) > 0 || map.get_tile_id(rightTile, topTile) > 0)
		{
			position.y = ((topTile + 1) * tileSize) - offY; // Snap to bottom of ceiling
			velocity_y = 0;
		}
	}

	// --- 3. JUMP INPUT ---
	if (IsKeyPressed(KEY_SPACE) && !is_jumping)
	{
		is_jumping = true;
		current_state = STATE_JUMP;
		velocity_y = JUMP_FORCE;
		// Reset anim...
		jump_anim.curr_frame = 0;
		jump_anim.frame_counter = 0;
	}

	// --- 4. ATTACK INPUT ---
	if (IsKeyPressed(KEY_E) && !is_attacking)
	{
		current_state = STATE_ATTACK;
		is_attacking = true;
		attack_anim.curr_frame = 0;
		attack_anim.frame_counter = 0;
	}

	// --- 5. ANIMATION STATE ---
	if (!is_jumping && !is_attacking)
	{
		if (std::fabs(velocity_x) > 0.5f)
			current_state = STATE_RUN;
		else
			current_state = STATE_IDLE;
	}

	// --- 6. ANIMATION PLAYBACK (Unchanged) ---
	// (Copy your existing animation update logic here: if(is_attacking)... else if(is_jumping)...)
	// I am omitting the 50 lines of animation code for brevity, but KEEP IT exactly as you had it!
	if (is_attacking)
	{
		attack_anim.frame_counter++;
		if (attack_anim.frame_counter >= (60 / FRAME_RATE))
		{
			attack_anim.frame_counter = 0;
			attack_anim.curr_frame++;
			if (attack_anim.curr_frame >= attack_anim.total_frame)
			{
				is_attacking = false;
				attack_anim.curr_frame = 0;
			}
			attack_anim.frame_rec.x = (float)attack_anim.curr_frame * attack_anim.frame_width;
		}
	}
	else if (is_jumping)
	{
		jump_anim.frame_counter++;
		if (jump_anim.frame_counter >= (60 / FRAME_RATE))
		{
			jump_anim.frame_counter = 0;
			if (jump_anim.curr_frame < jump_anim.total_frame - 1)
				jump_anim.curr_frame++;
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
	else
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
