#include <game.hpp>
using namespace Resources;
using namespace MapResource;
using namespace GameConstants;

Game::Game() : currentMap(BASE_SPRITE_SCALE), player({200.0f, GROUND_Y - 200.0f})
{
	// Initialising Window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Penance");
	SetTargetFPS(60);

	// Initialising Resources
	background = LoadTexture(BACKGROUND_IMAGE.c_str());

	// Initialising Map
	currentMap.load_map(CASTLE_IMAGE, CASTLE_CSV);

	// Initialising Player
	player.init();

	// Initialising Enemies
	init_enemies();

	// Initialising Camera
	camera = {0};
	camera.zoom = 1.0f;
	camera.offset = {(float)SCREEN_WIDTH * 0.38f, (float)SCREEN_HEIGHT * 0.75f};
}

Game::~Game()
{
	for (auto &enemy : enemies)
		delete enemy;

	enemies.clear();

	UnloadTexture(background);
	CloseWindow();
}

void Game::update()
{
	// Update character movement
	player.update(currentMap);

	for (auto &enemy : enemies)
		enemy->update(currentMap);

	// Update camera logic
	update_camera();
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(DARKGREEN);

	if (background.id != 0)
	{
		Rectangle src = {0, 0, (float)background.width, (float)background.height};
		Rectangle dest = {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
		DrawTexturePro(background, src, dest, {0, 0}, 0.0f, WHITE);
	}

	BeginMode2D(camera);

	currentMap.draw();
	player.draw();

	for (auto &enemy : enemies)
		enemy->draw();

	EndMode2D();

	DrawFPS(10, 20);

	EndDrawing();
}

void Game::run()
{
	while (!WindowShouldClose())
	{
		update();
		draw();
	}
}

void Game::init_enemies()
{
	// Setup Kitsune
	Vector2 pos = {1000.0f, GROUND_Y};
	Kitsune *kitsune = new Kitsune(pos);
	kitsune->init();
	enemies.push_back(kitsune);

	// Setup Blue Samurai
	pos = {5000.0f, GROUND_Y};
	BlueSamurai *samurai = new BlueSamurai(pos);
	samurai->init();
	samurai->set_scale(2.2f);
	enemies.push_back(samurai);

	// Setup Skeleton Warrior
	pos = {7000.0f, GROUND_Y};
	SkeletonWarrior *skeleton = new SkeletonWarrior(pos);
	skeleton->init();
	enemies.push_back(skeleton);

	// Setup Purple Knight
	pos = {9000.0f, GROUND_Y};
	PurpleKnight *knight = new PurpleKnight(pos);
	knight->init();
	enemies.push_back(knight);

	// Setup Skeleton Spearman
	pos = {11000.0f, GROUND_Y};
	SkeletonSpearman *spearman = new SkeletonSpearman(pos);
	spearman->init();
	enemies.push_back(spearman);

	// Setup Silver Knight
	pos = {13000.0f, GROUND_Y};
	SilverKnight *silver_knight = new SilverKnight(pos);
	silver_knight->init();
	enemies.push_back(silver_knight);

	// Setup Karasu Tengu
	pos = {15000.0f, GROUND_Y};
	KarasuTengu *tengu = new KarasuTengu(pos);
	tengu->init();
	enemies.push_back(tengu);

	// Setup Yamabushi
	pos = {17000.0f, GROUND_Y};
	Yamabushi *yamabushi = new Yamabushi(pos);
	yamabushi->init();
	enemies.push_back(yamabushi);

	for (auto &enemy : enemies)
		enemy->set_target_player(&player);
}

void Game::update_camera()
{
	// Follow Player
	camera.target = player.get_position();

	// Clamp Camera Logic
	float mapWidth = currentMap.get_width() * currentMap.get_tile_size();
	if (camera.target.x < SCREEN_WIDTH / 2)
		camera.target.x = SCREEN_WIDTH / 2;

	if (camera.target.x > mapWidth - SCREEN_WIDTH / 2)
		camera.target.x = mapWidth - SCREEN_WIDTH / 2;
}
