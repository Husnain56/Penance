#pragma once

#include "types.hpp"

class Character
{
  protected:
	Texture2D run_texture, idle_texture, attack_texture, jump_texture;
	Vector2 position;

	bool is_facing_right;
	bool is_attacking;
	bool is_jumping;

	CharacterState current_state;
	Animation run_anim, idle_anim, attack_anim, jump_anim;

  public:
	Character();

	Character(Vector2 pos);

	virtual ~Character();

	void draw();
	// used to draw the character

	void load_texture(CharacterState texture_type, const char *filename, int total_frames);
	// input:
	//    texture type: (run, idle, attack, jump)
	//    file name: path to the texture
	//    total frames: number of frames texture contains

	virtual void update() = 0;
	// update deals with the keyboard IO and player movement
	//
	// keyboard IO:
	//     A = move left
	//     D = move right
	//     E = attack
	//     Space = jump

	virtual void init() = 0;
	// init initalises the textures for every character

	Vector2 get_position() const { return position; }
	// getter to help main.cpp if needed
};
