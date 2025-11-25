#include "constants.hpp"
#include "kitsune.hpp"
#include "player.hpp"
#include "raylib.h"

int main()
{
    int current_monitor = GetCurrentMonitor();
    const int screenheight = GetMonitorHeight(current_monitor);
    const int screenwidth = GetMonitorWidth(current_monitor);
    InitWindow(screenwidth, screenheight, "Penance");
    SetTargetFPS(60);

    Vector2 pos = {100.0f, GameConstants::GROUND_Y};
    Player player(pos);
    player.init();

    Vector2 pos2 = {128.0f, GameConstants::GROUND_Y};
    Kitsune kitsune(pos2);
    kitsune.init();

    Camera2D camera = {0};
    camera.offset = { (float)screenwidth * 0.5f, (float)screenheight * 0.5f };
    camera.rotation = 0.0f;
    camera.zoom = 1.5f;

    player.take_damage(30);

    while (!WindowShouldClose())
    {
        player.update();
        kitsune.update();

        // Update camera: center screen on player's sprite center
        Vector2 camera_position = player.get_position();
        camera.target = {camera_position.x - 400, camera_position.y - 150};

        BeginDrawing();
        ClearBackground(WHITE);

        BeginMode2D(camera);
        player.draw();
        kitsune.draw();
        EndMode2D();

        EndDrawing();
    }
}
