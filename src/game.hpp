#include <blue_samurai.hpp>
#include <constants.hpp>
#include <dialogue.hpp>
#include <karasu_tengu.hpp>
#include <kitsune.hpp>
#include <main_menu.hpp>
#include <map.hpp>
#include <player.hpp>
#include <purple_knight.hpp>
#include <resources.hpp>
#include <silver_knight.hpp>
#include <skeleton_spearman.hpp>
#include <skeleton_warrior.hpp>
#include <vector>
#include <yamabushi.hpp>

class Game
{
  private:
	Font font;
	Player player;
	Map currentMap;
	bool shouldExit;
	Camera2D camera;
	MainMenu mainMenu;
	Texture2D winScreen;
	Texture2D loseScreen;
	Texture2D background;
	Music backgroundMusic;
	GameState currentState;
	DialogueBox dialogueBox;
	GameState previousState;
	std::vector<Enemy *> enemies;
	std::string storedDialogueFile;
	int counter;

	void init_enemies();
	void update_camera();
	void update_main_menu();
	void update_gameplay();
	void update_dialogue();
	void update_paused();
	void update_game_win();
	void update_game_over();

	void draw_paused();
	void draw_gameplay();
	void draw_dialogue();
	void draw_main_menu();
	void draw_game_win();
	void draw_game_over();

	void reset_game();

  public:
	Game();
	~Game();

	void run();
	void update();
	void draw();
};
