#include <enemy.hpp>
#include <resources.hpp>

class SkeletonSpearman : public ShortRangeEnemy
{

  public:
	SkeletonSpearman(Vector2 pos) : ShortRangeEnemy(pos)
	{
		set_scale(1.5f);
		this->set_attack_damage(5);
		this->chase_speed = 15.0f;
		this->draw_offset = {0.0f, -80.0f};
		this->max_hp = 80;
	}
	void init() override
	{
		using namespace Resources::SkeletonSpearmanResource;

		load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
		load_texture(STATE_IDLE, IDLE_TEXTURE.c_str(), IDLE_FRAMES);
		load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
	}
};
