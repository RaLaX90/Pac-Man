#pragma once

#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Global.h"
#include "Pacman.h"
#include "MapCollision.h"
#include "Game_object.h"

class Ghost : public Game_object
{
	//It can be the scatter mode or the chase mode.
	bool fear_mode;

	bool is_can_use_door;

	//0 - I'm not frightened
	//1 - Okay, maybe I am
	//2 - AAAAAAAH!!! I'M GOING TO MY HOUSE!
	unsigned char frightened_mode;
	//To make the ghost move more slowly, we'll move it after a certain number of frames. So we need a timer.
	unsigned char frightened_speed_timer;
	//0 - Red
	//1 - Pink
	//2 - Blue (It's actually cyan, but the website said it's blue.)
	//3 - Orange
	unsigned char id;

	//You can't stay in your house forever.
	Position door_position;

	Position target_position;

public:
	Ghost(unsigned char i_id, const Position& i_start_position, const Position& exit_position);
	virtual ~Ghost();

	//bool is_pacman_collision(const Position& pacman_position);

	float get_target_distance(unsigned char direction);

	void draw(bool flash, sf::RenderWindow& window);
	void reset();
	void set_position(float x, float y);
	void switch_mode();
	void move(unsigned char level, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, Ghost& ghost_0, Pacman& pacman);
	void update_target(unsigned char pacman_direction, const Position& ghost_0_position, const Position& pacman_position);

	bool is_frightened();
	Position get_position();
};