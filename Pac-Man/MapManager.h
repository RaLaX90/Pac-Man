#pragma once

#include <array>
#include <SFML/Graphics.hpp>

#include "Global.h"

class MapManager
{
	std::array<std::string, MAP_HEIGHT> map_sketch;

	std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> map;

	sf::Texture texture;

	void convert_sketch();
	
public:
	MapManager();
	~MapManager();

	std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> Get_map();

	unsigned int Get_pellets_count();
	std::array<Position, 4> Get_ghost_start_positions();
	Position Get_pacman_start_positions();

	void Draw_map(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window);
	
	void Draw_text(bool in_center, unsigned short x, unsigned short y, const std::string& drawing_text, sf::RenderWindow& window);
};