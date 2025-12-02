#include <enemy.hpp>
#include <resources.hpp>

class KarasuTengu : public ShortRangeEnemy
{
  public:
	KarasuTengu(Vector2 pos) : ShortRangeEnemy(pos)
	{
		set_scale(1.0f);
		this->set_attack_damage(15);
		this->set_chase_speed(5.0f);
	}
	void init() override
	{
		using namespace Resources::KarasuTenguResource;

		load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
		load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
		load_texture(STATE_HURT, HURT_TEXTURE.c_str(), HURT_FRAMES);
		load_texture(STATE_DEAD, DEAD_TEXTURE.c_str(), DEAD_FRAMES);
	}
};
