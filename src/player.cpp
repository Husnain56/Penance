#include "player.hpp"
#include "enemy.hpp"
#include "constants.hpp"
#include "resources.hpp"
#include <cmath>

Player::Player(Vector2 pos) : Character(pos)
{
	scale = 2.5f;
	accel = 0.7f;
	decel = 0.85f;
	max_speed = 20.0f;
	draw_offset = {0.0f, 40.0f};
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	jump_count = 0;
	attack_hit_registered = false;
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

	// ==============================
	// 1. HORIZONTAL MOVEMENT
	// ==============================

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

	// Clamp speed
	if (velocity_x > max_speed)
		velocity_x = max_speed;
	if (velocity_x < -max_speed)
		velocity_x = -max_speed;

	// Friction
	if (!moving)
	{
		velocity_x *= decel;
		if (std::fabs(velocity_x) < 0.05f)
			velocity_x = 0.0f;
	}

	// Apply X
	position.x += velocity_x;

	// --- Horizontal Collision ---
	float hitW = run_anim.frame_width * scale * 0.5f;
	float hitH = run_anim.frame_rec.height * scale * 0.9f;
	float offX = (run_anim.frame_width * scale - hitW) / 2.0f;
	float offY = (run_anim.frame_rec.height * scale - hitH);

	int leftTile = (int)((position.x + offX) / tileSize);
	int rightTile = (int)((position.x + offX + hitW) / tileSize);
	int topTile = (int)((position.y + offY) / tileSize);
	int bottomTile = (int)((position.y + offY + hitH - 1) / tileSize);

	if (velocity_x > 0) // Right
	{
		if (map.get_tile_id(rightTile, topTile) > 0 || map.get_tile_id(rightTile, bottomTile) > 0)
		{
			position.x = (rightTile * tileSize) - hitW - offX - 0.1f;
			velocity_x = 0;
		}
	}
	else if (velocity_x < 0) // Left
	{
		if (map.get_tile_id(leftTile, topTile) > 0 || map.get_tile_id(leftTile, bottomTile) > 0)
		{
			position.x = ((leftTile + 1) * tileSize) - offX + 0.1f;
			velocity_x = 0;
		}
	}

	// ==============================
	// 2. VERTICAL MOVEMENT & DOUBLE JUMP LOGIC
	// ==============================

	velocity_y += GRAVITY;
	position.y += velocity_y;

	// Recalc grids for Y
	leftTile = (int)((position.x + offX) / tileSize);
	rightTile = (int)((position.x + offX + hitW) / tileSize);
	topTile = (int)((position.y + offY) / tileSize);
	bottomTile = (int)((position.y + offY + hitH) / tileSize);

	// Determine grounded (reliable)
	bool hitMapFloor = (map.get_tile_id(leftTile, bottomTile) > 0
						|| map.get_tile_id(rightTile, bottomTile) > 0);
	bool hitWorldFloor = (position.y >= GROUND_Y);
	bool on_ground = false;

	if (velocity_y > 0) // Falling
	{
		if (hitMapFloor || hitWorldFloor)
		{
			if (hitMapFloor)
				position.y = (bottomTile * tileSize) - offY - hitH;
			else
				position.y = GROUND_Y;

			velocity_y = 0;
			is_jumping = false;

			// RESET DOUBLE JUMP when grounded
			on_ground = true;
			jump_count = 0;
		}
		else
		{
			is_jumping = true;
		}
	}
	else if (velocity_y < 0) // Jumping up
	{
		if (map.get_tile_id(leftTile, topTile) > 0 || map.get_tile_id(rightTile, topTile) > 0)
		{
			position.y = ((topTile + 1) * tileSize) - offY;
			velocity_y = 0;
		}
	}

	// --- DOUBLE JUMP INPUT ---
	// Allow up to 2 jumps (initial + one mid-air)
	if (IsKeyPressed(KEY_SPACE) && jump_count < 2)
	{
		is_jumping = true;
		current_state = STATE_JUMP;
		velocity_y = JUMP_FORCE;

		// Increase counter (initial jump -> 1, second jump -> 2)
		jump_count++;

		// Reset Animation so the second jump feels punchy
		jump_anim.curr_frame = 0;
		jump_anim.frame_counter = 0;
		jump_anim.frame_rec.x = 0.f;
	}

	// --- 4. ATTACK INPUT ---
	if (IsKeyPressed(KEY_E) && !is_attacking)
	{
		current_state = STATE_ATTACK;
		is_attacking = true;
		attack_hit_registered = false; // allow a fresh hit this attack
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

	// --- 6. ANIMATION PLAYBACK & ATTACK HIT LOGIC ---
	if (is_attacking)
	{
		// Faster playback for attack action
		attack_anim.frame_counter++;
		if (attack_anim.frame_counter >= (60 / PLAYER_ATTACK_FRAME_RATE))
		{
			attack_anim.frame_counter = 0;
			attack_anim.curr_frame++;
			if (attack_anim.curr_frame >= attack_anim.total_frame)
			{
				is_attacking = false;
				attack_anim.curr_frame = 0;
				attack_hit_registered = false;
			}
			else
			{
				attack_anim.frame_rec.x = (float)attack_anim.curr_frame * attack_anim.frame_width;
				attack_anim.frame_rec.width = attack_anim.frame_width; // ensure stable width

				// Determine hit frame (near middle)
				int hit_frame = attack_anim.total_frame > 0 ? (attack_anim.total_frame / 2) : 0;

				// Apply damage once at the hit frame to nearest enemy in range and in front
				if (!attack_hit_registered && attack_anim.curr_frame == hit_frame)
				{
					const float range = PLAYER_ATTACK_RANGE * (scale / 2.5f);
					const int dmg = PLAYER_ATTACK_DAMAGE;

					Enemy *closest = nullptr;
					float closestDist = 1e9f;

					for (Enemy *e : Enemy::all())
					{
						if (e == nullptr || !e->is_alive())
							continue;

						Vector2 myPos = get_position();
						Vector2 enemyPos = e->get_position();
						float dx = enemyPos.x - myPos.x;
						float dy = enemyPos.y - myPos.y;
						float dist = std::sqrt(dx * dx + dy * dy);

						// require enemy roughly in front for more consistent hits
						bool inFront = (dx >= 0 && is_facing_right) || (dx <= 0 && !is_facing_right);

						if (dist <= range && inFront && dist < closestDist)
						{
							closestDist = dist;
							closest = e;
						}
					}

					if (closest != nullptr)
					{
						closest->take_damage(dmg);
					}

					attack_hit_registered = true;
				}
			}
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
