#include "character.hpp"
#include "raylib.h"
#include "texture.hpp"

using namespace Resources;

int
main ()
{
	int current_monitor = GetCurrentMonitor();
	const int screenheight = GetMonitorHeight(current_monitor);
	const int screenwidth = GetMonitorWidth(current_monitor);
	InitWindow (screenwidth, screenheight, "game");
	SetTargetFPS (60);

	Vector2 pos = { 128.0f, 128.0f };
	Character kitsune (pos);

	kitsune.load_texture (STATE_RUN, Kitsune::RUN_TEXTURE.c_str (), Kitsune::RUN_FRAMES);
	kitsune.load_texture (STATE_IDLE, Kitsune::IDLE_TEXTURE.c_str (), Kitsune::IDLE_FRAMES);
	kitsune.load_texture (STATE_ATTACK, Kitsune::ATTACK_TEXTURE.c_str (), Kitsune::ATTACK_FRAMES);
	kitsune.load_texture (STATE_JUMP, Kitsune::JUMP_TEXTURE.c_str (), Kitsune::JUMP_FRAMES);

	// lady.load_texture(STATE_RUN, KarasuTengu::RUN_TEXTURE.c_str(),
	// KarasuTengu::RUN_FRAMES); lady.load_texture(STATE_IDLE,
	// KarasuTengu::IDLE_TEXTURE.c_str(), KarasuTengu::IDLE_FRAMES);
	// lady.load_texture(STATE_ATTACK, KarasuTengu::ATTACK_TEXTURE.c_str(),
	// KarasuTengu::ATTACK_FRAMES); lady.load_texture(STATE_JUMP,
	// KarasuTengu::JUMP_TEXTURE.c_str(), KarasuTengu::JUMP_FRAMES);

	while (!WindowShouldClose ())
		{
			kitsune.update ();

			BeginDrawing ();
			ClearBackground (WHITE);

			kitsune.draw ();

			EndDrawing ();
		}
	return 0;
}
