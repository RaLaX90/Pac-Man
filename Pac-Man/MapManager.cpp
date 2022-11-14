#include "MapManager.h"

void MapManager::convert_sketch() {

	for (unsigned char a = 0; a < MAP_HEIGHT; a++)
	{
		for (unsigned char b = 0; b < MAP_WIDTH; b++)
		{
			switch (map_sketch[a][b])
			{
			case '#':
			{
				map[b][a] = Cell::Wall;

				break;
			}
			case '=':
			{
				map[b][a] = Cell::Door;

				break;
			}
			case '.':
			{
				map[b][a] = Cell::Pellet;

				break;
			}
			case 'o':
			{
				map[b][a] = Cell::Energizer;

				break;
			}
			case 'T':
			{
				map[b][a] = Cell::Tunnel;

				break;
			}
			default: {
				map[b][a] = Cell::Empty;

				break;
			}
			}
		}
	}
}

MapManager::MapManager() {
	map_sketch = {
		" ################### ",
		" #........#........# ",
		" #o##.###.#.###.##o# ",
		" #.................# ",
		" #.##.#.#####.#.##.# ",
		" #....#...#...#....# ",
		" ####.### # ###.#### ",
		"    #.#   0   #.#    ",
		"#####.# ##=## #.#####",
		"TTTTT.  #123#  .TTTTT",
		"#####.# ##### #.#####",
		"    #.#       #.#    ",
		" ####.# ##### #.#### ",
		" #........#........# ",
		" #.##.###.#.###.##.# ",
		" #o.#.....P.....#.o# ",
		" ##.#.#.#####.#.#.## ",
		" #....#...#...#....# ",
		" #.######.#.######.# ",
		" #.................# ",
		" ################### "
	};

	convert_sketch();

	texture.loadFromFile("Images/Map" + std::to_string(CELL_SIZE) + ".png");
}

MapManager::~MapManager()
{
}

std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> MapManager::Get_map()
{
	return map;
}

void MapManager::Draw_map(const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map, sf::RenderWindow& window)
{
	sf::Sprite sprite;
	sprite.setTexture(texture);

	for (unsigned char a = 0; a < MAP_WIDTH; a++)
	{
		for (unsigned char b = 0; b < MAP_HEIGHT; b++)
		{
			sprite.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));

			//We just crop out what we need from the texture.
			switch (map[a][b])
			{
			case Cell::Door:
			{
				sprite.setTextureRect(sf::IntRect(2 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

				window.draw(sprite);

				break;
			}
			case Cell::Energizer:
			{
				sprite.setTextureRect(sf::IntRect(CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

				window.draw(sprite);

				break;
			}
			case Cell::Pellet:
			{
				sprite.setTextureRect(sf::IntRect(0, CELL_SIZE, CELL_SIZE, CELL_SIZE));

				window.draw(sprite);

				break;
			}
			case Cell::Wall:
			{
				unsigned char down = 0;
				unsigned char left = 0;
				unsigned char right = 0;
				unsigned char up = 0;

				if (b < MAP_HEIGHT - 1)
				{
					if (Cell::Wall == map[a][1 + b])
					{
						down = 1;
					}
				}

				//Since we have warp tunnels, we need to draw them as if they're connected.
				if (0 < a)
				{
					if (Cell::Wall == map[a - 1][b])
					{
						left = 1;
					}
				}
				else
				{
					left = 1;
				}

				if (a < MAP_WIDTH - 1)
				{
					if (Cell::Wall == map[1 + a][b])
					{
						right = 1;
					}
				}
				else
				{
					right = 1;
				}

				if (0 < b)
				{
					if (Cell::Wall == map[a][b - 1])
					{
						up = 1;
					}
				}

				//--------------------------------------------<         DISTRIBUTIVE PROPERTY!         >----------------------------
				sprite.setTextureRect(sf::IntRect(CELL_SIZE * (down + 2 * (left + 2 * (right + 2 * up))), 0, CELL_SIZE, CELL_SIZE));

				window.draw(sprite);

				break;
			}
			default: {
				break;
			}
			}
		}
	}
}

void MapManager::Draw_text(bool in_center, unsigned short x, unsigned short y, const std::string& drawing_text, sf::RenderWindow& window)
{
	short character_x = x;
	short character_y = y;

	unsigned char character_width;

	sf::Sprite character_sprite;

	sf::Texture font_texture;
	font_texture.loadFromFile("Images/Font.png"); //TODO (can move it to MapManager property)

	//There are 96 characters in the font texture.
	character_width = font_texture.getSize().x / 96;

	character_sprite.setTexture(font_texture);

	if (in_center)
	{
		character_x = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_WIDTH - character_width * drawing_text.substr(0, drawing_text.find_first_of('\n')).size())));
		character_y = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_HEIGHT - FONT_HEIGHT * (1 + std::count(drawing_text.begin(), drawing_text.end(), '\n')))));
	}

	for (std::string::const_iterator a = drawing_text.begin(); a != drawing_text.end(); a++)
	{
		if ('\n' == *a)
		{
			if (in_center)
			{
				character_x = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_WIDTH - character_width * drawing_text.substr(1 + a - drawing_text.begin(), drawing_text.find_first_of('\n', 1 + a - drawing_text.begin()) - (1 + a - drawing_text.begin())).size())));
			}
			else
			{
				character_x = x;
			}

			character_y += FONT_HEIGHT;

			continue;
		}

		character_sprite.setPosition(character_x, character_y);
		//The font texture begins with a space character, which is the 32nd character.
		character_sprite.setTextureRect(sf::IntRect(character_width * (*a - 32), 0, character_width, FONT_HEIGHT));

		character_x += character_width;

		window.draw(character_sprite);
	}
}

unsigned int MapManager::Get_pellets_count()
{
	unsigned int pellets_count = 0;

	for (const auto& column : map_sketch)
	{
		for (const auto& symbol : column)
		{
			switch (symbol)
			{
			case '.':
			{
				pellets_count++;

				break;
			}
			default: {
				break;
			}
			}
		}
	}

	return pellets_count;
}

std::array<Position, 4> MapManager::Get_ghost_start_positions()
{
	std::array<Position, 4> ghost_start_positions{};

	for (unsigned char a = 0; a < MAP_HEIGHT; a++)
	{
		for (unsigned char b = 0; b < MAP_WIDTH; b++)
		{
			switch (map_sketch[a][b])
			{
				//Red ghost
			case '0':
			{
				ghost_start_positions[0].x = CELL_SIZE * b;
				ghost_start_positions[0].y = CELL_SIZE * a;

				break;
			}
			//Pink ghost
			case '1':
			{
				ghost_start_positions[1].x = CELL_SIZE * b;
				ghost_start_positions[1].y = CELL_SIZE * a;

				break;
			}
			//Blue (cyan) ghost
			case '2':
			{
				ghost_start_positions[2].x = CELL_SIZE * b;
				ghost_start_positions[2].y = CELL_SIZE * a;

				break;
			}
			//Orange ghost
			case '3':
			{
				ghost_start_positions[3].x = CELL_SIZE * b;
				ghost_start_positions[3].y = CELL_SIZE * a;

				break;
			}
			default: {
				break;
			}
			}
		}
	}

	return ghost_start_positions;
}

Position MapManager::Get_pacman_start_positions()
{
	Position pacman_start_position{};

	for (unsigned char a = 0; a < MAP_HEIGHT; a++)
	{
		for (unsigned char b = 0; b < MAP_WIDTH; b++)
		{
			switch (map_sketch[a][b])
			{
			case 'P':
			{
				pacman_start_position.x = CELL_SIZE * b;
				pacman_start_position.y = CELL_SIZE * a;

				return pacman_start_position;
			}
			default: {
				break;
			}
			}
		}
	}

	return pacman_start_position;
}