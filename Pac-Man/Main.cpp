#include <array>
#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "Global.h"
#include "Pacman.h"
#include "Ghost.h"
#include "GhostManager.h"
#include "MapManager.h"

int main()
{
	bool game_won = false;

	//Used to make the game framerate-independent.
	unsigned lag = 0;

	unsigned char level = 0;

	//Similar to lag, used to make the game framerate-independent.
	std::chrono::time_point<std::chrono::steady_clock> previous_time;

	sf::Event event;

	sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);
	//Resizing the window.
	window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));

	//Generating a random seed.
	srand(static_cast<unsigned>(time(0)));

	MapManager mapManager{};

	std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map = mapManager.Get_map();


	GhostManager ghostManager{ mapManager.Get_ghost_start_positions() };

	ghostManager.reset(level);

	Pacman pacman{ mapManager.Get_pacman_start_positions() };

	unsigned int score = 0;

	//Get the current time and store it in a variable.
	previous_time = std::chrono::steady_clock::now();


	while (window.isOpen())
	{
		//Here we're calculating the lag.
		unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();

		lag += delta_time;

		previous_time += std::chrono::microseconds(delta_time);

		//While the lag exceeds the maximum allowed frame duration.
		while (FRAME_DURATION <= lag)
		{
			//We decrease the lag.
			lag -= FRAME_DURATION;

			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
				{
					//Making sure the player can close the window.
					window.close();
				}
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				//Making sure the player can close the window.
				window.close();
			}

			if (!game_won && !pacman.is_dead())
			{
				game_won = true;

				pacman.move(level, map);

				ghostManager.move_ghosts(level, map, pacman);

				//We're checking every cell in the map.
				for (const auto& column : map)
				{
					for (const auto& cell : column)
					{
						if (Cell::Pellet == cell) //And if at least one of them has a pellet.
						{
							game_won = false; //The game is not yet won.

							break;
						}
					}

					if (!game_won)
					{
						break;
					}
				}

				if (game_won)
				{
					pacman.set_animation_timer(0);
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //Restarting the game.
			{
				game_won = false;

				if (pacman.is_dead())
				{
					level = 0;
				}
				else
				{
					//After each win we reduce the duration of attack waves and energizers.
					level++;
				}

				map = mapManager.Get_map();

				ghostManager.reset(level);

				pacman.reset();
			}

			//I don't think anything needs to be explained here.
			if (FRAME_DURATION > lag)
			{
				window.clear();

				if (!game_won && !pacman.is_dead())
				{
					window.clear(sf::Color::Black);

					mapManager.Draw_map(map, window);

					ghostManager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window);

					mapManager.Draw_text(false, 0, CELL_SIZE * MAP_HEIGHT, "Level: " + std::to_string(1 + level), window);

					mapManager.Draw_text(false, MAP_WIDTH * 4, CELL_SIZE * MAP_HEIGHT, "Score: " + std::to_string(score), window);
				}

				pacman.draw(game_won, window);

				if (pacman.get_animation_over())
				{
					if (game_won)
					{
						mapManager.Draw_text(true, 0, 0, "Next level!", window);
					}
					else
					{
						mapManager.Draw_text(true, 0, 0, "Game over", window);
					}
				}

				window.display();

			}
		}
	}
}