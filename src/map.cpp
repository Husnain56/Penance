#include "map.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map(float scale_factor)
{
	scale = scale_factor;
	tile_size = 16;
	tileset = {0};
}

Map::~Map()
{
	if (tileset.id != 0)
		UnloadTexture(tileset);
}

void Map::load_map(const std::string &tileset_path, const std::string &csv_path)
{
	if (tileset.id != 0)
	{
		UnloadTexture(tileset);
	}
	map_data.clear();

	tileset = LoadTexture(tileset_path.c_str());
	SetTextureFilter(tileset, TEXTURE_FILTER_POINT);

	if (tileset.id == 0)
	{
		std::cerr << "Error: Could not load tileset " << tileset_path << std::endl;
		return;
	}

	std::ifstream file(csv_path);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open map file " << csv_path << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::vector<int> row;
		std::stringstream ss(line);
		std::string value;

		while (std::getline(ss, value, ','))
		{
			try
			{
				row.push_back(std::stoi(value));
			}
			catch (...)
			{
				row.push_back(0);
			}
		}
		map_data.push_back(row);
	}

	file.close();
	std::cout << "Level loaded: " << csv_path << std::endl;
}

void Map::draw()
{
	if (map_data.empty())
		return;

	int tileset_width_in_tiles = tileset.width / tile_size;

	for (int y = 0; y < map_data.size(); y++)
	{
		for (int x = 0; x < map_data[y].size(); x++)
		{
			int tile_id = map_data[y][x];

			if (tile_id == 0)
				continue;

			int actual_id = tile_id - 1;

			float src_x = (actual_id % tileset_width_in_tiles) * tile_size;
			float src_y = (actual_id / (float)tileset_width_in_tiles) * tile_size;

			Rectangle source = {src_x, src_y, (float)tile_size, (float)tile_size};

			Rectangle dest = {x * tile_size * scale, y * tile_size * scale, tile_size * scale,
							  tile_size * scale};

			DrawTexturePro(tileset, source, dest, {0, 0}, 0.0f, WHITE);
		}
	}
}

int Map::get_tile_id(int grid_x, int grid_y) const
{
	if (map_data.empty())
		return 0;

	// Bounds check to prevent crashing
	if (grid_y < 0 || grid_y >= map_data.size())
		return 0;
	if (grid_x < 0 || grid_x >= map_data[0].size())
		return 0;

	return map_data[grid_y][grid_x];
}
