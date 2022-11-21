#include "MapManager.h"
#include "Maps.h"

void MapManager::convert_sketch() {

	for (unsigned char a = 0; a < MAP_HEIGHT; a++)
	{
		for (unsigned char b = 0; b < MAP_WIDTH; b++)
		{
			switch (map_sketch[a][b])
			{
			case '#':
			{
				map[a][b] = Cell::Wall;

				break;
			}
			case '=':
			{
				map[a][b] = Cell::Door;

				break;
			}
			case '.':
			{
				map[a][b] = Cell::Pellet;

				break;
			}
			case 'o':
			{
				map[a][b] = Cell::Energizer;

				break;
			}
			case 'T':
			{
				map[a][b] = Cell::Tunnel;

				break;
			}
			default: {
				map[a][b] = Cell::Empty;

				break;
			}
			}
		}
	}
}

MapManager::MapManager()
{
	reset();

	texture.loadFromFile("Images/Map" + std::to_string(CELL_SIZE) + ".png");
}

MapManager::~MapManager()
{
}

std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& MapManager::Get_map()
{
	return map;
}

void MapManager::reset(unsigned char level)
{
	Change_map(level);

	convert_sketch();
}

void MapManager::Draw_map(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window)
{
	sf::Sprite sprite;
	sprite.setTexture(texture);

	for (unsigned char a = 0; a < MAP_HEIGHT; a++)
	{
		for (unsigned char b = 0; b < MAP_WIDTH; b++)
		{
			sprite.setPosition(CELL_SIZE * b, CELL_SIZE * a); // it is necessary

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

				if ((b > 0 && map[a][b - 1] == Cell::Wall) ||  //If previous cell is wall, than note it 
					(b == 0 && ((a > 0 && map[a - 1][b] == Cell::Tunnel) ||  //Or if in left side of map and cell below is tunnel, than draw an unfinished wall 
						(a < MAP_HEIGHT - 1 && map[a + 1][b] == Cell::Tunnel)))) //(since the cell above and below is a tunnel that leads to the other side of the map)
				{
					left = 1;
				}

				if (((b < MAP_WIDTH - 1) && map[a][b + 1] == Cell::Wall) || //If next cell is wall, than note it 
					(b == MAP_WIDTH - 1 && ((a > 0 && map[a - 1][b] == Cell::Tunnel) || // The same history with tunnel wals in right side
						(a < MAP_HEIGHT - 1 && map[a + 1][b] == Cell::Tunnel))))
				{
					right = 1;
				}

				if (a > 0 && map[a - 1][b] == Cell::Wall || //If next cell is wall, than note it 
					(a == 0 && ((b > 0 && map[a][b - 1] == Cell::Tunnel) || // The same history with tunnel wals in right side
						(b < MAP_WIDTH - 1 && map[a][b + 1] == Cell::Tunnel)))) //If cell above is wall, than note it 
				{
					up = 1;
				}

				if ((a < MAP_HEIGHT - 1) && map[a + 1][b] == Cell::Wall || //If next cell is wall, than note it 
					(a == MAP_HEIGHT - 1 && ((b > 0 && map[a][b - 1] == Cell::Tunnel) || // The same history with tunnel wals in right side
						(b < MAP_WIDTH - 1 && map[a][b + 1] == Cell::Tunnel)))) //If cell below is wall, than note it
				{
					down = 1;
				}

				//--------------------------------------------<         DISTRIBUTIVE PROPERTY!         >----------------------------
				sprite.setTextureRect(sf::IntRect(CELL_SIZE * (down + 2 * (left + 2 * (right + 2 * up))), 0, CELL_SIZE, CELL_SIZE));

				window.draw(sprite);

				break;
			}
			case Cell::Tunnel:
			{

				break;
			}
			default:
			{

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

	for (const auto& column : map)
	{
		for (const auto& cell : column)
		{
			switch (cell)
			{
			case Cell::Pellet:
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

Position MapManager::Get_door_position()
{
	Position door_positions{};

	for (unsigned char a = 0; a < MAP_HEIGHT; a++)
	{
		for (unsigned char b = 0; b < MAP_WIDTH; b++)
		{
			switch (map_sketch[a][b])
			{
			case '=':
			{
				door_positions.x = CELL_SIZE * b;
				door_positions.y = (CELL_SIZE * a) - CELL_SIZE; // Get exit position insead door position TODO

				return door_positions;
			}
			default: {
				break;
			}
			}
		}
	}

	return door_positions;
}

void MapManager::Change_map(unsigned char level) {
	if (level < 4) { //TODO (make max maps count dynamic)
		map_sketch = maps[level];
	}
	else {
		map_sketch = maps[4];
	}
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

	return pacman_start_position; //If pacman start position is not be founded - return object with 0
}