#pragma once

#include "character.hpp"

class Enemy : public Character
{
  public:
	Enemy(Vector2 pos) : Character(pos) {}

	void update(const Map &map) override;
};
