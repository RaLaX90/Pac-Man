#pragma once

#include "Global.h"

class Game_object
{
protected: //Use protected mode so this properties of the child classes was in private access
	Direction direction;

	Position start_position;

	Position position;

	unsigned short animation_timer;

	float speed;

public:
	Game_object(const Position& i_start_position, float i_speed);
	virtual ~Game_object();
};