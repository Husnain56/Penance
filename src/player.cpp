#include <algorithm>
#include <cmath>
#include <constants.hpp>
#include <enemy.hpp>
#include <player.hpp>
#include <resources.hpp>

Player::Player(Vector2 pos) : Character(pos)
{
	scale = 2.5f;
	accel = 0.75f;
	decel = 0.75f;
	max_speed = 5.0f;
	draw_offset = {0.0f, 40.0f}; // Adjust based on your sprite assets

	// Constants (set once)
	dash_frames_total = 10;
	dash_speed = 30.0f;
	dash_cooldown_frames = 30;

	reset(); // Initialize state variables
}

void Player::init()
{
	using namespace Resources::PlayerResource;
	load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
	load_texture(STATE_IDLE, IDLE_TEXTURE.c_str(), IDLE_FRAMES);
	load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
	load_texture(STATE_JUMP, JUMP_TEXTURE.c_str(), JUMP_FRAMES);
	load_texture(STATE_HURT, HURT_TEXTURE.c_str(), HURT_FRAMES);
	load_texture(STATE_DEAD, DEAD_TEXTURE.c_str(), DEAD_FRAMES);
}

void Player::reset()
{
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	jump_count = 0;
	hp = get_max_hp();

	// Movement / dash
	is_dashing = false;
	dash_frames_remaining = 0;
	dash_cooldown_timer = 0;

	// Character / animation flags (important)
	is_attacking = false;
	is_jumping = false;
	is_facing_right = true;
	current_state = STATE_IDLE;

	// Clear death/hurt
	dead_anim_playing = false;
	pending_removal = false;
	hurt_anim.curr_frame = hurt_anim.frame_counter = hurt_anim.frame_rec.x = 0.0f;
	attack_anim.curr_frame = attack_anim.frame_counter = attack_anim.frame_rec.x = 0.0f;
	run_anim.curr_frame = run_anim.frame_counter = run_anim.frame_rec.x = 0.0f;
	idle_anim.curr_frame = idle_anim.frame_counter = idle_anim.frame_rec.x = 0.0f;
	dead_anim.curr_frame = dead_anim.frame_counter = dead_anim.frame_rec.x = 0.0f;

	// Reset knockback / hurt timers
	knockback_vel_x = 0.0f;
	// If you want to zero-out hurt frames too:
	// hurt_frames_remaining = 0;

	attack_hit_enemies.clear();
	attack_hit_enemies.reserve(8); // optional small reserve to avoid reallocts

	// Default Spawn
	set_position({200.0f, GameConstants::GROUND_Y - 200.0f});
}

void Player::update(const Map &map)
{
	using namespace GameConstants;

	if (is_removed())
		return;

	// 1. High Priority States (Dead/Hurt) - Skip logic
	if (current_state == STATE_DEAD || current_state == STATE_HURT)
	{
		process_state();
		return;
	}

	float tileSize = map.get_tile_size();

	// --- Shared Hitbox Calculation (Done once per frame) ---
	// Note: Assuming run_anim is representative of the body size
	float hitW = run_anim.frame_width * scale * 0.5f;
	float hitH = run_anim.frame_rec.height * scale * 0.9f;
	float offX = (run_anim.frame_width * scale - hitW) / 2.0f;
	float offY = (run_anim.frame_rec.height * scale - hitH);

	// Grid Coordinates
	int leftTile = (int)((position.x + offX) / tileSize);
	int rightTile = (int)((position.x + offX + hitW) / tileSize);
	int topTile = (int)((position.y + offY) / tileSize);
	int bottomTile = (int)((position.y + offY + hitH - 1) / tileSize);

	// --- Input & Dash Timers ---
	if (dash_cooldown_timer > 0)
		--dash_cooldown_timer;

	bool wantRight = IsKeyDown(KEY_D);
	bool wantLeft = IsKeyDown(KEY_A);
	bool shiftPressed = IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT);

	// --- DASH LOGIC ---
	if (shiftPressed && !is_dashing && dash_cooldown_timer == 0 && !is_jumping)
	{
		int dir = 0;
		if (wantRight)
			dir = 1;
		else if (wantLeft)
			dir = -1;
		else
			dir = is_facing_right ? 1 : -1;

		is_dashing = true;
		dash_frames_remaining = dash_frames_total;
		velocity_x = dir * dash_speed;

		current_state = STATE_RUN; // Visual override
		// Reset run anim to make dash look responsive
		run_anim.curr_frame = 0;
		run_anim.frame_rec.x = 0.f;
	}

	// --- MOVEMENT PHYSICS ---
	if (is_dashing)
	{
		// Dash: No friction, fixed velocity, timer countdown
		if (--dash_frames_remaining <= 0)
		{
			is_dashing = false;
			velocity_x = 0.0f;
			dash_cooldown_timer = dash_cooldown_frames;
		}
	}
	else
	{
		// Normal Move
		bool moving = false;
		if (wantRight)
		{
			velocity_x += accel;
			is_facing_right = true;
			moving = true;
		}
		if (wantLeft)
		{
			velocity_x -= accel;
			is_facing_right = false;
			moving = true;
		}

		if (std::abs(velocity_x) > max_speed)
			velocity_x = (velocity_x > 0 ? max_speed : -max_speed);

		if (!moving)
		{
			velocity_x *= decel;
			if (std::abs(velocity_x) < 0.1f)
				velocity_x = 0.0f;
		}
	}

	// Apply X & Check Collision
	position.x += velocity_x;

	// Recalc X tiles after move
	leftTile = (int)((position.x + offX) / tileSize);
	rightTile = (int)((position.x + offX + hitW) / tileSize);

	if (velocity_x > 0
		&& (map.get_tile_id(rightTile, topTile) > 0 || map.get_tile_id(rightTile, bottomTile) > 0))
	{
		position.x = (rightTile * tileSize) - hitW - offX - 0.1f;
		velocity_x = 0;
		if (is_dashing)
		{
			is_dashing = false;
			dash_frames_remaining = 0;
		}
	}
	else if (velocity_x < 0
			 && (map.get_tile_id(leftTile, topTile) > 0
				 || map.get_tile_id(leftTile, bottomTile) > 0))
	{
		position.x = ((leftTile + 1) * tileSize) - offX + 0.1f;
		velocity_x = 0;
		if (is_dashing)
		{
			is_dashing = false;
			dash_frames_remaining = 0;
		}
	}

	// --- VERTICAL PHYSICS ---
	velocity_y += GRAVITY;
	position.y += velocity_y;

	// Recalc Y tiles after move
	topTile = (int)((position.y + offY) / tileSize);
	bottomTile = (int)((position.y + offY + hitH) / tileSize); // hitH full check

	// We need to re-check X tiles because X-collision might have shifted us
	leftTile = (int)((position.x + offX) / tileSize);
	rightTile = (int)((position.x + offX + hitW) / tileSize);

	bool hitMapFloor
		= (map.get_tile_id(leftTile, bottomTile) > 0 || map.get_tile_id(rightTile, bottomTile) > 0);
	bool hitWorldFloor = (position.y >= GROUND_Y);

	if (velocity_y > 0.0f) // Falling
	{
		if (hitMapFloor || hitWorldFloor)
		{
			if (hitMapFloor)
				position.y = (bottomTile * tileSize) - offY - hitH;
			else
				position.y = GROUND_Y;

			velocity_y = 0;
			is_jumping = false;
			jump_count = 0; // Reset double jump
		}
		else
		{
			is_jumping = true;
		}
	}
	else if (velocity_y < 0.0f) // Jumping Up
	{
		if (map.get_tile_id(leftTile, topTile) > 0 || map.get_tile_id(rightTile, topTile) > 0)
		{
			position.y = ((topTile + 1) * tileSize) - offY;
			velocity_y = 0;
		}
	}

	// --- JUMP INPUT ---
	if (IsKeyPressed(KEY_SPACE) && jump_count < 2)
	{
		is_jumping = true;
		current_state = STATE_JUMP;
		velocity_y = JUMP_FORCE;
		jump_count++;

		// Reset jump anim
		jump_anim.curr_frame = 0;
		jump_anim.frame_counter = 0;
	}

	// --- ATTACK INPUT ---
	if (IsKeyPressed(KEY_E) && !is_attacking)
	{
		current_state = STATE_ATTACK;
		is_attacking = true;
		attack_hit_enemies.clear();
		attack_anim.curr_frame = 0;
		attack_anim.frame_counter = 0;
	}

	// --- STATE MANAGEMENT ---
	if (!is_jumping && !is_attacking && current_state != STATE_HURT)
	{
		current_state = (std::abs(velocity_x) > 0.5f) ? STATE_RUN : STATE_IDLE;
	}

	// =========================================================
	// REVISED ATTACK LOGIC
	// =========================================================
	if (is_attacking)
	{
		// 1. Advance Animation
		attack_anim.frame_counter++;
		if (attack_anim.frame_counter >= (60 / PLAYER_ATTACK_FRAME_RATE))
		{
			attack_anim.frame_counter = 0;
			attack_anim.curr_frame++;
			if (attack_anim.curr_frame >= attack_anim.total_frame)
			{
				is_attacking = false;
				attack_anim.curr_frame = 0;
				attack_hit_enemies.clear();
			}
			else
			{
				attack_anim.frame_rec.x = (float)attack_anim.curr_frame * attack_anim.frame_width;
			}
		}

		// 2. Determine "Active Frames"
		// Example: If total frames is 6, the hit happens roughly on frames 2, 3, 4.
		// Adjust these numbers based on your specific sprite sheet visually!
		int start_hit_frame = 1;
		int end_hit_frame = std::max(1, (int)attack_anim.total_frame - 2);

		bool is_active_frame = (attack_anim.curr_frame >= start_hit_frame
								&& attack_anim.curr_frame <= end_hit_frame);

		if (is_active_frame)
		{
			const float baseRange = PLAYER_ATTACK_RANGE * (scale / 2.5f);

			// REDUCED BONUS: was * 6.0f, now * 1.5f.
			// This adds a slight lunge feeling without creating a phantom hitbox.
			const float speedBonus = std::abs(velocity_x) * 1.5f;
			const float effectiveRange = baseRange + speedBonus;
			const int dmg = PLAYER_ATTACK_DAMAGE;

			Vector2 myPos = get_position();

			for (Enemy *e : Enemy::all())
			{
				if (e == nullptr || !e->is_alive())
					continue;

				// Check if already hit this specific attack swing
				if (std::find(attack_hit_enemies.begin(), attack_hit_enemies.end(), e)
					!= attack_hit_enemies.end())
					continue;

				Vector2 enemyPos = e->get_position();
				float dx = enemyPos.x - myPos.x;
				float dy = enemyPos.y - myPos.y; // Keep Y check for vertical alignment

				// Quick Box Check first (Optimization)
				if (std::abs(dx) > effectiveRange || std::abs(dy) > effectiveRange)
					continue;

				float dist = std::sqrt(dx * dx + dy * dy);

				// STRICTER DIRECTION CHECK:
				// 1. Must be facing the enemy.
				// 2. OR, must be extremely close (overlapping body) to account for
				// "inside" hits.
				bool inFront = (dx > 0 && is_facing_right) || (dx < 0 && !is_facing_right);
				bool overlapping = dist < (hitW * 0.8f);

				if (dist <= effectiveRange && (inFront || overlapping))
				{
					e->take_damage(dmg);
					if (!e->is_alive())
						this->heal(10);
					attack_hit_enemies.push_back(e);
				}
			}
		}
	}
	// --- OTHER ANIMATIONS ---
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
		int run_anim_rate = is_dashing ? (FRAME_RATE * 3) : FRAME_RATE;
		run_anim.frame_counter++;
		if (run_anim.frame_counter >= (60 / run_anim_rate))
		{
			run_anim.frame_counter = 0;
			run_anim.curr_frame = (run_anim.curr_frame + 1) % run_anim.total_frame;
			run_anim.frame_rec.x = (float)run_anim.curr_frame * run_anim.frame_width;
		}
	}
	else // IDLE
	{
		idle_anim.frame_counter++;
		if (idle_anim.frame_counter >= (60 / FRAME_RATE))
		{
			idle_anim.frame_counter = 0;
			idle_anim.curr_frame = (idle_anim.curr_frame + 1) % idle_anim.total_frame;
			idle_anim.frame_rec.x = (float)idle_anim.curr_frame * idle_anim.frame_width;
		}
	}

	process_state();
}
