#pragma once

#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Global.h"
#include "MapCollision.h"
#include "Game_object.h"

class Pacman : public Game_object
{
	//This is used for the death animation.
	bool animation_over;
	
	bool dead;

	sf::Texture texture_dead;
	sf::Texture texture_alive;

	unsigned short energizer_timer;


public:
	Pacman(Position i_start_position);
	virtual ~Pacman();

	bool get_animation_over();
	bool is_dead();

	Direction get_direction();

	unsigned short get_energizer_timer();

	void draw(Game_mode mode, sf::RenderWindow& window);
	void reset();
	void set_animation_timer(unsigned short i_animation_timer);
	void set_dead(bool is_dead);
	void set_position(float new_position_x, float new_position_y);
	void move(unsigned char level, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map);

	void set_start_position(const Position& i_start_position);
	Position get_position();
};