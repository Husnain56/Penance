#include <enemy.hpp>
#include <resources.hpp>

class SilverKnight : public ShortRangeEnemy
{
  public:
	SilverKnight(Vector2 pos) : ShortRangeEnemy(pos)
	{
		set_scale(1.5f);
		this->attack_damage = 5;
		this->chase_speed =	2.0f;
		this->draw_offset = {0.0f, -80.0f};
	}

	void init() override
	{
		using namespace Resources::SilverKnightResource;

		load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
		load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
		load_texture(STATE_HURT, HURT_TEXTURE.c_str(), HURT_FRAMES);
		load_texture(STATE_DEAD, DEAD_TEXTURE.c_str(), DEAD_FRAMES);
	}
};
