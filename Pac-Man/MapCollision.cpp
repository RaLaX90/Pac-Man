#include "MapCollision.h"

bool is_wall_and_door_collision(const Position& future_position, const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, bool is_can_use_door)
{
	float cell_x = future_position.x / static_cast<float>(CELL_SIZE);
	float cell_y = future_position.y / static_cast<float>(CELL_SIZE);

	//A ghost/Pacman can intersect 4 cells at most.
	for (unsigned char a = 0; a < 4; a++)
	{
		size_t i_x = 0;
		size_t i_y = 0;

		switch (a)
		{
		case 0: //Top left cell
		{
			i_x = static_cast<short>(floor(cell_x));
			i_y = static_cast<short>(floor(cell_y));

			break;
		}
		case 1: //Top right cell
		{
			i_x = static_cast<short>(ceil(cell_x));
			i_y = static_cast<short>(floor(cell_y));

			break;
		}
		case 2: //Bottom left cell
		{
			i_x = static_cast<short>(floor(cell_x));
			i_y = static_cast<short>(ceil(cell_y));

			break;
		}
		case 3: //Bottom right cell
		{
			i_x = static_cast<short>(ceil(cell_x));
			i_y = static_cast<short>(ceil(cell_y));
		}
		}

		//Making sure that the position is inside the map.
		if ((0 <= i_x && i_x < MAP_WIDTH) && (0 <= i_y && i_y < MAP_HEIGHT))
		{
			if (map[i_y][i_x] == Cell::Wall || (map[i_y][i_x] == Cell::Door && !is_can_use_door))
			{
				return true;
			}
		}
	}

	return false;
}

bool is_in_cell_center(const Position& position) {

	float cell_x = position.x / CELL_SIZE;
	bool is_integer_cell_x = cell_x == (int)abs(cell_x);

	float cell_y = position.y / CELL_SIZE;
	bool is_integer_cell_y = cell_y == (int)abs(cell_y);

	return is_integer_cell_x && is_integer_cell_y;
}

bool is_collision(const Position& first_object_position, const Position& second_object_position)
{
	if (first_object_position.x > second_object_position.x - CELL_SIZE && first_object_position.x < CELL_SIZE + second_object_position.x)
	{
		if (first_object_position.y > second_object_position.y - CELL_SIZE && first_object_position.y < CELL_SIZE + second_object_position.y)
		{
			return true;
		}
	}

	return false;
}