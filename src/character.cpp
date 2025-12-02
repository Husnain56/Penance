#include <algorithm>
#include <character.hpp>
#include <iostream>
#include <raylib.h>
#include <constants.hpp>

Character::Character(Vector2 pos)
{
	position = pos;
	scale = 1.0f;
	is_attacking = false;
	is_facing_right = true;
	is_jumping = false;
	current_state = STATE_IDLE;
	max_hp = 100;
	hp = max_hp;
	draw_offset = { 0, 0 };

	Animation emptyAnim = { 0, 0, 1, 0.0f, {0, 0, 0, 0} };
	run_anim = emptyAnim;
	idle_anim = emptyAnim;
	attack_anim = emptyAnim;
	jump_anim = emptyAnim;
	hurt_anim = emptyAnim;
	dead_anim = emptyAnim;

	// hurt/dead defaults
	pending_removal = false;
	dead_anim_playing = false;
	knockback_vel_x = 0.0f;
	hurt_frames_remaining = 0;
	// Default hurt duration in frames (will be used if hurt_anim info isn't set)
	hurt_frames_total = 12;
}

Character::~Character()
{
	UnloadTexture(run_texture);
	UnloadTexture(idle_texture);
	UnloadTexture(attack_texture);
	UnloadTexture(jump_texture);
	UnloadTexture(hurt_texture);
	UnloadTexture(dead_texture);
}

void Character::load_texture(CharacterState texture_type, const char* filename, int total_frames)
{
	Texture loaded_texture = LoadTexture(filename);

	SetTextureFilter(loaded_texture, TEXTURE_FILTER_POINT);

	if (loaded_texture.id == 0)
	{
		std::cout << "Failed to load: " << filename << std::endl;
		return;
	}

	float frame_width = (float)loaded_texture.width / total_frames;
	Rectangle frame_rec = { 0, 0, frame_width, (float)loaded_texture.height };
	Animation anim = { 0, 0, total_frames, frame_width, frame_rec };

	switch (texture_type)
	{
	case STATE_IDLE:
		idle_texture = loaded_texture;
		idle_anim = anim;
		break;
	case STATE_RUN:
		run_texture = loaded_texture;
		run_anim = anim;
		break;
	case STATE_ATTACK:
		attack_texture = loaded_texture;
		attack_anim = anim;
		break;
	case STATE_JUMP:
		jump_texture = loaded_texture;
		jump_anim = anim;
		break;
	case STATE_HURT:
		hurt_texture = loaded_texture;
		hurt_anim = anim;
		break;
	case STATE_DEAD:
		dead_texture = loaded_texture;
		dead_anim = anim;
		break;
	default:
		break;
	}
}

void Character::take_damage(int amount)
{
	if (amount <= 0)
		return;

	hp = std::max(0, hp - amount);

	this->current_state = STATE_HURT;
	is_attacking = false;

	hurt_anim.curr_frame = 0;
	hurt_anim.frame_counter = 0;
	hurt_anim.frame_rec.x = 0.0f;

	const float default_knockback = 8.0f;
	this->on_hurt(default_knockback);
}

void Character::heal(int amount)
{
	if (amount <= 0)
		return;

	hp = std::min(max_hp, hp + amount);
}

void Character::draw()
{
	if (pending_removal)
		return;

	Texture2D texture = { 0 };
	Rectangle frame_rec = { 0, 0, 0, 0 };

	if (current_state == STATE_DEAD)
	{
		texture = dead_texture.id != 0 ? dead_texture : idle_texture;
		frame_rec = (dead_anim.total_frame > 0) ? dead_anim.frame_rec : idle_anim.frame_rec;
	}
	else if (current_state == STATE_HURT)
	{
		texture = hurt_texture.id != 0 ? hurt_texture : idle_texture;
		frame_rec = (hurt_anim.total_frame > 0) ? hurt_anim.frame_rec : idle_anim.frame_rec;
	}
	else if (is_attacking)
	{
		texture = attack_texture;
		frame_rec = attack_anim.frame_rec;
	}
	else if (is_jumping)
	{
		texture = jump_texture;
		frame_rec = jump_anim.frame_rec;
	}
	else if (current_state == STATE_RUN)
	{
		texture = run_texture;
		frame_rec = run_anim.frame_rec;
	}
	else
	{
		texture = idle_texture;
		frame_rec = idle_anim.frame_rec;
	}

	if (texture.id != 0)
	{
		Rectangle source = frame_rec;
		if (!is_facing_right)
			source.width = -source.width;

		Rectangle dest = { position.x + draw_offset.x, position.y + draw_offset.y,
						  frame_rec.width * scale, frame_rec.height * scale };
		Vector2 origin = { 0, 0 };

		DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);

		float barWidth = dest.width;
		float barHeight = 6.0f;
		float hpRatio = (max_hp > 0) ? (float)hp / (float)max_hp : 0.0f;
		Rectangle backBar = { dest.x, dest.y - (barHeight + 6.0f), barWidth, barHeight };
		Rectangle fgBar
			= { backBar.x + 1.0f, backBar.y + 1.0f, (barWidth - 2.0f) * hpRatio, barHeight - 2.0f };

		DrawRectangleRec(backBar, Fade(BLACK, 0.6f));
		DrawRectangleRec(fgBar, (hpRatio > 0.5f) ? GREEN : (hpRatio > 0.2f ? YELLOW : RED));
	}
}

bool Character::is_alive()
{
	return hp > 0;
}

void Character::on_hurt(float strength)
{
	knockback_vel_x = is_facing_right ? -strength : strength;

	if (hurt_anim.total_frame > 0)
		hurt_frames_total = hurt_anim.total_frame * (60 / GameConstants::FRAME_RATE);
	else
		hurt_frames_total = 12;

	hurt_frames_remaining = hurt_frames_total;
}

void Character::process_state()
{
	using namespace GameConstants;

	if (pending_removal)
		return;

	if (current_state == STATE_HURT)
	{
		if (hurt_anim.total_frame > 0)
		{
			hurt_anim.frame_counter++;
			if (hurt_anim.frame_counter >= (60 / FRAME_RATE))
			{
				hurt_anim.frame_counter = 0;
				if (hurt_anim.curr_frame < hurt_anim.total_frame - 1)
					hurt_anim.curr_frame++;
				hurt_anim.frame_rec.x = (float)hurt_anim.curr_frame * hurt_anim.frame_width;
			}
		}

		position.x += knockback_vel_x;

		knockback_vel_x *= 0.90f;

		if (hurt_frames_remaining > 0)
			--hurt_frames_remaining;

		if (hurt_frames_remaining <= 0)
		{
			if (hp <= 0)
			{
				current_state = STATE_DEAD;
				dead_anim.curr_frame = 0;
				dead_anim.frame_counter = 0;
				dead_anim.frame_rec.x = 0.0f;
				dead_anim_playing = true;
			}
			else
			{
				current_state = STATE_IDLE;
			}
		}

		return;
	}

	if (current_state == STATE_DEAD)
	{
		if (dead_anim.total_frame > 0 && dead_anim_playing)
		{
			dead_anim.frame_counter++;
			if (dead_anim.frame_counter >= (60 / FRAME_RATE))
			{
				dead_anim.frame_counter = 0;
				dead_anim.curr_frame++;
				if (dead_anim.curr_frame >= dead_anim.total_frame)
				{
					dead_anim_playing = false;
					pending_removal = true;
				}
				else
				{
					dead_anim.frame_rec.x = (float)dead_anim.curr_frame * dead_anim.frame_width;
				}
			}
		}
		else
		{
			pending_removal = true;
		}
	}
}