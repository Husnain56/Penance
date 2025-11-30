#pragma once

#include <map.hpp>
#include <types.hpp>

class Character
{
  protected:
	// Textures and Position
	Texture2D run_texture, idle_texture, attack_texture, jump_texture;
	Vector2 position;
	Vector2 draw_offset;
	float scale;

	// Character States
	bool is_facing_right;
	bool is_attacking;
	bool is_jumping;

	// Current States and Animations
	CharacterState current_state;
	Animation run_anim, idle_anim, attack_anim, jump_anim;

	// Health
	int hp;
	int max_hp;

  public:
	Character(Vector2 pos);

	virtual ~Character();

	void draw();
	// used to draw the character

	void load_texture(CharacterState texture_type, const char *filename, int total_frames);
	// input:
	//    texture type: (run, idle, attack, jump)
	//    file name: path to the texture
	//    total frames: number of frames texture contains

	virtual void update(const Map &map) = 0;
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

	void set_scale(float new_scale) { scale = new_scale; }

	// Health API
	int get_hp() const { return hp; }
	int get_max_hp() const { return max_hp; }
	bool is_alive() const { return hp > 0; }
	void take_damage(int amount);
	void heal(int amount);
};
