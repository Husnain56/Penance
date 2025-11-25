#pragma once
#include "character.hpp"

class Player : public Character
{
  private:
	float velocity_y = 0.0f;
	float velocity_x = 0.0f;

	float accel = 0.4f;
	float decel = 0.85f;
	float max_speed = 6.0f;

  public:
	Player(Vector2 pos);

	void update() override;
	void init() override;
};
