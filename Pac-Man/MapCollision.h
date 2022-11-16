#pragma once

#include <array>
#include <cmath>

#include "Global.h"

bool is_map_collision(float future_position_x, float future_position_y, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map);

bool is_in_cell_center(float position_x, float position_y);