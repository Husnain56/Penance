#pragma once
#include "character.hpp"

class Enemy; // forward declaration

class Player : public Character
{
  private:
	float velocity_y;
	float velocity_x;

	float accel;
	float decel;
	float max_speed;

	int jump_count;

	// Pointer to an enemy so player attacks can affect it
	Enemy *target_enemy;
	bool attack_hit_registered;

  public:
	Player(Vector2 pos);

	void update(const Map &map) override;
	void init() override;

	// Link an enemy for interaction (nullable)
	void set_target_enemy(Enemy *e) { target_enemy = e; }
};
