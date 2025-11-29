#pragma once

#include "character.hpp"

class Player; // forward

class Enemy : public Character
{
  public:
	Enemy(Vector2 pos) : Character(pos), target_player(nullptr), attack_hit_registered(false) {}

	void update(const Map &map) override;

	// Link to player for chasing/attacking
	void set_target_player(Player *p) { target_player = p; }

  private:
	Player *target_player;

	// Attack bookkeeping
	bool attack_hit_registered;
};
