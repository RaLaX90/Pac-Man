#include "MapCollision.h"

bool is_map_collision(bool is_collecting_pellets, bool is_using_door, float x, float y, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map)
{
	bool output = false;

	//Getting the exact position.
	float cell_x = x / static_cast<float>(CELL_SIZE);
	float cell_y = y / static_cast<float>(CELL_SIZE);

	//A ghost/Pacman can intersect 4 cells at most.
	for (unsigned char a = 0; a < 4; a++)
	{
		short i_x = 0;
		short i_y = 0;

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
			if (!is_collecting_pellets) //Here we only care about the walls.
			{
				if (Cell::Wall == map[i_y][i_x])
				{
					output = true;
				}
				else if (!is_using_door && Cell::Door == map[i_y][i_x])
				{
					output = true;
				}
			}
			else //Here we only care about the collectables.
			{
				if (Cell::Energizer == map[i_y][i_x])
				{
					output = true;

					map[i_y][i_x] = Cell::Empty;
				}
				else if (Cell::Pellet == map[i_y][i_x])
				{
					map[i_y][i_x] = Cell::Empty;
				}
			}
		}
	}

	return output;
}

Cell map_collision(float future_position_x, float future_position_y, Direction direction, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map)
{
	float cell_x = future_position_x / static_cast<float>(CELL_SIZE);
	float cell_y = future_position_y / static_cast<float>(CELL_SIZE);

	size_t i_x = 0;
	size_t i_y = 0;

	//float lul1 = 0;
	//size_t debug1 = 0;

	//float lul2 = 0;
	//size_t debug2 = 0;

	switch (direction)
	{
	case 0: //Top right cell  right
	{
		i_x = static_cast<short>(ceil(cell_x));
		i_y = static_cast<short>(floor(cell_y));



		//lul1 = future_position_y / CELL_SIZE;
		//debug1 = floor(lul1);

		//lul2 = future_position_x / CELL_SIZE;
		//debug2 = ceil(lul2);

		break;
	}
	case 1: //Top left cell  up
	{
		i_x = static_cast<short>(floor(cell_x));
		i_y = static_cast<short>(floor(cell_y));

		//lul1 = future_position_y / CELL_SIZE;
		//debug1 = lul1;

		//lul2 = future_position_x / CELL_SIZE;
		//debug2 = lul2;

		break;
	}
	case 2: //Bottom left cell  left
	{
		i_x = static_cast<short>(floor(cell_x));
		i_y = static_cast<short>(ceil(cell_y));

		//lul1 = future_position_y / CELL_SIZE;
		//debug1 = lul1;

		//lul2 = future_position_x / CELL_SIZE;
		//debug2 = lul2;

		break;
	}
	case 3: //Bottom right cell  down
	{
		i_x = static_cast<short>(ceil(cell_x));
		i_y = static_cast<short>(ceil(cell_y));

		//lul1 = future_position_y / CELL_SIZE;
		//debug1 = lul1;

		//lul2 = future_position_x / CELL_SIZE;
		//debug2 = lul2;

		break;
	}
	}

	auto lul = map[i_y][i_x];
	return lul;
}

/*std::array<Cell, 4>*/bool map_collision2(float position_x, float position_y, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map)
{
	//std::array<Cell, 4> cells_around{};

	float cell_x = position_x / static_cast<float>(CELL_SIZE);
	float cell_y = position_y / static_cast<float>(CELL_SIZE);

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
		//if ((0 <= i_x && i_x < MAP_WIDTH) && (0 <= i_y && i_y < MAP_HEIGHT))
		//{
		//	cells_around[a] = map[i_y][i_x];
		//}

		if (Cell::Wall == map[i_y][i_x])
		{
			return true;
		}
	}

	return false;
	//return cells_around;
}

bool is_in_cell_center(float position_x, float position_y) {
	auto debug = position_x / CELL_SIZE;
	auto lul = debug == (int)abs(debug);

	auto debug2 = position_y / CELL_SIZE;
	auto lul2 = debug2 == (int)abs(debug2);

	return lul && lul2;
}