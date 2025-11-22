#include "player.hpp"
#include "constants.hpp"

void Player::update()
{
	using namespace GameConstants;

	if (IsKeyDown(KEY_D))
	{

		if (is_attacking)
		{
			attack_anim.frame_counter = 0;
			attack_anim.curr_frame = 0;
			attack_anim.frame_rec.x = 0.f;
			is_attacking = false;
		}

		current_state = STATE_RUN;
		position.x += MOVEMENT_SPEED;

		run_anim.frame_counter++;
		if (run_anim.frame_counter >= (60 / FRAME_RATE))
		{
			run_anim.frame_counter = 0;
			run_anim.curr_frame = (run_anim.curr_frame + 1) % run_anim.total_frame;
			is_facing_right = true;
			run_anim.frame_rec.x = (float)run_anim.curr_frame * run_anim.frame_width;
		}
	}
	else if (IsKeyDown(KEY_A))
	{
		if (is_attacking)
		{
			attack_anim.frame_counter = 0;
			attack_anim.curr_frame = 0;
			attack_anim.frame_rec.x = 0.f;
			is_attacking = false;
		}

		current_state = STATE_RUN;
		position.x -= MOVEMENT_SPEED;
		run_anim.frame_counter++;

		if (run_anim.frame_counter >= (60 / FRAME_RATE))
		{
			run_anim.frame_counter = 0;
			run_anim.curr_frame = (run_anim.curr_frame + 1) % run_anim.total_frame;
			is_facing_right = false;
			run_anim.frame_rec.x = (float)run_anim.curr_frame * run_anim.frame_width;
		}
	}
	else if (IsKeyPressed(KEY_E) && !is_attacking)
	{
		current_state = STATE_ATTACK;
		is_attacking = true;
		attack_anim.frame_counter = 0;
		attack_anim.curr_frame = 0;
		attack_anim.frame_rec.x = 0.f;
	}
	else if (IsKeyPressed(KEY_SPACE) && !is_jumping)
	{
		is_jumping = true;
		current_state = STATE_JUMP;
		jump_anim.curr_frame = 0;
		jump_anim.frame_counter = 0;
		jump_anim.frame_rec.x = 0.f;
	}
	else if (!is_attacking && !is_jumping)
	{
		current_state = STATE_IDLE;
		idle_anim.frame_counter++;
		if (idle_anim.frame_counter >= (60 / FRAME_RATE))
		{
			idle_anim.frame_counter = 0;
			idle_anim.curr_frame = (idle_anim.curr_frame + 1) % idle_anim.total_frame;
			idle_anim.frame_rec.x = (float)idle_anim.curr_frame * idle_anim.frame_width;
		}
	}

	if (is_attacking)
	{
		attack_anim.frame_counter++;
		if (is_jumping)
		{
			attack_anim.frame_counter = 0;
			attack_anim.curr_frame = 0;
			attack_anim.frame_rec.x = 0.f;
			is_attacking = false;
		}

		if (attack_anim.frame_counter >= (60 / FRAME_RATE))
		{
			attack_anim.frame_counter = 0;
			attack_anim.curr_frame++;

			if (attack_anim.curr_frame >= attack_anim.total_frame)
			{
				is_attacking = false;
				current_state = STATE_IDLE;
				attack_anim.curr_frame = 0;
			}
			else
			{
				attack_anim.frame_rec.x = (float)attack_anim.curr_frame * attack_anim.frame_width;
			}
		}
	}
	else if (is_jumping)
	{
		jump_anim.frame_counter++;
		if (is_attacking)
		{
			attack_anim.frame_counter = 0;
			attack_anim.curr_frame = 0;
			attack_anim.frame_rec.x = 0.f;
			is_attacking = false;
		}

		if (jump_anim.frame_counter >= (60 / FRAME_RATE))
		{
			jump_anim.frame_counter = 0;
			jump_anim.curr_frame++;

			if (jump_anim.curr_frame > 5)
				position.y += JUMP_SPEED;
			else if (jump_anim.curr_frame < 5)
				position.y -= JUMP_SPEED;

			if (jump_anim.curr_frame >= jump_anim.total_frame)
			{
				is_jumping = false;
				current_state = STATE_IDLE;
				jump_anim.curr_frame = 0;
			}
			else
				jump_anim.frame_rec.x = (float)jump_anim.curr_frame * jump_anim.frame_width;
		}
	}
}
