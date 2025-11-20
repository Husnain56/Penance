#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "raylib.h"
#include <iostream>

using std::cout;
using std::string;

struct Animation
{
    int curr_frame;
    int frame_counter;
    int total_frame;
    float frame_width;
    Rectangle frame_rec;
};

namespace GameConstants
{
constexpr float MOVEMENT_SPEED = 4.0f;
constexpr float JUMP_SPEED = 12.0f;
constexpr int FRAME_RATE = 8;
constexpr int ACTION_FRAME_RATE = 4;
}; // namespace GameConstants

enum CharacterState
{
    STATE_RUN,
    STATE_IDLE,
    STATE_ATTACK,
    STATE_JUMP
};

class Character
{
  private:
    Texture2D run_texture, idle_texture, attack_texture, jump_texture;
    Vector2 position;
    bool is_facing_right = true;
    bool is_attacking = false;
    bool is_jumping = false;
    CharacterState current_state = STATE_IDLE;
    Animation run_anim, idle_anim, attack_anim, jump_anim;

  public:
    Character()
    {
        is_attacking = false;
        is_facing_right = true;
        is_jumping = false;
    }

    Character(Vector2 pos)
    {
        position = pos;
        is_attacking = false;
        is_facing_right = true;
        is_jumping = false;
    }

    ~Character()
    {
        UnloadTexture(run_texture);
        UnloadTexture(idle_texture);
        UnloadTexture(attack_texture);
        UnloadTexture(jump_texture);
    }

    void draw(void);
    // used to draw the character

    void load_texture(CharacterState texture_type, const char *filename, int total_frames);
    // input:
    //    texture type: (run, idle, attack, jump)
    //    file name: path to the texture
    //    total frames: number of frames texture contains

    void update(void);
    // update deals with the keyboard IO and player movement
    //
    // keyboard IO:
    //     A = move left
    //     D = move right
    //     E = attack
    //     Space = jump
};
#endif
