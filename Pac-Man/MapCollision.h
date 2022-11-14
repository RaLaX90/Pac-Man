#pragma once

#include <array>
#include <cmath>

#include "Global.h"

bool is_map_collision(bool is_collecting_pellets, bool is_using_door, float x, float y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map);

Cell map_collision(float future_position_x, float future_position_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map);