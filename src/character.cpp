#include "character.hpp"
#include <iostream>

using std::cout;
using std::endl;

Character::Character()
{
	is_attacking = false;
	is_facing_right = true;
	is_jumping = false;
	current_state = STATE_IDLE;
}

Character::Character(Vector2 pos)
{
	position = pos;
	is_attacking = false;
	is_facing_right = true;
	is_jumping = false;
	current_state = STATE_IDLE;
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
	if (loaded_texture.id == 0)
	{
		cout << "Fail to load texture: " << texture_type << " Path: " << filename << endl;
		return;
	}

	float frame_width = (float)loaded_texture.width / total_frames;
	Rectangle frame_rec = {0, 0, frame_width, (float)loaded_texture.height};

	switch (texture_type)
	{
	case CharacterState::STATE_IDLE:
		idle_texture = loaded_texture;
		idle_anim = {0, 0, total_frames, frame_width, frame_rec};
		break;

	case CharacterState::STATE_RUN:
		run_texture = loaded_texture;
		run_anim = {0, 0, total_frames, frame_width, frame_rec};
		break;

	case CharacterState::STATE_ATTACK:
		attack_texture = loaded_texture;
		attack_anim = {0, 0, total_frames, frame_width, frame_rec};
		break;

	case CharacterState::STATE_JUMP:
		jump_texture = loaded_texture;
		jump_anim = {0, 0, total_frames, frame_width, frame_rec};
		break;

	default:
		cout << "Sprite Type not identified\n";
		break;
	}
}

void Character::draw()
{
	Texture2D texture;
	Rectangle frame_rec;

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

	else if (current_state == STATE_IDLE)
	{
		texture = idle_texture;
		frame_rec = idle_anim.frame_rec;
	}

	if (is_facing_right)
	{
		DrawTextureRec(texture, frame_rec, position, WHITE);
	}
	else
	{
		Rectangle flipped_frame = {frame_rec.x, frame_rec.y, -frame_rec.width, frame_rec.height};
		DrawTextureRec(texture, flipped_frame, position, WHITE);
	}
}
