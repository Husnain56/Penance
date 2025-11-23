#include "kitsune.hpp"
#include "player.hpp"

int main()
{
	int current_monitor = GetCurrentMonitor();
	const int screenheight = GetMonitorHeight(current_monitor);
	const int screenwidth = GetMonitorWidth(current_monitor);
	InitWindow(screenwidth, screenheight, "Penance");
	SetTargetFPS(60);

	Vector2 pos = {128.0f, 128.0f};
	Player player(pos);
	player.init();

	Vector2 pos2 = {128.0f, 256.0f};
	Kitsune kitsune(pos2);
	kitsune.init();

	while (!WindowShouldClose())
	{
		player.update();
		kitsune.update();

		BeginDrawing();
		ClearBackground(WHITE);

		player.draw();
		kitsune.draw();

		EndDrawing();
	}
}
