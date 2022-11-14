#include "MapCollision.h"

bool is_map_collision(bool is_collecting_pellets, bool is_using_door, float x, float y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map)
{
	bool output = false;

	//Getting the exact position.
	float cell_x = x / static_cast<float>(CELL_SIZE);
	float cell_y = y / static_cast<float>(CELL_SIZE);

	//A ghost/Pacman can intersect 4 cells at most.
	for (unsigned char a = 0; a < 4; a++)
	{
		short x = 0;
		short y = 0;

		switch (a)
		{
		case 0: //Top left cell
		{
			x = static_cast<short>(floor(cell_x));
			y = static_cast<short>(floor(cell_y));

			break;
		}
		case 1: //Top right cell
		{
			x = static_cast<short>(ceil(cell_x));
			y = static_cast<short>(floor(cell_y));

			break;
		}
		case 2: //Bottom left cell
		{
			x = static_cast<short>(floor(cell_x));
			y = static_cast<short>(ceil(cell_y));

			break;
		}
		case 3: //Bottom right cell
		{
			x = static_cast<short>(ceil(cell_x));
			y = static_cast<short>(ceil(cell_y));
		}
		}

		//Making sure that the position is inside the map.
		if ((0 <= x && x < MAP_WIDTH) && (0 <= y && y < MAP_HEIGHT))
		{
			if (!is_collecting_pellets) //Here we only care about the walls.
			{
				if (Cell::Wall == map[x][y])
				{
					output = true;
				}
				else if (!is_using_door && Cell::Door == map[x][y])
				{
					output = true;
				}
			}
			else //Here we only care about the collectables.
			{
				if (Cell::Energizer == map[x][y])
				{
					output = true;

					map[x][y] = Cell::Empty;
				}
				else if (Cell::Pellet == map[x][y])
				{
					map[x][y] = Cell::Empty;
				}
			}
		}
	}

	return output;
}

Cell map_collision(float future_position_x, float future_position_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map)
{
	if (future_position_x < CELL_SIZE) {
		return map[0][future_position_y / CELL_SIZE];
	}
	else if (future_position_x > CELL_SIZE) {
		return map[MAP_WIDTH - 1][future_position_y / CELL_SIZE];
	}
	else if (future_position_y < future_position_y) {
		return map[future_position_x / CELL_SIZE][0];
	}

	return map[future_position_x / CELL_SIZE][future_position_y / CELL_SIZE];
}