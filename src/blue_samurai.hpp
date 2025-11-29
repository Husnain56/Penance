#include "enemy.hpp"
#include "resources.hpp"

class BlueSamurai : public LongRangeEnemy {
public:
  BlueSamurai(Vector2 pos) : LongRangeEnemy(pos) 
  {
	  this->draw_offset = { 0.0f, -20.0f };
      this->set_attack_damage(15);
  }
  void init() override {
    using namespace Resources::BlueSamuraiResource;

    load_texture(STATE_RUN, RUN_TEXTURE.c_str(), RUN_FRAMES);
    load_texture(STATE_IDLE, IDLE_TEXTURE.c_str(), IDLE_FRAMES);
    load_texture(STATE_ATTACK, ATTACK_TEXTURE.c_str(), ATTACK_FRAMES);
    load_texture(STATE_JUMP, JUMP_TEXTURE.c_str(), JUMP_FRAMES);
  }
};
