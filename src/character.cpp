#include "character.hpp"
#include "raylib.h"
#include <iostream>

using std::cout;

void Character::load_texture (CharacterState texture_type, const char* filename, int total_frames)
{
  Texture loaded_texture = LoadTexture(filename);
  if(loaded_texture.id == 0){
    cout << "Fail to load texture: " << texture_type << " Path: " << filename << std::endl;
    return;
  }

  float frame_width = (float)loaded_texture.width/total_frames;
  Rectangle frame_rec = {0, 0, frame_width, (float)loaded_texture.height};
  
  switch (texture_type) {

    case CharacterState::STATE_IDLE:
      idle_texture = loaded_texture;
      idle_anim = {0, 0, total_frames, frame_width, frame_rec};
      break;
    
    case CharacterState::STATE_RUN:
      run_texture = loaded_texture;
      run_anim = {0, 0, total_frames, frame_width, frame_rec};
      break;
  
    case CharacterState::STATE_ATTACK:
      attack_texture = loaded_texture;
      attack_anim = {0, 0, total_frames, frame_width, frame_rec};
      break;

        case CharacterState::STATE_JUMP:
      jump_texture = loaded_texture;
      jump_anim = {0, 0, total_frames, frame_width, frame_rec};
      break;

      default:
        cout << "Sprite Type not identified\n";
        break;
  }
}

void Character::draw()
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

void Character::update()
{
  using namespace GameConstants;

  if (IsKeyDown(KEY_D))
  {

    if (is_attacking)
    {      
      attack_anim.frame_counter = 0;
      attack_anim.curr_frame = 0;
      attack_anim.frame_rec.x = 0.f;
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
      attack_anim.frame_rec.x = 0.f;
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
    attack_anim.frame_rec.x = 0.f;
  }
  else if (IsKeyPressed(KEY_SPACE) && !is_jumping)
  {
    is_jumping = true;
    current_state = STATE_JUMP;
    jump_anim.curr_frame = 0;
    jump_anim.frame_counter = 0;
    jump_anim.frame_rec.x = 0.f;    
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
      attack_anim.frame_rec.x = 0.f;
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
      attack_anim.frame_rec.x = 0.f;
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

