#pragma once
#include "raylib.h"
#include <string>
#include <vector>

class Map
{
  private:
	Texture2D tileset;
	float scale;
	int tile_size;
	std::vector<std::vector<int>> map_data;

  public:
	Map(float scale_factor);
	~Map();

	// UPDATED: Now takes the image path AND the data path
	void load_map(const std::string &tileset_path, const std::string &csv_path);

	void draw();

	// Optional: Helper to check if a map is currently loaded
	bool is_ready() const { return !map_data.empty(); }
};
