#pragma once

#include <raylib.h>
#include <resources.hpp>
#include <string>
using namespace Resources::MenuResource;

class MainMenu
{
  private:
	// ========== TEXTURES ==========
	Texture2D menuBackground;
	Texture2D menuOverlay;
	bool textureLoaded;
	bool overlayLoaded;

	// ========== BUTTON REGIONS ==========
	Rectangle startButton;
	Rectangle optionsButton;
	Rectangle creditsButton;
	Rectangle exitButton;
	Rectangle closeButton;

	// ========== BUTTON STATES ==========
	bool startClicked;
	bool optionsClicked;
	bool creditsClicked;
	bool exitClicked;

	// ========== SUBMENU STATES ==========
	bool showOptionsMenu;
	bool showCreditsMenu;

	// ========== LAYOUT CONSTANTS ==========
	static constexpr float MENU_OVERLAY_WIDTH = 400.0f;
	static constexpr float MENU_OVERLAY_HEIGHT = 500.0f;
	static constexpr float MENU_OVERLAY_OFFSET_X = 50.0f;
	static constexpr float MENU_OVERLAY_OFFSET_Y = 200.0f;

	// ========== POPUP CONSTANTS ==========
	static constexpr float POPUP_X = 500.0f;
	static constexpr float POPUP_Y = 200.0f;
	static constexpr float POPUP_WIDTH = 920.0f;
	static constexpr float POPUP_HEIGHT = 680.0f;
	static constexpr int POPUP_BORDER_THICKNESS = 3;

	// ========== TEXT LAYOUT CONSTANTS ==========
	static constexpr int TITLE_Y = 240;
	static constexpr int CONTENT_START_Y = 340;
	static constexpr int LINE_SPACING = 50;
	static constexpr int SECTION_SPACING = 80;

	// ========== HELPER FUNCTIONS ==========
	bool IsRectangleClicked(Rectangle rect)
	{
		Vector2 mousePos = GetMousePosition();
		return CheckCollisionPointRec(mousePos, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	}

	bool IsRectangleHovered(Rectangle rect)
	{
		Vector2 mousePos = GetMousePosition();
		return CheckCollisionPointRec(mousePos, rect);
	}

	// ========== POPUP DRAWING FUNCTIONS ==========
	void DrawOptionsMenu()
	{
		// Dark overlay
		DrawRectangle(0, 0, 1920, 1080, Fade(BLACK, 0.7f));

		// Popup box
		Rectangle popupBox = {POPUP_X, POPUP_Y, POPUP_WIDTH, POPUP_HEIGHT};
		DrawRectangleRec(popupBox, WHITE);
		DrawRectangleRec(popupBox, Fade(DARKPURPLE, 0.2f));
		DrawRectangleLinesEx(popupBox, POPUP_BORDER_THICKNESS, DARKPURPLE);

		// Title
		DrawText("HOW TO PLAY", 820, TITLE_Y, 40, DARKPURPLE);
		DrawLine(550, 290, 1370, 290, DARKPURPLE);

		// Content
		int yPos = CONTENT_START_Y;

		DrawText("CONTROLS:", 550, yPos, 30, DARKGRAY);
		yPos += LINE_SPACING;
		DrawText("- ENTER: Advance dialogue", 570, yPos, 25, BLACK);
		DrawText("- 1-9: Select dialogue choices", 950, yPos, 25, BLACK);
		yPos += LINE_SPACING;
		DrawText("- WASD: Move", 570, yPos, 25, BLACK);
		DrawText("- Spacebar: Jump", 950, yPos, 25, BLACK);
		yPos += LINE_SPACING;
		DrawText("- E: Attack", 570, yPos, 25, BLACK);
		DrawText("- ESC: Pause game", 950, yPos, 25, BLACK);
		yPos += SECTION_SPACING;
		DrawText("GAMEPLAY:", 550, yPos, 30, DARKGRAY);
		yPos += LINE_SPACING;
		DrawText("- Read dialogues and make choices", 570, yPos, 25, BLACK);
		yPos += LINE_SPACING;
		DrawText("- Your choices affect the story", 570, yPos, 25, BLACK);
		yPos += LINE_SPACING;
		DrawText("- Explore different paths", 570, yPos, 25, BLACK);

		// Back button
		DrawRectangleRec(closeButton, DARKPURPLE);
		DrawText("BACK", closeButton.x + 5, closeButton.y + 12, 25, WHITE);
		DrawText("Click BACK to return to main menu", 750, 830, 20, DARKGRAY);
	}

	void DrawCreditsMenu()
	{
		// Dark overlay
		DrawRectangle(0, 0, 1920, 1080, Fade(BLACK, 0.7f));

		// Popup box
		Rectangle popupBox = {POPUP_X, POPUP_Y, POPUP_WIDTH, POPUP_HEIGHT};
		DrawRectangleRec(popupBox, WHITE);
		DrawRectangleRec(popupBox, Fade(GOLD, 0.1f));
		DrawRectangleLinesEx(popupBox, POPUP_BORDER_THICKNESS, GOLD);

		// Title
		DrawText("CREDITS", 880, TITLE_Y, 40, DARKGRAY);
		DrawLine(550, 290, 1370, 290, GOLD);

		// Content
		int yPos = 360;
		const int creditSpacing = 60;

		DrawText("DEVELOPMENT TEAM", 800, yPos, 30, DARKGRAY);
		yPos += creditSpacing + 20;

		DrawText("Game Mechanic Designer", 650, yPos, 25, GOLD);
		DrawText("Eesa Shoaib && Ahsan Baig", 1050, yPos, 25, BLACK);
		yPos += creditSpacing;

		DrawText("Map Designer", 650, yPos, 25, GOLD);
		DrawText("Husnain Barkat", 1050, yPos, 25, BLACK);
		yPos += creditSpacing;

		DrawText("Story Writer", 650, yPos, 25, GOLD);
		DrawText("Abdullah Mushtaq", 1050, yPos, 25, BLACK);
		yPos += creditSpacing;

		DrawText("Artist", 650, yPos, 25, GOLD);
		DrawText("Harris Tabassum", 1050, yPos, 25, BLACK);
		yPos += creditSpacing + 30;

		DrawText("Built with Raylib", 830, yPos, 22, DARKGRAY);
		yPos += 40;
		DrawText("© 2025 - All Rights Reserved", 800, yPos, 20, GRAY);

		// Back button
		DrawRectangleRec(closeButton, GOLD);
		DrawText("BACK", closeButton.x + 5, closeButton.y + 12, 25, WHITE);
		DrawText("Click BACK to return to main menu", 750, 830, 20, DARKGRAY);
	}

  public:
	// ========== CONSTRUCTOR ==========
	MainMenu()
		: textureLoaded(false), overlayLoaded(false), startClicked(false), optionsClicked(false),
		  creditsClicked(false), exitClicked(false), showOptionsMenu(false), showCreditsMenu(false)
	{

		menuBackground = {0};
		menuOverlay = {0};

		// Button positions (x, y, width, height)
		startButton = {860.0f, 580.0f, 290.0f, 70.0f};
		optionsButton = {880.0f, 740.0f, 250.0f, 60.0f};
		creditsButton = {880.0f, 800.0f, 250.0f, 60.0f};
		exitButton = {910.0f, 880.0f, 150.0f, 70.0f};

		// Close button for popups
		closeButton = {910.0f, 850.0f, 100.0f, 50.0f};
	}

	// ========== DESTRUCTOR ==========
	~MainMenu()
	{
		if (textureLoaded)
			UnloadTexture(menuBackground);
		if (overlayLoaded)
			UnloadTexture(menuOverlay);
	}

	// ========== INITIALIZATION ==========
	void Initialize()
	{
		// Load background texture
		if (!textureLoaded)
		{
			menuBackground = LoadTexture(MAIN_MENU_IMAGE.c_str());
			textureLoaded = (menuBackground.id != 0);
			if (!textureLoaded)
			{
				printf("Failed to load main menu texture!\n");
			}
		}

		// Load overlay texture with transparency
		if (!overlayLoaded)
		{
			Image menuImg = LoadImage(MENU_IMAGE.c_str());
			if (menuImg.data != nullptr)
			{
				// Replace black with transparent
				Color black = {0, 0, 0, 255};
				Color transparent = {0, 0, 0, 0};
				ImageColorReplace(&menuImg, black, transparent);

				menuOverlay = LoadTextureFromImage(menuImg);
				UnloadImage(menuImg);

				overlayLoaded = (menuOverlay.id != 0);
				if (!overlayLoaded)
				{
					printf("Failed to create menu overlay texture!\n");
				}
			}
			else
			{
				printf("Failed to load menu overlay image!\n");
			}
		}
	}

	// ========== UPDATE LOGIC ==========
	void Update()
	{
		// Reset click states
		startClicked = false;
		optionsClicked = false;
		creditsClicked = false;
		exitClicked = false;

		// Handle submenu close button
		if (showOptionsMenu || showCreditsMenu)
		{
			if (IsRectangleClicked(closeButton))
			{
				showOptionsMenu = false;
				showCreditsMenu = false;
			}
			return; // Don't process other buttons while submenu is open
		}

		// Check main menu buttons
		if (IsRectangleClicked(startButton))
		{
			startClicked = true;
		}
		if (IsRectangleClicked(optionsButton))
		{
			optionsClicked = true;
			showOptionsMenu = true;
		}
		if (IsRectangleClicked(creditsButton))
		{
			creditsClicked = true;
			showCreditsMenu = true;
		}
		if (IsRectangleClicked(exitButton))
		{
			exitClicked = true;
		}
	}

	// ========== DRAW FUNCTION ==========
	void Draw(bool showDebug = false)
	{
		if (!textureLoaded)
		{
			// Fallback display
			ClearBackground(BLACK);
			DrawText("MAIN MENU", 810, 400, 60, WHITE);
			DrawText("(Background image failed to load)", 700, 500, 20, RED);
			return;
		}

		// Draw background
		Rectangle bgSource = {0, 0, (float)menuBackground.width, (float)menuBackground.height};
		Rectangle bgDest = {0, 0, 1920, 1080};
		DrawTexturePro(menuBackground, bgSource, bgDest, {0, 0}, 0.0f, WHITE);

		// Draw overlay
		if (overlayLoaded)
		{
			Rectangle overlaySource = {0, 0, (float)menuOverlay.width, (float)menuOverlay.height};

			float overlayX = (1920 - MENU_OVERLAY_WIDTH) / 2 + MENU_OVERLAY_OFFSET_X;
			float overlayY = (1080 - MENU_OVERLAY_HEIGHT) / 2 + MENU_OVERLAY_OFFSET_Y;

			Rectangle overlayDest = {overlayX, overlayY, MENU_OVERLAY_WIDTH, MENU_OVERLAY_HEIGHT};
			DrawTexturePro(menuOverlay, overlaySource, overlayDest, {0, 0}, 0.0f, WHITE);
		}

		// Draw debug rectangles
		if (showDebug && !showOptionsMenu && !showCreditsMenu)
		{
			if (IsRectangleHovered(startButton))
				DrawRectangleRec(startButton, Fade(GREEN, 0.3f));
			if (IsRectangleHovered(optionsButton))
				DrawRectangleRec(optionsButton, Fade(YELLOW, 0.3f));
			if (IsRectangleHovered(creditsButton))
				DrawRectangleRec(creditsButton, Fade(BLUE, 0.3f));
			if (IsRectangleHovered(exitButton))
				DrawRectangleRec(exitButton, Fade(RED, 0.3f));
		}

		// Draw submenus
		if (showOptionsMenu)
			DrawOptionsMenu();
		if (showCreditsMenu)
			DrawCreditsMenu();
	}

	// ========== GETTERS ==========
	bool IsStartClicked() const { return startClicked; }
	bool IsOptionsClicked() const { return optionsClicked; }
	bool IsCreditsClicked() const { return creditsClicked; }
	bool IsExitClicked() const { return exitClicked; }

	// ========== SETTERS ==========
	void SetButtonPositions(Rectangle start, Rectangle options, Rectangle credits, Rectangle exit)
	{
		startButton = start;
		optionsButton = options;
		creditsButton = credits;
		exitButton = exit;
	}
};
