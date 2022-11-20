#pragma once

#include <array>
#include <cmath>

#include "Global.h"

bool is_wall_and_door_collision(const Position& future_position, const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, bool is_can_use_door = false);

bool is_in_cell_center(const Position& position);

bool is_collision(const Position& first_object_object, const Position& second_object_position);