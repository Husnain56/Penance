#include <enemy.hpp>
#include <resources.hpp>

class BlueSamurai : public LongRangeEnemy
{
  public:
	BlueSamurai(Vector2 pos) : LongRangeEnemy(pos)
	{
		this->draw_offset = {0.0f, -20.0f};
		this->aggro_range = 1300;
		this->attack_range = 1000;
		this->on_hurt(200.0f);
	}
	void init() override
	{
		using namespace Resources::BlueSamuraiResource;

		load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
		load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
		load_texture(STATE_HURT, HURT_TEXTURE.c_str(), HURT_FRAMES);
		load_texture(STATE_DEAD, DEAD_TEXTURE.c_str(), DEAD_FRAMES);
	}
};
