#include <enemy.hpp>
#include <resources.hpp>

class PurpleKnight : public ShortRangeEnemy
{
  public:
	PurpleKnight(Vector2 pos) : ShortRangeEnemy(pos)
	{
		set_scale(1.5f);
		this->set_attack_damage(7);
		this->set_chase_speed(2.5f);
		this->draw_offset = {0.0f, -80.0f};
	}

	void init() override
	{
		using namespace Resources::PurpleKnightResource;

		load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
		load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
		load_texture(STATE_HURT, HURT_TEXTURE.c_str(), HURT_FRAMES);
		load_texture(STATE_DEAD, DEAD_TEXTURE.c_str(), DEAD_FRAMES);
	}
};
