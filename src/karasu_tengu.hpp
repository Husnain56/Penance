#include "enemy.hpp"
#include "resources.hpp"

class KarasuTengu : public ShortRangeEnemy
{
  public:
	KarasuTengu(Vector2 pos) : ShortRangeEnemy(pos) 
	{
		set_scale(1.0f);
		this->set_attack_damage(20);
		this->set_chase_speed(5.0f);
		this->set_attack_range(80.0f);

	}
	void init() override
	{
		using namespace Resources::KarasuTenguResource;
	
		load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
		load_texture(STATE_IDLE, IDLE_TEXTURE.c_str(), IDLE_FRAMES);
		load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
		load_texture(STATE_JUMP, JUMP_TEXTURE.c_str(), JUMP_FRAMES);
	}
};