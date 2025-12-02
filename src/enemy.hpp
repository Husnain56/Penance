#pragma once

#include <algorithm>
#include <character.hpp>
#include <constants.hpp>
#include <player.hpp>
#include <vector>

class Player; // forward declaration
using namespace GameConstants;

class Enemy : public Character
{
public:
	enum class AIState
	{
		ROAM,
		CHASE,
		ATTACK
	};

	Enemy(Vector2 pos)
		: Character(pos), target_player(nullptr), attack_hit_registered(false), velocity_y(0.0f),
		  ai_state(AIState::ROAM), patrol_speed(MOVEMENT_SPEED)
	{
		s_enemies.push_back(this);
	}

	virtual ~Enemy()
	{
		auto it = std::find(s_enemies.begin(), s_enemies.end(), this);
		if (it != s_enemies.end())
			s_enemies.erase(it);
	}

	void update(const Map &map) override;

	void set_target_player(Player *p) { target_player = p; }

	static const std::vector<Enemy *> &all() { return s_enemies; }

  public:

	virtual void on_roam(const Map &map, float &outMoveX)
	{
		outMoveX = (is_facing_right ? patrol_speed : -patrol_speed);
	}

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

	virtual void on_attack(Player *player)
	{
		(void)player;
		// default does nothing special; update() will handle animations / hits
	}

	virtual void on_hurt(float strength);

	float patrol_speed;
	float chase_speed;
	float aggro_range;
	float attack_range;
	int attack_damage;

  private:
	Player *target_player;

	bool attack_hit_registered;

	float velocity_y;

	AIState ai_state;

	static std::vector<Enemy *> s_enemies;
};


class LongRangeEnemy : public Enemy
{
public:
	LongRangeEnemy(Vector2 pos) : Enemy(pos)
	{
		aggro_range = 1000.0f;
		attack_range = 200.0f;
		attack_damage = 10;
		patrol_speed = 1.5f;
		chase_speed = 2.0f;
	}

	void on_hurt(float strength) override;
};

class ShortRangeEnemy : public Enemy
{
public:
	ShortRangeEnemy(Vector2 pos) : Enemy(pos)
	{
		aggro_range = 1000.0f;
		attack_range = 200.0f;
		attack_damage = 15;
		patrol_speed = 1.5f;
		chase_speed = 2.0f;
	}

	void on_hurt(float strength) override;
};

class MainVillain : public Enemy
{
};