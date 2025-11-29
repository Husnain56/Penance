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

  public:
	Player(Vector2 pos);

	void update(const Map &map) override;
	void init() override;
};
