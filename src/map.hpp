#pragma once
#include <raylib.h>
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

	void load_map(const std::string &tileset_path, const std::string &csv_path);

	void draw();

	int get_tile_id(int grid_x, int grid_y) const;
	float get_tile_size() const { return tile_size * scale; }
	int get_width() const { return map_data.empty() ? 0 : (int)map_data[0].size(); }
	int get_height() const { return (int)map_data.size(); }
};
