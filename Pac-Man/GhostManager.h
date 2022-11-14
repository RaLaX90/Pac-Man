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
	GhostManager(const std::array<Position, 4>& ghost_positions);
	~GhostManager();

	void draw(bool flash, sf::RenderWindow& window);
	void reset(unsigned char level);
	void move_ghosts(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map, Pacman& pacman);
};