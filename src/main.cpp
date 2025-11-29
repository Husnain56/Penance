#include "constants.hpp"
#include "kitsune.hpp"
#include "blue_samurai.hpp"
#include "map.hpp"
#include "player.hpp"
#include "resources.hpp"

int main()
{
	using namespace GameConstants;
	using namespace Resources::MapResource;

	// Initialising Window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Penance");
	SetTargetFPS(60);

	// Setup Map
	Map currentMap(BASE_SPRITE_SCALE);
	currentMap.load_map(CASTLE_IMAGE, CASTLE_CSV);

	// Setup Player
	Vector2 pos = {200.0f, GROUND_Y - 40.0f};
	Player player(pos);
	player.init();

	// Setup Enemy::Kitsune
	pos = {800.0f, GROUND_Y};
	Kitsune kitsune(pos);
	kitsune.init();

	// Setup BlueSamurai (spawn closer so you can test)
	/*pos = {1100.0f, GROUND_Y};
	BlueSamurai samurai(pos);
	samurai.init();
	samurai.set_scale(2.2f);*/

	// Link them so they can interact (attacks/damage)
	kitsune.set_target_player(&player);
	//samurai.set_target_player(&player);

	// Camera
	Camera2D camera = {0};
	camera.zoom = 1.0f;
	camera.offset = {(float)SCREEN_WIDTH * 0.38f, (float)SCREEN_HEIGHT * 0.5f};

	while (!WindowShouldClose())
	{
		// Update character movement
		player.update(currentMap);
		kitsune.update(currentMap);
		//samurai.update(currentMap);

		// --- Camera Logic ---
		// 1. Follow Player
		camera.target = player.get_position();

		// --- Drawing ---
		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode2D(camera);
		currentMap.draw();
		kitsune.draw();
		//samurai.draw();
		player.draw();
		EndMode2D();

		DrawFPS(10, 10);

		EndDrawing();
	}

	CloseWindow();
}
