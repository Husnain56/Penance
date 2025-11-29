#pragma once

#include "character.hpp"
#include "constants.hpp"
#include "player.hpp"
#include <vector>
#include <algorithm>

class Player; // forward
using namespace GameConstants;

class Enemy : public Character
{
  public:
	// High level AI states - easy to extend / reason about in children
	enum class AIState
	{
		ROAM,
		CHASE,
		ATTACK
	};

	// Register enemy on construction so other systems (player, AI) can find all enemies
	Enemy(Vector2 pos)
		: Character(pos),
		  target_player(nullptr),
		  attack_hit_registered(false),
		  velocity_y(0.0f),
		  ai_state(AIState::ROAM),
		  patrol_speed(MOVEMENT_SPEED),
		  chase_speed(2.8f),
		  aggro_range(480.0f),
		  attack_range(64.0f),
		  attack_damage(15)
	{
		s_enemies.push_back(this);
	}

	// Unregister on destruction
	virtual ~Enemy()
	{
		auto it = std::find(s_enemies.begin(), s_enemies.end(), this);
		if (it != s_enemies.end())
			s_enemies.erase(it);
	}

	void update(const Map &map) override;

	// Link to player for chasing/attacking (optional)
	void set_target_player(Player *p) { target_player = p; }

	// Access all enemies (useful for player attacks / inter-enemy checks)
	static const std::vector<Enemy *> &all() { return s_enemies; }

	// --- Protected hooks so child classes can override only behavior they need ---
  protected:
	// Called when deciding ROAM behaviour. Default: simple left/right patrol.
	// Child classes can override to change roaming movement (e.g., idle, patrol area etc.)
	virtual void on_roam(const Map &map, float &outMoveX)
	{
		outMoveX = (is_facing_right ? patrol_speed : -patrol_speed);
	}

	// Called when deciding CHASE behaviour. Default: move directly on X toward player.
	virtual void on_chase(Player *player, float &outMoveX)
	{
		if (player == nullptr)
		{
			outMoveX = 0.0f;
			return;
		}
		float dx = player->get_position().x - position.x;
		outMoveX = (dx > 0.0f) ? chase_speed : -chase_speed;
	}

	// Called when entering ATTACK behaviour. Default: stop and play attack animation.
	// Child classes can override for ranged attacks or special telegraphs.
	virtual void on_attack(Player *player)
	{
		(void)player;
		// default does nothing special; update() will handle animations / hits
	}

	// Tuning parameters for AI (protected so child classes can tweak in ctor)
	float patrol_speed;
	float chase_speed;
	float aggro_range;
	float attack_range;
	int attack_damage;

  private:
	Player *target_player;

	// Attack bookkeeping
	bool attack_hit_registered;

	// Vertical velocity for simple physics (gravity/fall)
	float velocity_y;

	// Current AI state
	AIState ai_state;

	// Central registry of enemies
	static std::vector<Enemy *> s_enemies;
};
