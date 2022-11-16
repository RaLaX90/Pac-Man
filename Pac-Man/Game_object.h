#pragma once

#include "Global.h"

class Game_object
{
protected:
	Direction direction;

	Position start_position;

	Position position;

	unsigned short animation_timer;

public:
	Game_object(const Position& i_start_position);
	virtual ~Game_object();
};