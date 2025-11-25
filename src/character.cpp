#include "character.hpp"
#include <iostream>
#include <algorithm>
#include <raylib.h>

Character::Character(Vector2 pos)
{
	position = pos;
	scale = 1.0f;
	is_attacking = false;
	is_facing_right = true;
	is_jumping = false;
	current_state = STATE_IDLE;

	// default health; concrete types may override in their ctor
	max_hp = 100;
	hp = max_hp;
}

Character::~Character()
{
	UnloadTexture(run_texture);
	UnloadTexture(idle_texture);
	UnloadTexture(attack_texture);
	UnloadTexture(jump_texture);
}

void Character::load_texture(CharacterState texture_type, const char *filename, int total_frames)
{
	Texture loaded_texture = LoadTexture(filename);

	// Critical for pixel art scaling
	SetTextureFilter(loaded_texture, TEXTURE_FILTER_POINT);

	if (loaded_texture.id == 0)
	{
		std::cout << "Failed to load: " << filename << std::endl;
		return;
	}

	float frame_width = (float)loaded_texture.width / total_frames;
	Rectangle frame_rec = {0, 0, frame_width, (float)loaded_texture.height};
	Animation anim = {0, 0, total_frames, frame_width, frame_rec};

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
	default:
		break;
	}
}

void Character::take_damage(int amount)
{
	if (amount <= 0)
		return;

	hp = std::max(0, hp - amount);
	// optional: set hit reaction state here
}

void Character::heal(int amount)
{
	if (amount <= 0)
		return;

	hp = std::min(max_hp, hp + amount);
}

void Character::draw()
{
	Texture2D texture = {0};
	Rectangle frame_rec = {0, 0, 0, 0};

	if (is_attacking)
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

		Rectangle dest = {position.x, position.y, frame_rec.width * scale, frame_rec.height * scale};
		Vector2 origin = {0, 0};

		DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);

		// Draw a simple health bar above the sprite
		float barWidth = dest.width;
		float barHeight = 6.0f;
		float hpRatio = (max_hp > 0) ? (float)hp / (float)max_hp : 0.0f;
		Rectangle backBar = {dest.x, dest.y - (barHeight + 6.0f), barWidth, barHeight};
		Rectangle fgBar = {backBar.x + 1.0f, backBar.y + 1.0f, (barWidth - 2.0f) * hpRatio, barHeight - 2.0f};

		DrawRectangleRec(backBar, Fade(BLACK, 0.6f));
		DrawRectangleRec(fgBar, (hpRatio > 0.5f) ? GREEN : (hpRatio > 0.2f ? YELLOW : RED));
	}
}
