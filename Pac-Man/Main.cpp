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
	State state = State::STATE_OK;

	//Used to make the game framerate-independent.
	unsigned lag = 0;

	unsigned char level = 0;

	sf::Event event;

	sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);
	//Resizing the window.
	window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));

	//Generating a random seed.
	srand(static_cast<unsigned>(time(0)));

	MapManager mapManager{};

	Pacman pacman{ mapManager.Get_pacman_start_positions() };

	GhostManager ghostManager{ mapManager.Get_ghost_start_positions(), mapManager.Get_door_position() };

	unsigned int max_pelleets_count = mapManager.Get_pellets_count();
	unsigned int score = 0;

	unsigned delta_time;
	//Similar to lag, used to make the game framerate-independent.
	//Get the current time and store it in a variable.
	std::chrono::time_point<std::chrono::steady_clock> previous_time = std::chrono::steady_clock::now();

	while (window.isOpen())
	{
		//Here we're calculating the lag.
		delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();

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

			if (state == State::STATE_OK)
			{
				pacman.move(level, mapManager.Get_map());

				ghostManager.move_ghosts(level, mapManager.Get_map(), pacman);

				for (Ghost& ghost : ghostManager.get_ghosts()) {
					if (is_collision(pacman.get_position(), ghost.get_position()) && !ghost.is_frightened()) {
						//if (is_in_cell_center(pacman.get_position()) && is_in_cell_center(ghost.get_position())) { //Correct game mechanics, but too easy to play
							state = State::STATE_PACMAN_DEAD;

							break;
						//}
					}
				}

				score = (max_pelleets_count - mapManager.Get_pellets_count()) * 10; //TODO (make it more optimized)

				if (mapManager.Get_pellets_count() == 0) { //TODO (make it more optimized)
					state = State::STATE_ALL_PELLETS_COLLECTED;
				}

				//if (state == State::All_pellets_collected) // TODO (useless i think:))
				//{
				//	pacman.set_animation_timer(0);
				//}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //Restarting the game.
			{
				if (state == State::STATE_PACMAN_DEAD)
				{
					level = 0;
				}
				else
				{
					//After each win we reduce the duration of attack waves, energizers and maps.
					level++;
				}

				state = State::STATE_OK;

				mapManager.reset(level);

				ghostManager.reset(level, mapManager.Get_ghost_start_positions(), mapManager.Get_door_position());	//TODO (can be optimized: 
																													//do not update start position, if map is the same)

				pacman.reset(mapManager.Get_pacman_start_positions());
			}

			if (FRAME_DURATION > lag)
			{
				window.clear(sf::Color::Black);

				pacman.draw(state, window);

				if (state == State::STATE_OK)
				{
					mapManager.Draw_map(mapManager.Get_map(), window);

					ghostManager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window);

					mapManager.Draw_text(false, 0, CELL_SIZE * MAP_HEIGHT, "Level: " + std::to_string(1 + level), window);

					mapManager.Draw_text(false, MAP_WIDTH * 4, CELL_SIZE * MAP_HEIGHT, "Score: " + std::to_string(score), window);
				}

				if (pacman.get_animation_over())
				{
					if (state == State::STATE_ALL_PELLETS_COLLECTED)
					{
						mapManager.Draw_text(true, 0, 0, "Next level!\n\n\nPress enter", window);
					}
					else if (state == State::STATE_PACMAN_DEAD)
					{
						mapManager.Draw_text(true, 0, 0, "Game over\n\n\nPress enter", window);
					}
				}

				window.display();
			}
		}
	}
}