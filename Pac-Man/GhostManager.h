#pragma once

class GhostManager
{
	//The ghosts will switch between the scatter mode and the chase mode before permanently chasing Pacman.
	//So we need this to keep track of the waves.
	unsigned char current_wave;

	//Damn, I really used a lot of timers.
	unsigned short wave_timer;

	std::array<Ghost, 4> ghosts;
public:
	GhostManager(const std::array<Position, 4>& ghost_positions);

	void draw(bool flash, sf::RenderWindow& window);
	void reset(unsigned char level/*, const std::array<Position, 4>& ghost_positions*/);
	void move(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map, Pacman& pacman);
};