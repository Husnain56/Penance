#include <algorithm>
#include <cmath>
#include <constants.hpp>
#include <enemy.hpp>
#include <player.hpp>
#include <resources.hpp>

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

	// Dash defaults: adjust to taste
	is_dashing = false;
	dash_frames_total = 10; // number of frames dash lasts
	dash_frames_remaining = 0;
	dash_speed = 30.0f;		   // pixels per frame -> dash distance ~ dash_speed * dash_frames_total
	dash_cooldown_frames = 30; // cooldown in frames (~0.5s at 60FPS)
	dash_cooldown_timer = 0;
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

void Player::update(const Map &map)
{
	using namespace GameConstants;
	float tileSize = map.get_tile_size();

	if (is_removed())
		return;

	// If currently playing dead animation OR hurt state is active, let base class progress it and skip gameplay logic.
	// process_state handles the dead animation, transition, and hurt knockback/anim.
	if (current_state == STATE_DEAD || current_state == STATE_HURT)
	{
		process_state();
		return; // Skip all input, movement, attack, jump, dash logic.
	}

	// reduce dash cooldown timer (frame-based)
	if (dash_cooldown_timer > 0)
		--dash_cooldown_timer;

	// ==============================
	// 0. Read directional intent (but do not apply yet)
	// ==============================
	bool wantRight = IsKeyDown(KEY_D);
	bool wantLeft = IsKeyDown(KEY_A);

	// ==============================
	// DASH: trigger on SHIFT press + directional intent (single press)
	// - uses IsKeyPressed so holding shift won't keep dashing
	// - respects cooldown and won't allow repeated dashes
	// - do NOT allow dash while jumping
	// ==============================
	bool shiftPressed = IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT);
	if (shiftPressed && !is_dashing && dash_cooldown_timer == 0 && !is_jumping)
	{
		// require directional input or use facing direction as fallback
		int dir = 0;
		if (wantRight)
			dir = 1;
		else if (wantLeft)
			dir = -1;
		else
			dir = is_facing_right ? 1 : -1;

		// start dash
		is_dashing = true;
		dash_frames_remaining = dash_frames_total;
		velocity_x = dir * dash_speed;

		// make dash feel like a quick run visually
		current_state = STATE_RUN;

		// reset run animation so fast traversal looks immediate
		run_anim.curr_frame = 0;
		run_anim.frame_counter = 0;
		run_anim.frame_rec.x = 0.f;
	}

	// ==============================
	// 1. HORIZONTAL MOVEMENT (normal, only when not dashing)
	// ==============================
	bool moving = false;
	if (!is_dashing)
	{
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
	}
	else
	{
		// Apply dash movement directly (overrides normal handling)
		position.x += velocity_x;
	}

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
			// if we collided while dashing, cancel dash
			if (is_dashing)
			{
				is_dashing = false;
				dash_frames_remaining = 0;
				dash_cooldown_timer = dash_cooldown_frames;
			}
		}
	}
	else if (velocity_x < 0) // Left
	{
		if (map.get_tile_id(leftTile, topTile) > 0 || map.get_tile_id(leftTile, bottomTile) > 0)
		{
			position.x = ((leftTile + 1) * tileSize) - offX + 0.1f;
			velocity_x = 0;
			// if we collided while dashing, cancel dash
			if (is_dashing)
			{
				is_dashing = false;
				dash_frames_remaining = 0;
				dash_cooldown_timer = dash_cooldown_frames;
			}
		}
	}

	// If we're dashing, count down frames and finish dash when done
	if (is_dashing)
	{
		if (dash_frames_remaining > 0)
			--dash_frames_remaining;

		// end dash if finished
		if (dash_frames_remaining <= 0)
		{
			is_dashing = false;
			// stop horizontal movement after dash
			velocity_x = 0.0f;
			// start cooldown
			dash_cooldown_timer = dash_cooldown_frames;
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
	bool hitMapFloor
		= (map.get_tile_id(leftTile, bottomTile) > 0 || map.get_tile_id(rightTile, bottomTile) > 0);
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
		attack_hit_enemies.clear();	   // clear per-attack hit list
		attack_hit_registered = false; // legacy flag
		attack_anim.curr_frame = 0;
		attack_anim.frame_counter = 0;
	}

	// --- 5. ANIMATION STATE ---
	// Do not override hurt state if it is active.
	if (!is_jumping && !is_attacking && current_state != STATE_HURT)
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
				attack_hit_enemies.clear();
			}
			else
			{
				attack_anim.frame_rec.x = (float)attack_anim.curr_frame * attack_anim.frame_width;
				attack_anim.frame_rec.width = attack_anim.frame_width; // ensure stable width
			}
		}

		// --- LOOSENED HIT: any enemy inside effective range at ANY frame during attack gets hit
		// (once) ---
		{
			const float baseRange = PLAYER_ATTACK_RANGE * (scale / 2.5f);
			const float speedBonus
				= std::fabs(velocity_x) * 6.0f + (is_dashing ? dash_speed * 0.9f : 0.0f);
			const float effectiveRange = baseRange + speedBonus;
			const int dmg = PLAYER_ATTACK_DAMAGE;

			for (Enemy *e : Enemy::all())
			{
				if (e == nullptr || !e->is_alive())
					continue;

				// skip if already hit this attack
				if (std::find(attack_hit_enemies.begin(), attack_hit_enemies.end(), e)
					!= attack_hit_enemies.end())
					continue;

				Vector2 myPos = get_position();
				Vector2 enemyPos = e->get_position();
				float dx = enemyPos.x - myPos.x;
				float dy = enemyPos.y - myPos.y;
				float dist = std::sqrt(dx * dx + dy * dy);

				// require enemy roughly in front for consistent hits, unless moving fast
				bool inFront = (dx >= 0 && is_facing_right) || (dx <= 0 && !is_facing_right);
				bool allowHitWhileMovingFast = (std::fabs(velocity_x) > 8.0f) || is_dashing;

				if (dist <= effectiveRange && (inFront || allowHitWhileMovingFast))
				{
					e->take_damage(dmg);

					// If enemy died from this hit, heal the player by 20 hp (cap enforced in
					// Character::heal)
					if (!e->is_alive())
					{
						this->heal(20);
					}

					// record this enemy as hit for this attack animation
					attack_hit_enemies.push_back(e);
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
		// When dashing, play run animation faster to convey speed
		int run_anim_rate = is_dashing ? (FRAME_RATE * 3) : FRAME_RATE;
		run_anim.frame_counter++;
		if (run_anim.frame_counter >= (60 / run_anim_rate))
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

	// Let Character handle hurt/dead progression (knockback, hurt->dead transition, dead anim)
	process_state();
}
