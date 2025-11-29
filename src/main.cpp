#include "constants.hpp"
#include "kitsune.hpp"
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

	// Link them so they can interact (attacks/damage)
	player.set_target_enemy(&kitsune);
	kitsune.set_target_player(&player);

	// Camera
	Camera2D camera = {0};
	camera.zoom = 1.0f;
	camera.offset = {(float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f};

	while (!WindowShouldClose())
	{
		// Update character movement
		player.update(currentMap);
		kitsune.update(currentMap);

		// --- Camera Logic ---
		// 1. Follow Player
		camera.target = player.get_position();

		// 2. Clamp Camera (Don't view outside map)
		float mapWidthPixels = currentMap.get_width() * currentMap.get_tile_size();
		float mapHeightPixels = currentMap.get_height() * currentMap.get_tile_size();

		float camW = SCREEN_WIDTH;
		float camH = SCREEN_HEIGHT;

		// Clamp X
		if (camera.target.x < camW / 2)
			camera.target.x = camW / 2;
		if (camera.target.x > mapWidthPixels - camW / 2)
			camera.target.x = mapWidthPixels - camW / 2;

		// Clamp Y (Optional, usually you want to follow up/down)
		// if (camera.target.y > mapHeightPixels - camH/2) camera.target.y = mapHeightPixels -
		// camH/2;

		// --- Drawing ---
		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode2D(camera);
		currentMap.draw();
		kitsune.draw();
		player.draw();
		EndMode2D();

		DrawFPS(10, 10);

		EndDrawing();
	}

	CloseWindow();
}
