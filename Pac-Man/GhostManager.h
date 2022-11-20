#pragma once

#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Global.h"
#include "Pacman.h"
#include "Ghost.h"

class GhostManager
{
	//The ghosts will switch between the scatter mode and the chase mode before permanently chasing Pacman.
	//So we need this to keep track of the waves.
	unsigned char current_wave;

	unsigned short wave_timer;

	std::array<Ghost, 4> ghosts;

public:
	GhostManager(const std::array<Position, 4>& ghost_start_positions, const Position& door_position);
	~GhostManager();

	void draw(bool flash, sf::RenderWindow& window);
	void reset(unsigned char level = 0, const std::array<Position, 4>& ghost_start_positions = {}, const Position& door_position = {});
	void move_ghosts(unsigned char level, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, Pacman& pacman);

	std::array<Ghost, 4>& get_ghosts();
};