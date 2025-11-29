#include "enemy.hpp"
#include "resources.hpp"

class SilverKnight : public ShortRangeEnemy {
public:
	SilverKnight(Vector2 pos) : ShortRangeEnemy(pos)
	{
		set_scale(1.5f);
		this->set_attack_damage(15);
		this->set_chase_speed(10.0f);
		this->draw_offset = { 0.0f, -80.0f };
	}

	void init() override
	{
		using namespace Resources::SilverKnightResource;

		load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
		load_texture(STATE_IDLE, IDLE_TEXTURE.c_str(), IDLE_FRAMES);
		load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
		load_texture(STATE_JUMP, JUMP_TEXTURE.c_str(), JUMP_FRAMES);
	}
};
