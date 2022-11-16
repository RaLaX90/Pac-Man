#include "GhostManager.h"

GhostManager::GhostManager(const std::array<Position, 4>& ghost_positions) :
	wave_timer(LONG_SCATTER_DURATION),
	ghosts({
		Ghost(0, {ghost_positions[0].x, ghost_positions[0].y}, {ghost_positions[0].x, ghost_positions[0].y}),
		Ghost(1, {ghost_positions[1].x, ghost_positions[1].y}, {ghost_positions[0].x, ghost_positions[0].y}),
		Ghost(2, {ghost_positions[2].x, ghost_positions[2].y}, {ghost_positions[0].x, ghost_positions[0].y}),
		Ghost(3, {ghost_positions[3].x, ghost_positions[3].y}, {ghost_positions[0].x, ghost_positions[0].y}) })
{

}

GhostManager::~GhostManager()
{
}

void GhostManager::draw(bool flash, sf::RenderWindow& window)
{
	for (Ghost& ghost : ghosts)
	{
		ghost.draw(flash, window);
	}
}

void GhostManager::reset(unsigned char level)
{
	current_wave = 0;

	//This is how we're increasing the difficulty.
	wave_timer = static_cast<unsigned short>(LONG_SCATTER_DURATION / pow(2, level));

	for (unsigned char a = 0; a < ghosts.size(); a++)
	{
		ghosts[a].reset();
	}
}

void GhostManager::move_ghosts(unsigned char level, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, Pacman& pacman)
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