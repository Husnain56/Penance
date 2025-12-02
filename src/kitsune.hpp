#pragma once

#include <enemy.hpp>
#include <resources.hpp>

class Kitsune : public LongRangeEnemy
{
public:
	Kitsune(Vector2 pos) : LongRangeEnemy(pos) { scale = 1.2f; }

	void init() override
	{
		using namespace Resources::KitsuneResource;

		load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
		load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
		load_texture(STATE_HURT, HURT_TEXTURE.c_str(), HURT_FRAMES);
		load_texture(STATE_DEAD, DEAD_TEXTURE.c_str(), DEAD_FRAMES);
	}
};