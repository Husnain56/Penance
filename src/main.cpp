#include "constants.hpp"
#include "kitsune.hpp"
#include "blue_samurai.hpp"
#include "skeleton_warrior.hpp"
#include "purple_knight.hpp"
#include "skeleton_spearman.hpp"
#include "silver_knight.hpp"
#include "karasu_tengu.hpp"
#include "yamabushi.hpp"
#include "map.hpp"
#include "player.hpp"
#include "resources.hpp"

int main()
{
	using namespace GameConstants;
	using namespace Resources::MapResource;
	using namespace Resources;

	// Initialising Window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Penance");
	SetTargetFPS(60);

	// Background
	Texture2D background = LoadTexture(BACKGROUND_IMAGE.c_str());

	// Setup Map
	Map currentMap(BASE_SPRITE_SCALE);
	currentMap.load_map(CASTLE_IMAGE, CASTLE_CSV);

	// Setup Player
	Vector2 pos = { 200.0f, GROUND_Y - 200.0f };
	Player player(pos);
	player.init();

	// Setup Enemy::Kitsune
	pos = { 1000.0f, GROUND_Y };
	Kitsune kitsune(pos);
	kitsune.init();

	// Setup BlueSamurai (spawn closer so you can test)
	pos = { 8000.0f, GROUND_Y };
	BlueSamurai samurai(pos);
	samurai.init();
	samurai.set_scale(2.2f);	

	pos = { 2500.0f, GROUND_Y };
	SkeletonWarrior skeleton(pos);
	skeleton.init();

	pos = { 3500.0f, GROUND_Y };
	PurpleKnight knight(pos);
	knight.init();

	pos = { 5000.0f, GROUND_Y };
	SkeletonSpearman spearman(pos);
	spearman.init();

	pos = { 6000.0f, GROUND_Y };
	SilverKnight silver_knight(pos);
	silver_knight.init();

	pos = { 6500.0f, GROUND_Y };
	KarasuTengu tengu(pos);
	tengu.init();

	pos = { 45000.0f, GROUND_Y };
	Yamabushi yamabushi(pos);
	yamabushi.init();

	// Link them so they can interact (attacks/damage)
	kitsune.set_target_player(&player);
	samurai.set_target_player(&player);
	skeleton.set_target_player(&player);
	knight.set_target_player(&player);
	spearman.set_target_player(&player);	
	silver_knight.set_target_player(&player);
	tengu.set_target_player(&player);
	yamabushi.set_target_player(&player);

	// Camera
	Camera2D camera = { 0 };
	camera.zoom = 1.0f;
	camera.offset = { (float)SCREEN_WIDTH * 0.38f, (float)SCREEN_HEIGHT * 0.75f };

	while (!WindowShouldClose())
	{
		// Update character movement
		player.update(currentMap);
		kitsune.update(currentMap);
		samurai.update(currentMap);
		skeleton.update(currentMap);
		knight.update(currentMap);
		spearman.update(currentMap);
		silver_knight.update(currentMap);
		tengu.update(currentMap);
		yamabushi.update(currentMap);

		// --- Camera Logic ---
		// 1. Follow Player
		camera.target = player.get_position();

		// Clamp Camera logic...
		float mapWidth = currentMap.get_width() * currentMap.get_tile_size();
		if (camera.target.x < SCREEN_WIDTH / 2) camera.target.x = SCREEN_WIDTH / 2;
		if (camera.target.x > mapWidth - SCREEN_WIDTH / 2) camera.target.x = mapWidth - SCREEN_WIDTH / 2;

		// --- Drawing ---
		BeginDrawing();
		ClearBackground(DARKGREEN);

		if (background.id != 0)
		{
			Rectangle src = { 0, 0, (float)background.width, (float)background.height };
			Rectangle dest = { 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT }; // Stretch to fill screen
			DrawTexturePro(background, src, dest, { 0,0 }, 0.0f, WHITE);
		}

		BeginMode2D(camera);

		currentMap.draw();
		kitsune.draw();
		samurai.draw();
		player.draw();
		skeleton.draw();
		knight.draw();
		spearman.draw();	
		silver_knight.draw();
		tengu.draw();
		yamabushi.draw();

		EndMode2D();

		DrawFPS(10, 10);

		EndDrawing();
	}

	CloseWindow();
}
