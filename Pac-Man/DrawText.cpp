#include <cmath>
#include <SFML/Graphics.hpp>

#include "DrawText.h"
#include "Global.h"

void draw_text(bool in_center, unsigned short x, unsigned short y, const std::string& drawing_text, sf::RenderWindow& window)
{
	short character_x = x;
	short character_y = y;

	unsigned char character_width;

	sf::Sprite character_sprite;

	sf::Texture font_texture;
	font_texture.loadFromFile("Images/Font.png");

	//There are 96 characters in the font texture.
	character_width = font_texture.getSize().x / 96;

	character_sprite.setTexture(font_texture);

	if (in_center)
	{
		//I spent HOURS trying to make this work.
		//If you know any better way of doing this, please tell me.
		character_x = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_WIDTH - character_width * drawing_text.substr(0, drawing_text.find_first_of('\n')).size())));
		character_y = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_HEIGHT - FONT_HEIGHT * (1 + std::count(drawing_text.begin(), drawing_text.end(), '\n')))));
	}

	for (std::string::const_iterator a = drawing_text.begin(); a != drawing_text.end(); a++)
	{
		if ('\n' == *a)
		{
			if (in_center)
			{
				//I still don't understand what I wrote here.
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