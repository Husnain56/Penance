#pragma once

#include <raylib.h>

struct Animation
{
	int curr_frame;
	int frame_counter;
	int total_frame;
	float frame_width;
	Rectangle frame_rec;
};

enum CharacterState
{
	STATE_RUN,
	STATE_IDLE,
	STATE_ATTACK,
	STATE_JUMP
};
