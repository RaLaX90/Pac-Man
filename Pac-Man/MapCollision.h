#pragma once

#include <array>
#include <cmath>

#include "Global.h"

bool is_map_collision(bool is_collecting_pellets, bool is_using_door, float x, float y, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map);

Cell map_collision(float future_position_x, float future_position_y, Direction direction, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map);

/*std::array<Cell, 4>*/bool map_collision2(float position_x, float position_y, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map);

bool is_in_cell_center(float position_x, float position_y);