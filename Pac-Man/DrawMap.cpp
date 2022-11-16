//#include "DrawMap.h"
//
//void draw_map(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window)
//{
//	sf::Sprite sprite;
//
//	sf::Texture texture;
//	texture.loadFromFile("Images/Map" + std::to_string(CELL_SIZE) + ".png");
//
//	sprite.setTexture(texture);
//
//	for (unsigned char a = 0; a < MAP_WIDTH; a++)
//	{
//		for (unsigned char b = 0; b < MAP_HEIGHT; b++)
//		{
//			sprite.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));
//
//			//We just crop out what we need from the texture.
//			switch (map[a][b])
//			{
//			case Cell::Door:
//			{
//				sprite.setTextureRect(sf::IntRect(2 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));
//
//				window.draw(sprite);
//
//				break;
//			}
//			case Cell::Energizer:
//			{
//				sprite.setTextureRect(sf::IntRect(CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));
//
//				window.draw(sprite);
//
//				break;
//			}
//			case Cell::Pellet:
//			{
//				sprite.setTextureRect(sf::IntRect(0, CELL_SIZE, CELL_SIZE, CELL_SIZE));
//
//				window.draw(sprite);
//
//				break;
//			}
//			case Cell::Wall:
//			{
//				unsigned char down = 0;
//				unsigned char left = 0;
//				unsigned char right = 0;
//				unsigned char up = 0;
//
//				if (b < MAP_HEIGHT - 1)
//				{
//					if (Cell::Wall == map[a][1 + b])
//					{
//						down = 1;
//					}
//				}
//
//				//Since we have warp tunnels, we need to draw them as if they're connected.
//				if (0 < a)
//				{
//					if (Cell::Wall == map[a - 1][b])
//					{
//						left = 1;
//					}
//				}
//				else
//				{
//					left = 1;
//				}
//
//				if (a < MAP_WIDTH - 1)
//				{
//					if (Cell::Wall == map[1 + a][b])
//					{
//						right = 1;
//					}
//				}
//				else
//				{
//					right = 1;
//				}
//
//				if (0 < b)
//				{
//					if (Cell::Wall == map[a][b - 1])
//					{
//						up = 1;
//					}
//				}
//
//				//--------------------------------------------<         DISTRIBUTIVE PROPERTY!         >----------------------------
//				sprite.setTextureRect(sf::IntRect(CELL_SIZE * (down + 2 * (left + 2 * (right + 2 * up))), 0, CELL_SIZE, CELL_SIZE));
//
//				window.draw(sprite);
//
//				break;
//			}
//			default: {
//				break;
//			}
//			}
//		}
//	}
//}