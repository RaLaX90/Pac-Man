//#include "ConvertSketch.h"
//
//std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> convert_sketch(const std::array<std::string, MAP_HEIGHT>& map_sketch_level_1, std::array<Position, 4>& ghost_positions, Pacman& pacman)
//{
//	std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> output_map{};
//
//	for (unsigned char a = 0; a < MAP_HEIGHT; a++)
//	{
//		for (unsigned char b = 0; b < MAP_WIDTH; b++)
//		{
//			//By default, every cell is empty.
//			output_map[b][a] = Cell::Empty;
//
//			switch (map_sketch_level_1[a][b])
//			{
//			//#wall #obstacle #youcantgothroughme
//			case '#':
//			{
//				output_map[b][a] = Cell::Wall;
//
//				break;
//			}
//			case '=':
//			{
//				output_map[b][a] = Cell::Door;
//
//				break;
//			}
//			case '.':
//			{
//				output_map[b][a] = Cell::Pellet;
//
//				break;
//			}
//			//Red ghost
//			case '0':
//			{
//				ghost_positions[0].x = CELL_SIZE * b;
//				ghost_positions[0].y = CELL_SIZE * a;
//
//				break;
//			}
//			//Pink ghost
//			case '1':
//			{
//				ghost_positions[1].x = CELL_SIZE * b;
//				ghost_positions[1].y = CELL_SIZE * a;
//
//				break;
//			}
//			//Blue (cyan) ghost
//			case '2':
//			{
//				ghost_positions[2].x = CELL_SIZE * b;
//				ghost_positions[2].y = CELL_SIZE * a;
//
//				break;
//			}
//			//Orange ghost
//			case '3':
//			{
//				ghost_positions[3].x = CELL_SIZE * b;
//				ghost_positions[3].y = CELL_SIZE * a;
//
//				break;
//			}
//			//Pacman!
//			case 'P':
//			{
//				pacman.set_start_position({ CELL_SIZE * b, CELL_SIZE * a });
//
//				break;
//			}
//			//This looks like a surprised face.
//			case 'o':
//			{
//				output_map[b][a] = Cell::Energizer;
//			}
//			}
//		}
//	}
//
//	return output_map;
//}