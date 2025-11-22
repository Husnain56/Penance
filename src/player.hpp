#pragma once
#include "character.hpp"

class Player : public Character
{
  public:
	Player(Vector2 pos) : Character(pos) {}

	void update() override;
};
