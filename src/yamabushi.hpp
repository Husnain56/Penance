#include "enemy.hpp"
#include "resources.hpp"

class Yamabushi : public ShortRangeEnemy {
	public:
	Yamabushi(Vector2 pos) : ShortRangeEnemy(pos) 
	{
		set_scale(1.0f);
		this->set_attack_damage(20);
		this->set_chase_speed(5.0f);
		this->set_attack_range(80.0f);
		this->set_aggro_range(200.0f);
		this->draw_offset = { 0.0f, 5.0f };
	}
	void init() override
	{
		using namespace Resources::YamaBushiResource;
	
		load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
		load_texture(STATE_IDLE, IDLE_TEXTURE.c_str(), IDLE_FRAMES);
		load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
	}
};