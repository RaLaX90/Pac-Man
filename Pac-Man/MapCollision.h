#pragma once

bool is_map_collision(bool is_collecting_pellets, bool is_using_door, short x, short y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map);