#include "enemy.hpp"
#include "constants.hpp"
#include <cmath>
#include "player.hpp" // needed for Player pointer methods

void Enemy::update(const Map &map)
{
	using namespace GameConstants;

	// AI parameters
	const float AGGRO_RANGE = 300.0f;
	const float ATTACK_RANGE = 60.0f;
	const int ENEMY_ATTACK_DAMAGE = 15;
	const float CHASE_SPEED = 2.5f;

	// Default to running animation
	current_state = STATE_RUN;

	// If we have a player target, implement chase/attack behavior
	if (target_player != nullptr)
	{
		Vector2 ppos = target_player->get_position();
		float dx = ppos.x - position.x;
		float dy = ppos.y - position.y;
		float dist = std::sqrt(dx * dx + dy * dy);

		// If far away: patrol behavior (keep slight movement)
		if (dist > AGGRO_RANGE)
		{
			// simple patrol along x similar to original implementation
			if (is_facing_right)
			{
				position.x += MOVEMENT_SPEED;
				if (position.x >= GetScreenWidth() - run_anim.frame_width)
					is_facing_right = false;
			}
			else
			{
				position.x -= MOVEMENT_SPEED;
				if (position.x <= 0)
					is_facing_right = true;
			}
		}
		else
		{
			// Aggro: chase player on X axis
			if (std::fabs(dx) > ATTACK_RANGE)
			{
				// move towards player
				if (dx > 0)
				{
					position.x += CHASE_SPEED;
					is_facing_right = true;
				}
				else
				{
					position.x -= CHASE_SPEED;
					is_facing_right = false;
				}

				// Use run state
				current_state = STATE_RUN;
				is_attacking = false;
				attack_hit_registered = false;
			}
			else
			{
				// Player is within attack range — perform attack
				if (!is_attacking)
				{
					is_attacking = true;
					current_state = STATE_ATTACK;
					attack_anim.curr_frame = 0;
					attack_anim.frame_counter = 0;
					attack_hit_registered = false;
				}
			}

			// If close enough while not attacking, optionally damage player (instant or via attack frame)
		}
	}
	else
	{
		position.x -= MOVEMENT_SPEED;

		if (position.x <= 0)
		{
			is_facing_right = true;
		}
	}

	run_anim.frame_counter++;

	int anim_speed = 60 / FRAME_RATE;

	if (run_anim.frame_counter >= anim_speed)
	{
		run_anim.frame_counter = 0;

		run_anim.curr_frame = (run_anim.curr_frame + 1) % run_anim.total_frame;

		run_anim.frame_rec.x = (float)run_anim.curr_frame * run_anim.frame_width;
	}
}
