#include "raylib.h"
#include "character.hpp"
#include "texture.hpp"

using namespace Resources;

int main ()
{
  const int screenheight = GetScreenHeight();  
  const int screenwidth = GetScreenWidth();
  InitWindow(screenwidth, screenheight, "game");
  SetTargetFPS(60);

  Vector2 pos = {128.0f, 128.0f};
  Character lady(pos);

  lady.load_texture("run", Kitsune::RUN_TEXTURE.c_str(), Kitsune::RUN_FRAMES);
  lady.load_texture("idle", Kitsune::IDLE_TEXTURE.c_str(), Kitsune::IDLE_FRAMES);
  lady.load_texture("attack", Kitsune::ATTACK_TEXTURE.c_str(), Kitsune::ATTACK_FRAMES);
  lady.load_texture("jump", Kitsune::JUMP_TEXTURE.c_str(), Kitsune::JUMP_FRAMES);

  while (!WindowShouldClose())
  {
    lady.update();

    BeginDrawing();
    ClearBackground(WHITE);

    lady.draw();

    EndDrawing();
    
  }
  return 0;
}
