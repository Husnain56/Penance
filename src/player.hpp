#pragma once
#include "character.hpp"

class Player : public Character
{
  private:
	float velocity_y = 0.0f;

  public:
	Player(Vector2 pos);

	void update() override;
	void init() override;
};
