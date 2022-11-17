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
	Game_mode game_result = Game_mode::Continue;

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

	//std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> map = mapManager.Get_map();

	Pacman pacman{ mapManager.Get_pacman_start_positions() };

	GhostManager ghostManager{ mapManager.Get_ghost_start_positions() };

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
				game_result = Game_mode::Pacman_dead;
				//window.close();
			}

			if (game_result == Game_mode::Continue)
			{
				pacman.move(level, mapManager.Get_map());

				ghostManager.move_ghosts(level, mapManager.Get_map(), pacman);

				for (Ghost& ghost : ghostManager.get_ghosts()) {
					if (!ghost.is_frightened() && is_collision(pacman.get_position(), ghost.get_position())) {
						game_result = Game_mode::Pacman_dead;

						break;
					}
				}

				//We're checking every cell in the map.
				if (mapManager.Get_pellets_count() == 0) { //TODO (make it more optimized)
					game_result = Game_mode::All_pellets_collected;
				}

				//if (game_result == Game_mode::All_pellets_collected) // TODO (useless i think:))
				//{
				//	pacman.set_animation_timer(0);
				//}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //Restarting the game.
			{
				if (game_result == Game_mode::Pacman_dead)
				{
					level = 0;
				}
				else
				{
					//After each win we reduce the duration of attack waves and energizers.
					level++;
				}

				game_result = Game_mode::Continue;

				mapManager.reset();

				ghostManager.reset(level);

				pacman.reset();
			}

			if (FRAME_DURATION > lag)
			{
				window.clear(sf::Color::Black);

				pacman.draw(game_result, window);

				if (game_result == Game_mode::Continue)
				{
					mapManager.Draw_map(mapManager.Get_map(), window);

					ghostManager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window);

					mapManager.Draw_text(false, 0, CELL_SIZE * MAP_HEIGHT, "Level: " + std::to_string(1 + level), window);

					mapManager.Draw_text(false, MAP_WIDTH * 4, CELL_SIZE * MAP_HEIGHT, "Score: " + std::to_string(score), window);
				}

				if (pacman.get_animation_over())
				{
					if (game_result == Game_mode::All_pellets_collected)
					{
						mapManager.Draw_text(true, 0, 0, "Next level!", window);
					}
					else if (game_result == Game_mode::Pacman_dead)
					{
						mapManager.Draw_text(true, 0, 0, "Game over", window);
					}
				}

				window.display();
			}
		}
	}
}