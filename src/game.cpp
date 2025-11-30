#include <game.hpp>
#include <kitsune.hpp>
#include <raylib.h>

using namespace Resources;
using namespace MapResource;
using namespace MenuResource;
using namespace GameConstants;

Game::Game() : currentMap(BASE_SPRITE_SCALE), player({200.0f, GROUND_Y - 200.0f})
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Penance");
	SetTargetFPS(60);
	SetExitKey(0);

	background = LoadTexture(BACKGROUND_IMAGE.c_str());
	currentMap.load_map(CASTLE_IMAGE, CASTLE_CSV);
	player.init();

	init_enemies();

	camera = {0};
	camera.zoom = 1.0f;
	camera.offset = {(float)SCREEN_WIDTH * 0.38f, (float)SCREEN_HEIGHT * 0.75f};

	// Initialising Dialogue Box
	dialogueBox.Initialize();

	// Initialising Main Menu
	mainMenu.Initialize();

	shouldExit = false;
	currentState = MAIN_MENU;
	previousState = MAIN_MENU;
	storedDialogueFile = "";
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
	switch (currentState)
	{
		case MAIN_MENU:
		{
			update_main_menu();
			break;
		}

		case GAMEPLAY:
		{
			update_gameplay();
			break;
		}

		case DIALOGUE:
		{
			update_dialogue();
			break;
		}

		case PAUSED:
		{
			update_paused();
			break;
		}
	}
}


void Game::draw()
{
	BeginDrawing();
	ClearBackground(DARKGREEN);

	switch (currentState)
	{
		case MAIN_MENU:
		{
			draw_main_menu();
			break;
		}
		case GAMEPLAY:
		{
			draw_gameplay();
			break;
		}
		case DIALOGUE:
		{
			draw_dialogue();
			break;
		}
		case PAUSED:
		{
			draw_paused();
			break;
		}
	}

	EndDrawing();
}

void Game::run()
{
	while (!WindowShouldClose() && !shouldExit)
	{
		update();
		draw();
	}
}

void Game::init_enemies()
{
	//// Add enemies here. They will automatically register to the static list via constructor.
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

	// Set player target for all
	for (auto &enemy : enemies)
		enemy->set_target_player(&player);
}

void Game::update_camera()
{
	camera.target = player.get_position();

	float mapWidth = currentMap.get_width() * currentMap.get_tile_size();
	if (camera.target.x < SCREEN_WIDTH / 2)
		camera.target.x = SCREEN_WIDTH / 2;

	if (camera.target.x > mapWidth - SCREEN_WIDTH / 2)
		camera.target.x = mapWidth - SCREEN_WIDTH / 2;
}

void Game::update_main_menu()
{
	mainMenu.Update();
	if (mainMenu.IsStartClicked())
	{
		previousState = GAMEPLAY;
		currentState = DIALOGUE;
		dialogueBox.StartDialogue(INTRO_TXT.c_str());
	}
	if (mainMenu.IsExitClicked())
		shouldExit = true;
}

void Game::update_gameplay()
{

	// 1. Update Player (Inputs -> Physics -> Animation)
	player.update(currentMap);

	// 2. Update Enemies (AI -> Physics -> Animation OR Hurt/Dead logic)
	for (auto& enemy : enemies)
		enemy->update(currentMap);

	// 3. Cleanup Dead Enemies
	// This removes enemies only AFTER they have finished their death animation
	// (Character::process_state sets is_removed() to true when dead anim ends)
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		if ((*it)->is_removed())
		{
			delete* it;
			it = enemies.erase(it);
		}
		else
		{
			++it;
		}
	}

	update_camera();

	if (IsKeyPressed(KEY_T))
	{
		previousState = GAMEPLAY;
		currentState = DIALOGUE;

		if (!storedDialogueFile.empty())
		{
			dialogueBox.StartDialogue(DialogueResource::BASE_PATH + storedDialogueFile);
			storedDialogueFile = "";
		}
		else
		{
			dialogueBox.StartDialogue(INTRO_TXT.c_str());
		}
	}

	// Pause game
	if (IsKeyPressed(KEY_ESCAPE))
	{
		previousState = GAMEPLAY;
		currentState = PAUSED;
	}
}

void Game::update_dialogue()
{
	dialogueBox.Update();

	if (!dialogueBox.IsActive())
	{
		// Choice-based transition (immediate)
		if (dialogueBox.HasNextDialogue())
		{
			string nextFile = dialogueBox.GetNextDialogueFile();
			dialogueBox.StartDialogue(DialogueResource::BASE_PATH + nextFile);
		}
		// [End Dialogue] transition (delayed)
		else if (dialogueBox.HasPendingDialogue())
		{
			storedDialogueFile = dialogueBox.GetPendingDialogueFile();
			currentState = previousState;
		}
		// No next dialogue
		else
		{
			currentState = previousState;
		}
	}
}

void Game::update_paused()
{
	if (IsKeyPressed(KEY_ESCAPE))
	{
		currentState = previousState;
	}

	if (IsKeyPressed(KEY_M))
	{
		currentState = MAIN_MENU;
		previousState = MAIN_MENU;
	}
}

void Game::draw_main_menu()
{
	mainMenu.Draw();
}

void Game::draw_gameplay()
{
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
	DrawText("Press T for dialogue | ESC to pause", 700, 100, 20, LIGHTGRAY);
}

void Game::draw_dialogue()
{
	// Draw previous state underneath (frozen)
	if (previousState == GAMEPLAY)
	{
		ClearBackground(DARKBLUE);
		DrawText("GAMEPLAY", 850, 50, 40, WHITE);
		DrawText("Press T for dialogue | ESC to pause", 700, 100, 20, LIGHTGRAY);
	}
	else if (previousState == MAIN_MENU)
	{
		mainMenu.Draw();
	}

	// Draw dialogue box on top
	dialogueBox.Draw();
}

void Game::draw_paused()
{
	// Draw gameplay underneath
	draw_gameplay();

	// Pause overlay
	DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
	DrawText("PAUSED", 850, 450, 60, WHITE);
	DrawText("ESC - Resume | M - Main Menu", 720, 550, 25, GRAY);
}
