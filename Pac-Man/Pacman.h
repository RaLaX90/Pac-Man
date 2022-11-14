#pragma once

#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Global.h"
#include "MapCollision.h"

class Pacman
{
	//This is used for the death animation.
	bool animation_over;
	
	bool dead;

	Position start_position;

	Direction direction;

	sf::Texture texture_dead;
	sf::Texture texture_alive;

	unsigned short animation_timer;
	unsigned short energizer_timer;

	Position position;

public:
	Pacman(Position i_start_position);

	bool get_animation_over();
	bool is_dead();

	Direction get_direction();

	unsigned short get_energizer_timer();

	void draw(bool is_victory, sf::RenderWindow& window);
	void reset();
	void set_animation_timer(unsigned short i_animation_timer);
	void set_dead(bool is_dead);
	void set_position(float x, float y);
	void move(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map);

	void set_start_position(const Position& i_start_position);
	Position get_position();
};