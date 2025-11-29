#pragma once
#include "character.hpp"

class Player : public Character
{
  private:
	float velocity_y;
	float velocity_x;

	float accel;
	float decel;
	float max_speed;

	int jump_count;

	// bookkeeping for player's attack (single hit per attack animation)
	bool attack_hit_registered;

  public:
	Player(Vector2 pos);

	void update(const Map &map) override;
	void init() override;
};
