#pragma once

#include <raylib.h>
#include <resources.hpp>
#include <string>
using namespace Resources::MenuResource;
using namespace Resources::FontResource;

class MainMenu
{
  private:
	// ========== TEXTURES & FONTS ==========
	Texture2D menuBackground;
	Texture2D menuOverlay;
	Font menuFont; // Added Font member

	bool textureLoaded;
	bool overlayLoaded;
	bool fontLoaded; // Added Font loaded state

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

	// =====================================================================
	//                              OPTIONS POPUP (NAVY BLUE)
	// =====================================================================
	void DrawOptionsMenu()
	{
		// Dark overlay behind popup
		DrawRectangle(0, 0, 1920, 1080, Fade(BLACK, 0.7f));

		Rectangle popupBox = {POPUP_X, POPUP_Y, POPUP_WIDTH, POPUP_HEIGHT};

		Color navy = Color{10, 25, 70, 255};
		Color navyLight = Color{40, 80, 160, 255};
		Color textWhite = Color{235, 235, 245, 255};

		// Navy popup background
		DrawRectangleRec(popupBox, navy);
		DrawRectangleLinesEx(popupBox, POPUP_BORDER_THICKNESS, navyLight);

		// Title
		DrawTextEx(menuFont, "HOW TO PLAY", {820, (float)TITLE_Y}, 40, 1, textWhite);
		DrawLine(550, 290, 1370, 290, navyLight);

		// Content
		int yPos = CONTENT_START_Y;

		DrawTextEx(menuFont, "CONTROLS:", {550, (float)yPos}, 30, 1, textWhite);
		yPos += LINE_SPACING;
		DrawTextEx(menuFont, "- ENTER: Advance dialogue", {570, (float)yPos}, 25, 1, textWhite);
		DrawTextEx(menuFont, "- 1-9: Select dialogue choices", {950, (float)yPos}, 25, 1,
				   textWhite);
		yPos += LINE_SPACING;
		DrawTextEx(menuFont, "- WASD: Move", {570, (float)yPos}, 25, 1, textWhite);
		DrawTextEx(menuFont, "- Spacebar: Jump", {950, (float)yPos}, 25, 1, textWhite);
		yPos += LINE_SPACING;
		DrawTextEx(menuFont, "- E: Attack", {570, (float)yPos}, 25, 1, textWhite);
		DrawTextEx(menuFont, "- ESC: Pause game", {950, (float)yPos}, 25, 1, textWhite);

		yPos += SECTION_SPACING;
		DrawTextEx(menuFont, "GAMEPLAY:", {550, (float)yPos}, 30, 1, textWhite);
		yPos += LINE_SPACING;
		DrawTextEx(menuFont, "- Read dialogues and make choices", {570, (float)yPos}, 25, 1,
				   textWhite);
		yPos += LINE_SPACING;
		DrawTextEx(menuFont, "- Your choices affect the story", {570, (float)yPos}, 25, 1,
				   textWhite);
		yPos += LINE_SPACING;
		DrawTextEx(menuFont, "- Explore different paths", {570, (float)yPos}, 25, 1, textWhite);

		// Back button
		DrawRectangleRec(closeButton, navyLight);
		DrawTextEx(menuFont, "BACK", {closeButton.x + 5, closeButton.y + 12}, 25, 1, WHITE);
		DrawTextEx(menuFont, "Click BACK to return to main menu", {750, 830}, 20, 1, textWhite);
	}

	// =====================================================================
	//                              CREDITS POPUP (NAVY BLUE)
	// =====================================================================
	void DrawCreditsMenu()
	{
		DrawRectangle(0, 0, 1920, 1080, Fade(BLACK, 0.7f));

		Rectangle popupBox = {POPUP_X, POPUP_Y, POPUP_WIDTH, POPUP_HEIGHT};

		Color navy = Color{10, 25, 70, 255};
		Color royalBlue = Color{60, 110, 220, 255};
		Color textWhite = Color{235, 235, 245, 255};
		Color gold = Color{220, 185, 40, 255};

		// Navy popup background
		DrawRectangleRec(popupBox, navy);
		DrawRectangleLinesEx(popupBox, POPUP_BORDER_THICKNESS, royalBlue);

		DrawTextEx(menuFont, "CREDITS", {880, (float)TITLE_Y}, 40, 1, textWhite);
		DrawLine(550, 290, 1370, 290, royalBlue);

		int yPos = 360;
		const int creditSpacing = 60;

		DrawTextEx(menuFont, "DEVELOPMENT TEAM", {800, (float)yPos}, 30, 1, textWhite);
		yPos += creditSpacing + 20;

		DrawTextEx(menuFont, "Game Mechanic Designer", {650, (float)yPos}, 25, 1, gold);
		DrawTextEx(menuFont, "Eesa Shoaib && Ahsan Baig", {1050, (float)yPos}, 25, 1, textWhite);
		yPos += creditSpacing;

		DrawTextEx(menuFont, "Map Designer", {650, (float)yPos}, 25, 1, gold);
		DrawTextEx(menuFont, "Husnain Barkat", {1050, (float)yPos}, 25, 1, textWhite);
		yPos += creditSpacing;

		DrawTextEx(menuFont, "Story Writer", {650, (float)yPos}, 25, 1, gold);
		DrawTextEx(menuFont, "Abdullah Mushtaq", {1050, (float)yPos}, 25, 1, textWhite);
		yPos += creditSpacing;

		DrawTextEx(menuFont, "Director", {650, (float)yPos}, 25, 1, gold);
		DrawTextEx(menuFont, "Harris Tabassum", {1050, (float)yPos}, 25, 1, textWhite);
		yPos += creditSpacing + 30;

		DrawTextEx(menuFont, "Built with Raylib", {830, (float)yPos}, 22, 1, textWhite);
		yPos += 40;
		DrawTextEx(menuFont, "© 2025 - All Rights Reserved", {800, (float)yPos}, 20, 1, textWhite);

		DrawRectangleRec(closeButton, royalBlue);
		DrawTextEx(menuFont, "BACK", {closeButton.x + 5, closeButton.y + 12}, 25, 1, WHITE);
		DrawTextEx(menuFont, "Click BACK to return to main menu", {750, 830}, 20, 1, textWhite);
	}

  public:
	// ========== CONSTRUCTOR ==========
	MainMenu()
		: textureLoaded(false), overlayLoaded(false), fontLoaded(false), startClicked(false),
		  optionsClicked(false), creditsClicked(false), exitClicked(false), showOptionsMenu(false),
		  showCreditsMenu(false)
	{
		menuBackground = {0};
		menuOverlay = {0};
		menuFont = {0};

		startButton = {860.0f, 580.0f, 290.0f, 70.0f};
		optionsButton = {880.0f, 740.0f, 250.0f, 60.0f};
		creditsButton = {880.0f, 800.0f, 250.0f, 60.0f};
		exitButton = {910.0f, 880.0f, 150.0f, 70.0f};

		closeButton = {910.0f, 850.0f, 100.0f, 50.0f};
	}

	// ========== DESTRUCTOR ==========
	~MainMenu()
	{
		if (textureLoaded)
			UnloadTexture(menuBackground);
		if (overlayLoaded)
			UnloadTexture(menuOverlay);
		if (fontLoaded)
			UnloadFont(menuFont);
	}

	// ========== INITIALIZATION ==========
	void Initialize()
	{
		if (!textureLoaded)
		{
			menuBackground = LoadTexture(MAIN_MENU_IMAGE.c_str());
			textureLoaded = (menuBackground.id != 0);
		}

		if (!overlayLoaded)
		{
			Image menuImg = LoadImage(MENU_IMAGE.c_str());
			if (menuImg.data)
			{
				Color black = {0, 0, 0, 255};
				Color transparent = {0, 0, 0, 0};
				ImageColorReplace(&menuImg, black, transparent);

				menuOverlay = LoadTextureFromImage(menuImg);
				UnloadImage(menuImg);

				overlayLoaded = (menuOverlay.id != 0);
			}
		}

		// Load font once here
		if (!fontLoaded)
		{
			menuFont = LoadFont(CINZEL_DECORATIVE_BOLD_TTF.c_str());
			// Check if font loaded correctly (texture id > 0)
			fontLoaded = (menuFont.texture.id != 0);
		}
	}

	// ========== UPDATE ==========
	void Update()
	{
		startClicked = optionsClicked = creditsClicked = exitClicked = false;

		if (showOptionsMenu || showCreditsMenu)
		{
			if (IsRectangleClicked(closeButton))
			{
				showOptionsMenu = false;
				showCreditsMenu = false;
			}
			return;
		}

		if (IsRectangleClicked(startButton))
			startClicked = true;
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
			exitClicked = true;
	}

	// ========== DRAW ==========
	void Draw(bool showDebug = false)
	{
		if (!textureLoaded)
		{
			ClearBackground(BLACK);
			DrawText("MAIN MENU", 810, 400, 60, WHITE);
			DrawText("(Background image failed to load)", 700, 500, 20, RED);
			return;
		}

		Rectangle bgSource = {0, 0, (float)menuBackground.width, (float)menuBackground.height};
		Rectangle bgDest = {0, 0, 1920, 1080};
		DrawTexturePro(menuBackground, bgSource, bgDest, {0, 0}, 0.0f, WHITE);

		if (overlayLoaded)
		{
			Rectangle overlaySource = {0, 0, (float)menuOverlay.width, (float)menuOverlay.height};
			float overlayX = (1920 - MENU_OVERLAY_WIDTH) / 2 + MENU_OVERLAY_OFFSET_X;
			float overlayY = (1080 - MENU_OVERLAY_HEIGHT) / 2 + MENU_OVERLAY_OFFSET_Y;
			Rectangle overlayDest = {overlayX, overlayY, MENU_OVERLAY_WIDTH, MENU_OVERLAY_HEIGHT};

			DrawTexturePro(menuOverlay, overlaySource, overlayDest, {0, 0}, 0.0f, WHITE);
		}

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
