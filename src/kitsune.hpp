#include "enemy.hpp"
#include "resources.hpp"

class Kitsune : public Enemy
{
  public:
	Kitsune(Vector2 pos) : Enemy(pos) {}

	void init() override
	{
		using namespace Resources::KitsuneResource;

		load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
		load_texture(STATE_IDLE, IDLE_TEXTURE.c_str(), IDLE_FRAMES);
		load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
		load_texture(STATE_JUMP, JUMP_TEXTURE.c_str(), JUMP_FRAMES);
	}
};
