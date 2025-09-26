#include "raylib.h"
#include "character.hpp"

int main ()
{
  const int screenheight = GetScreenHeight();  
  const int screenwidth = GetScreenWidth();
  InitWindow(screenwidth, screenheight, "game");
  SetTargetFPS(60);

  Vector2 pos = {128.0f, 128.0f};
  Character lady(pos);

  lady.load_texture("run", "resources/yokai/Kitsune/Run.png", 8);
  lady.load_texture("idle", "resources/yokai/Kitsune/Idle.png", 8);
  lady.load_texture("attack", "resources/yokai/Kitsune/Attack_3.png", 7);
  lady.load_texture("jump", "resources/yokai/Kitsune/Jump.png", 10);

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
