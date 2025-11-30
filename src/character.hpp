#pragma once

#include <map.hpp>
#include <types.hpp>

class Character
{
  protected:
	Texture2D run_texture, idle_texture, attack_texture, jump_texture, hurt_texture, dead_texture;
	Vector2 position;
	Vector2 draw_offset;
	float scale;

	bool is_facing_right;
	bool is_attacking;
	bool is_jumping;

	CharacterState current_state;
	Animation run_anim, idle_anim, attack_anim, jump_anim, hurt_anim, dead_anim;

	// Health
	int hp;
	int max_hp;

	// Hurt / Dead
	bool pending_removal;
	bool dead_anim_playing;
	float knockback_vel_x;
	int hurt_frames_remaining;
	int hurt_frames_total;

  public:
	Character(Vector2 pos);

	virtual ~Character();

	void draw();

	void load_texture(CharacterState texture_type, const char *filename, int total_frames);

	virtual void update(const Map &map) = 0;

	virtual void init() = 0;

	Vector2 get_position() const { return position; }

	void set_scale(float new_scale) { scale = new_scale; }

	// Health API
	int get_hp() const { return hp; }
	int get_max_hp() const { return max_hp; }
	bool is_alive();
	void take_damage(int amount);
	void heal(int amount);

	// Called by take_damage to apply knockback / hurt-specific behaviour.
	virtual void on_hurt(float strength);

	void process_state();

	bool is_removed() const { return pending_removal; }

	bool is_dead_anim_playing() const { return dead_anim_playing; }
};
