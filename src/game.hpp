#include <blue_samurai.hpp>
#include <constants.hpp>
#include <karasu_tengu.hpp>
#include <kitsune.hpp>
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
	Player player;
	Map currentMap;
	Camera2D camera;
	Texture2D background;
	std::vector<Enemy *> enemies;

	void init_enemies();
	void update_camera();

  public:
	Game();
	~Game();

	void run();
	void update();
	void draw();
};
