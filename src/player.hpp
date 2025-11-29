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

	// Dash state
	bool is_dashing;
	int dash_frames_remaining;    // frames left in current dash
	int dash_frames_total;        // frames per dash
	int dash_cooldown_frames;     // frames between dashes
	int dash_cooldown_timer;      // current cooldown counter (frames)
	float dash_speed;             // pixels per frame during dash

  public:
	Player(Vector2 pos);

	void update(const Map &map) override;
	void init() override;
};
