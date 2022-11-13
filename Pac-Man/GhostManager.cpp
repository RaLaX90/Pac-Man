#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Global.h"
#include "Pacman.h"
#include "Ghost.h"
#include "GhostManager.h"

GhostManager::GhostManager(const std::array<Position, 4>& ghost_positions) :
	current_wave(0),
	wave_timer(LONG_SCATTER_DURATION),
	ghosts({
		Ghost(0, {ghost_positions[0].x, ghost_positions[0].y}, {ghost_positions[0].x, ghost_positions[0].y}),
		Ghost(1, {ghost_positions[1].x, ghost_positions[1].y}, {ghost_positions[0].x, ghost_positions[0].y}),
		Ghost(2, {ghost_positions[2].x, ghost_positions[2].y}, {ghost_positions[0].x, ghost_positions[0].y}),
		Ghost(3, {ghost_positions[3].x, ghost_positions[3].y}, {ghost_positions[0].x, ghost_positions[0].y}) })
{

}

void GhostManager::draw(bool flash, sf::RenderWindow& window)
{
	for (Ghost& ghost : ghosts)
	{
		ghost.draw(flash, window);
	}
}

void GhostManager::reset(unsigned char level/*, const std::array<Position, 4>& ghost_positions*/)
{
	current_wave = 0;

	//This is how we're increasing the difficulty.
	wave_timer = static_cast<unsigned short>(LONG_SCATTER_DURATION / pow(2, level));

	for (unsigned char a = 0; a < ghosts.size(); a++)
	{
		//ghosts[a].set_position(ghost_positions[a].x, ghost_positions[a].y);
		ghosts[a].reset();
	}

	//for (Ghost& ghost : ghosts)
	//{
	//	//We use the blue ghost to get the location of the house and the red ghost to get the location of the exit.
	//	ghost.reset(/*ghosts[2].get_position(), ghosts[0].get_position()*/);
	//}
}

void GhostManager::move(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map, Pacman& pacman)
{
	if (!pacman.get_energizer_timer()) //We won't move the wave timer when Pacman is energized.
	{
		if (!wave_timer)
		{
			if (7 > current_wave)
			{
				current_wave++;

				for (Ghost& ghost : ghosts)
				{
					ghost.switch_mode();
				}
			}

			//I took the rules from the website.
			if (current_wave % 2)
			{
				wave_timer = CHASE_DURATION;
			}
			else if (current_wave)
			{
				wave_timer = static_cast<unsigned short>(LONG_SCATTER_DURATION / pow(2, level));
			}
			else
			{
				wave_timer = static_cast<unsigned short>(SHORT_SCATTER_DURATION / pow(2, level));
			}
		}
		else
		{
			wave_timer--;
		}
	}

	for (Ghost& ghost : ghosts)
	{
		ghost.move(level, map, ghosts[0], pacman);
	}
}