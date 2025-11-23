#include "enemy.hpp"
#include "player.hpp"
#include "resources.hpp"

using namespace Resources;

int main()
{
	int current_monitor = GetCurrentMonitor();
	const int screenheight = GetMonitorHeight(current_monitor);
	const int screenwidth = GetMonitorWidth(current_monitor);
	InitWindow(screenwidth, screenheight, "Penance");
	SetTargetFPS(60);

	Vector2 pos = {128.0f, 128.0f};
	Player player(pos);

	player.load_texture(STATE_RUN, PlayerResource::RUN_TEXTURE.c_str(), PlayerResource::RUN_FRAMES);
	player.load_texture(STATE_IDLE, PlayerResource::IDLE_TEXTURE.c_str(),
						PlayerResource::IDLE_FRAMES);
	player.load_texture(STATE_ATTACK, PlayerResource::ATTACK_TEXTURE.c_str(),
						PlayerResource::ATTACK_FRAMES);
	player.load_texture(STATE_JUMP, PlayerResource::JUMP_TEXTURE.c_str(),
						PlayerResource::JUMP_FRAMES);

	Vector2 pos2 = {128.0f, 256.0f};
	Enemy kitsune(pos2);

	kitsune.load_texture(STATE_RUN, KitsuneResource::RUN_TEXTURE.c_str(),
						 KitsuneResource::RUN_FRAMES);
	kitsune.load_texture(STATE_IDLE, KitsuneResource::IDLE_TEXTURE.c_str(),
						 KitsuneResource::IDLE_FRAMES);
	kitsune.load_texture(STATE_ATTACK, KitsuneResource::ATTACK_TEXTURE.c_str(),
						 KitsuneResource::ATTACK_FRAMES);
	kitsune.load_texture(STATE_JUMP, KitsuneResource::JUMP_TEXTURE.c_str(),
						 KitsuneResource::JUMP_FRAMES);

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
