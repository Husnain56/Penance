#include "raylib.h"
#include <iostream>

using std::string;
using std::cout;

struct Animation
{
  int curr_frame;
  int frame_counter;
  int total_frame;
  int frame_width;
  Rectangle frame_rec;  
};

namespace GameConstants
{
  constexpr float MOVEMENT_SPEED = 4.0f;
  constexpr float JUMP_SPEED = 12.0f;
  constexpr int FRAME_RATE = 3;
  constexpr int ACTION_FRAME_RATE = 4;
};

enum CharacterState {STATE_RUN, STATE_IDLE, STATE_ATTACK, STATE_JUMP};

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

    void load_texture (string texture_type, const char* filename, int total_frames)
    {
      if (texture_type == "run")
      {
        run_texture = LoadTexture(filename);
        run_anim = {0, 0, total_frames, run_texture.width/total_frames, {0, 0 , (float)run_texture.width/total_frames, (float)run_texture.height}};       
      }
      else if (texture_type == "idle")
      {
        idle_texture = LoadTexture(filename);
        idle_anim = {0, 0, total_frames, idle_texture.width/total_frames, {0, 0, (float)idle_texture.width/total_frames, (float)idle_texture.height}};       
      }
      else if (texture_type == "attack")
      {
        attack_texture = LoadTexture(filename);
        attack_anim = {0, 0, total_frames, attack_texture.width/total_frames, {0, 0, (float)attack_texture.width/total_frames, (float)attack_texture.height}};        
      }
      else if (texture_type == "jump")
      {
        jump_texture = LoadTexture(filename);
        jump_anim = {0, 0, total_frames, jump_texture.width/total_frames, {0, 0, (float)jump_texture.width/total_frames, (float)jump_texture.height}};
        
      }
      else
        cout << "Texture type not found\n";
    }

    ~Character()
    {
      UnloadTexture(run_texture);  
      UnloadTexture(idle_texture);  
      UnloadTexture(attack_texture);  
      UnloadTexture(jump_texture);  
    }

    void draw()
    {
      Texture2D texture;
      Rectangle frame_rec;

      if (is_attacking)
      {
        texture = attack_texture;
        frame_rec= attack_anim.frame_rec;
      }
      else if (is_jumping)
      {
        texture = jump_texture;
        frame_rec = jump_anim.frame_rec;
      }
      else if (current_state == STATE_RUN)
      {
        texture = run_texture;
        frame_rec = run_anim.frame_rec;
      }

      else if (current_state == STATE_IDLE)
      {
        texture = idle_texture;
        frame_rec = idle_anim.frame_rec;
      }

      if (is_facing_right)
      {
        DrawTextureRec(texture, frame_rec, position, WHITE);
      }
      else
      {
        Rectangle flipped_frame = {frame_rec.x, frame_rec.y, -frame_rec.width, frame_rec.height};
        DrawTextureRec(texture, flipped_frame, position, WHITE);
      }

    }

    void update()
    {
      using namespace GameConstants;

      if (IsKeyDown(KEY_D))
      {

        if (is_attacking)
        {      
          attack_anim.frame_counter = 0;
          attack_anim.curr_frame = 0;
          attack_anim.frame_rec.x = 0;
          is_attacking = false;
        }

        current_state = STATE_RUN;
        position.x += MOVEMENT_SPEED;

        run_anim.frame_counter++;
        if (run_anim.frame_counter >= (60 >> FRAME_RATE))
        {
          run_anim.frame_counter = 0;
          run_anim.curr_frame = (run_anim.curr_frame + 1) % run_anim.total_frame;
          is_facing_right = true;
          run_anim.frame_rec.x = (float)run_anim.curr_frame * run_anim.frame_width;
        }
      }
      else if (IsKeyDown(KEY_A))
      {
        if (is_attacking)
        {       
          attack_anim.frame_counter = 0;
          attack_anim.curr_frame = 0;
          attack_anim.frame_rec.x = 0;
          is_attacking = false;
        }

        current_state = STATE_RUN;
        position.x -= MOVEMENT_SPEED;
        run_anim.frame_counter++;

        if (run_anim.frame_counter >= (60 >> FRAME_RATE))
        {
          run_anim.frame_counter = 0;
          run_anim.curr_frame = (run_anim.curr_frame + 1) % run_anim.total_frame;
          is_facing_right = false;
          run_anim.frame_rec.x = (float)run_anim.curr_frame * run_anim.frame_width;
        }
      }
      else if (IsKeyPressed(KEY_E) && !is_attacking)
      {
        current_state = STATE_ATTACK;
        is_attacking = true;
        attack_anim.frame_counter = 0;
        attack_anim.curr_frame = 0;
        attack_anim.frame_rec.x = 0;
      }
      else if (IsKeyPressed(KEY_SPACE) && !is_jumping)
      {
        is_jumping = true;
        current_state = STATE_JUMP;
        jump_anim.curr_frame = 0;
        jump_anim.frame_counter = 0;
        jump_anim.frame_rec.x = 0;    
      }
      else if (!is_attacking && !is_jumping)
      {
        current_state = STATE_IDLE;
        idle_anim.frame_counter++;
        if (idle_anim.frame_counter >= (60 >> FRAME_RATE))
        {
          idle_anim.frame_counter = 0;
          idle_anim.curr_frame = (idle_anim.curr_frame + 1) % idle_anim.total_frame;
          idle_anim.frame_rec.x = (float)idle_anim.curr_frame * idle_anim.frame_width;
        }
      }

      if (is_attacking)
      {
        attack_anim.frame_counter++;
        if (is_jumping)
        {       
          attack_anim.frame_counter = 0;
          attack_anim.curr_frame = 0;
          attack_anim.frame_rec.x = 0;
          is_attacking = false;
        }

        if (attack_anim.frame_counter >= (60 >> ACTION_FRAME_RATE))
        {
          attack_anim.frame_counter = 0;
          attack_anim.curr_frame++;

          if (attack_anim.curr_frame >= attack_anim.total_frame)
          {
            is_attacking = false;
            current_state = STATE_IDLE;
            attack_anim.curr_frame = 0;
          }
          else
          {
            attack_anim.frame_rec.x = (float)attack_anim.curr_frame * attack_anim.frame_width;
          }
        }
      }
      else if (is_jumping)
      {
        jump_anim.frame_counter++;
        if (is_attacking)
        {       
          attack_anim.frame_counter = 0;
          attack_anim.curr_frame = 0;
          attack_anim.frame_rec.x = 0;
          is_attacking = false;
        }

        if (jump_anim.frame_counter >= (60 >> FRAME_RATE))
        {
          jump_anim.frame_counter = 0;
          jump_anim.curr_frame++;

          if (jump_anim.curr_frame > 5)
            position.y += JUMP_SPEED;
          else if (jump_anim.curr_frame < 5)
            position.y -= JUMP_SPEED;       

          if (jump_anim.curr_frame >= jump_anim.total_frame)
          {
            is_jumping = false;
            current_state = STATE_IDLE;
            jump_anim.curr_frame = 0;
          }
          else
            jump_anim.frame_rec.x = (float)jump_anim.curr_frame * jump_anim.frame_width;
        }
      }  
    }
};
