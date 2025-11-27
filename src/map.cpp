#include "map.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map(float scale_factor)
{
	scale = scale_factor;
	tile_size = 32;
	// Initialize texture to 0 so we know it's empty
	tileset = {0};
}

Map::~Map()
{
	// Safety check before unloading
	if (tileset.id != 0)
		UnloadTexture(tileset);
}

void Map::load_map(const std::string &tileset_path, const std::string &csv_path)
{
	// 1. CLEANUP: If we already have a map loaded, unload the old texture
	if (tileset.id != 0)
	{
		UnloadTexture(tileset);
	}
	map_data.clear();

	// 2. LOAD NEW ASSETS
	tileset = LoadTexture(tileset_path.c_str());
	SetTextureFilter(tileset, TEXTURE_FILTER_POINT);

	if (tileset.id == 0)
	{
		std::cerr << "Error: Could not load tileset " << tileset_path << std::endl;
		return;
	}

	// 3. PARSE CSV (Same logic as before)
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

	// Calculate tileset width (in tiles) to find texture coordinates
	int tileset_width_in_tiles = tileset.width / tile_size;

	for (int y = 0; y < map_data.size(); y++)
	{
		for (int x = 0; x < map_data[y].size(); x++)
		{
			int tile_id = map_data[y][x];

			// 0 means empty air
			if (tile_id == 0)
				continue;

			// Adjust ID: Your friend's editor uses 1-based indexing for tiles
			// 1 = First tile in image (0,0)
			int actual_id = tile_id - 1;

			// Source Rect (Where on the sprite sheet)
			float src_x = (actual_id % tileset_width_in_tiles) * tile_size;
			float src_y = (actual_id / (float)tileset_width_in_tiles) * tile_size;

			Rectangle source = {src_x, src_y, (float)tile_size, (float)tile_size};

			// Destination Rect (Where on screen)
			Rectangle dest = {x * tile_size * scale, y * tile_size * scale, tile_size * scale,
							  tile_size * scale};

			DrawTexturePro(tileset, source, dest, {0, 0}, 0.0f, WHITE);
		}
	}
}
