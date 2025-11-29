#include "enemy.hpp"
#include "constants.hpp"

void Enemy::update(const Map &map)
{
	using namespace GameConstants;

	current_state = STATE_RUN;

	if (is_facing_right)
	{
		position.x += MOVEMENT_SPEED;

		if (position.x >= GetScreenWidth() - run_anim.frame_width)
		{
			is_facing_right = false;
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
