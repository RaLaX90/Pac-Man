#include "Game_object.h"

Game_object::Game_object(const Position& i_start_position, float i_speed) :
	start_position(i_start_position),
	speed(i_speed)
{
}

Game_object::~Game_object()
{
}