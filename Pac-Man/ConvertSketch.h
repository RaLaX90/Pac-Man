#pragma once

std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> convert_sketch(const std::array<std::string, MAP_HEIGHT>& map_sketch, std::array<Position, 4>& ghost_positions, Pacman& pacman);