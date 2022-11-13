#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Global.h"
#include "Pacman.h"
#include "Ghost.h"
#include "MapCollision.h"

Ghost::Ghost(unsigned char i_id, const Position& start_position, const Position& exit_position) :
	id(i_id),
	start_position(start_position),
	door_position(exit_position)
{

}

bool Ghost::pacman_collision(const Position& pacman_position)
{
	//I used the ADVANCED collision checking algorithm.
	//Only experts like me can understand this.
	if (position.x > pacman_position.x - CELL_SIZE && position.x < CELL_SIZE + pacman_position.x)
	{
		if (position.y > pacman_position.y - CELL_SIZE && position.y < CELL_SIZE + pacman_position.y)
		{
			return 1;
		}
	}

	return 0;
}

float Ghost::get_target_distance(unsigned char direction)
{
	short x = position.x;
	short y = position.y;

	//We'll imaginarily move the ghost in a given direction and calculate the distance to the target_position.
	switch (direction)
	{
	case 0:
	{
		x += GHOST_SPEED;

		break;
	}
	case 1:
	{
		y -= GHOST_SPEED;

		break;
	}
	case 2:
	{
		x -= GHOST_SPEED;

		break;
	}
	case 3:
	{
		y += GHOST_SPEED;
	}
	}

	//I used the Pythagoras' theorem.
	//Because I know math.
	//Are you impressed?
	return static_cast<float>(sqrt(pow(x - target_position.x, 2) + pow(y - target_position.y, 2)));
}

void Ghost::draw(bool flash, sf::RenderWindow& window)
{
	//Current frame of the animation.
	unsigned char body_frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(GHOST_ANIMATION_SPEED)));

	sf::Sprite body;
	sf::Sprite face;

	sf::Texture texture;
	texture.loadFromFile("Images/Ghost" + std::to_string(CELL_SIZE) + ".png");

	body.setTexture(texture);
	body.setPosition(position.x, position.y);
	//Animation is basically a quick display of similar images.
	//So that's what we're doing here.
	body.setTextureRect(sf::IntRect(CELL_SIZE * body_frame, 0, CELL_SIZE, CELL_SIZE));

	face.setTexture(texture);
	face.setPosition(position.x, position.y);

	//The "I'm not frightened" look.
	if (0 == frightened_mode)
	{
		switch (id)
		{
		case 0:
		{
			//Red color
			body.setColor(sf::Color(255, 0, 0));

			break;
		}
		case 1:
		{
			//Pink color
			body.setColor(sf::Color(255, 182, 255));

			break;
		}
		case 2:
		{
			//Cyan color (I still don't understand why they called it blue)
			body.setColor(sf::Color(0, 255, 255));

			break;
		}
		case 3:
		{
			//Orange color
			body.setColor(sf::Color(255, 182, 85));
		}
		}

		face.setTextureRect(sf::IntRect(CELL_SIZE * direction, CELL_SIZE, CELL_SIZE, CELL_SIZE));

		window.draw(body);
	}
	//The "Maybe I am frightened" look.
	else if (1 == frightened_mode)
	{
		body.setColor(sf::Color(36, 36, 255));
		//The face remains the same regardless of where the ghost is going right now.
		face.setTextureRect(sf::IntRect(4 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

		if (flash && 0 == body_frame % 2)
		{
			body.setColor(sf::Color(255, 255, 255));
			face.setColor(sf::Color(255, 0, 0));
		}
		else
		{
			body.setColor(sf::Color(36, 36, 255));
			face.setColor(sf::Color(255, 255, 255));
		}

		window.draw(body);
	}
	//The "AAAAH!!!! I'M OUTTA HERE!!!!" look.
	else
	{
		//We only draw the face because Pacman stole the body.
		face.setTextureRect(sf::IntRect(CELL_SIZE * direction, 2 * CELL_SIZE, CELL_SIZE, CELL_SIZE));
	}

	window.draw(face);

	//--------------------------------------<        This is to prevent overflowing.         >-
	animation_timer = (1 + animation_timer) % (GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED);
}

void Ghost::reset(/*const Position& start_position, const Position& exit_position*/)
{
	fear_mode = false;
	//Everyone can use the door except the red ghost.
	//Because I hate the red ghost.
	is_can_use_door = 0 < id;

	direction = 0;
	frightened_mode = 0;
	frightened_speed_timer = 0;

	animation_timer = 0;

	set_position(start_position.x, start_position.y);
	//start_position = start_position;
	//door_position = exit_position;
	target_position = door_position;
}

void Ghost::set_position(short x, short y)
{
	position = { x, y };
}

void Ghost::switch_mode()
{
	fear_mode = !fear_mode;
}

void Ghost::move(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map, Ghost& ghost_0, Pacman& pacman)
{
	//Can the ghost move?
	bool move = false;

	//If this is greater than 1, that means that the ghost has reached the intersection.
	//We don't consider the way back as an available way.
	unsigned char available_ways = 0;
	unsigned char speed = GHOST_SPEED;

	std::array<bool, 4> walls{};

	//Here the ghost starts and stops being frightened.
	if (0 == frightened_mode && pacman.get_energizer_timer() == ENERGIZER_DURATION / pow(2, level))
	{
		frightened_speed_timer = GHOST_FRIGHTENED_SPEED;

		frightened_mode = 1;
	}
	else if (0 == pacman.get_energizer_timer() && 1 == frightened_mode)
	{
		frightened_mode = 0;
	}

	//I used the modulo operator in case the ghost goes outside the grid.
	if (2 == frightened_mode && 0 == position.x % GHOST_ESCAPE_SPEED && 0 == position.y % GHOST_ESCAPE_SPEED)
	{
		speed = GHOST_ESCAPE_SPEED;
	}

	update_target(pacman.get_direction(), ghost_0.get_position(), pacman.get_position());

	//This is so clean! I could spend hours staring at it.
	walls[0] = is_map_collision(0, is_can_use_door, speed + position.x, position.y, map);
	walls[1] = is_map_collision(0, is_can_use_door, position.x, position.y - speed, map);
	walls[2] = is_map_collision(0, is_can_use_door, position.x - speed, position.y, map);
	walls[3] = is_map_collision(0, is_can_use_door, position.x, speed + position.y, map);

	if (1 != frightened_mode)
	{
		//I used 4 because using a number between 0 and 3 will make the ghost move in a direction it can't move.
		unsigned char optimal_direction = 4;

		//The ghost can move.
		move = 1;

		for (unsigned char a = 0; a < 4; a++)
		{
			//ghosts can't turn back! (Unless they really have to)
			if (a == (2 + direction) % 4)
			{
				continue;
			}
			else if (!walls[a])
			{
				if (4 == optimal_direction)
				{
					optimal_direction = a;
				}

				available_ways++;

				if (get_target_distance(a) < get_target_distance(optimal_direction))
				{
					//The optimal direction is the direction that's closest to the target_position.
					optimal_direction = a;
				}
			}
		}

		if (1 < available_ways)
		{
			//When the ghost is at the intersection, it chooses the optimal direction.
			direction = optimal_direction;
		}
		else
		{
			if (4 == optimal_direction)
			{
				//"Unless they have to" part.
				direction = (2 + direction) % 4;
			}
			else
			{
				direction = optimal_direction;
			}
		}
	}
	else
	{
		//I used rand() because I figured that we're only using randomness here, and there's no need to use a whole library for it.
		unsigned char random_direction = rand() % 4;

		if (0 == frightened_speed_timer)
		{
			//The ghost can move after a certain number of frames.
			move = 1;

			frightened_speed_timer = GHOST_FRIGHTENED_SPEED;

			for (unsigned char a = 0; a < 4; a++)
			{
				//They can't turn back even if they're frightened.
				if (a == (2 + direction) % 4)
				{
					continue;
				}
				else if (!walls[a])
				{
					available_ways++;
				}
			}

			if (0 < available_ways)
			{
				while (walls[random_direction] || random_direction == (2 + direction) % 4)
				{
					//We keep picking a random direction until we can use it.
					random_direction = rand() % 4;
				}

				direction = random_direction;
			}
			else
			{
				//If there's no other way, it turns back.
				direction = (2 + direction) % 4;
			}
		}
		else
		{
			frightened_speed_timer--;
		}
	}

	//If the ghost can move, we move it.
	if (move)
	{
		switch (direction)
		{
		case 0:
		{
			position.x += speed;

			break;
		}
		case 1:
		{
			position.y -= speed;

			break;
		}
		case 2:
		{
			position.x -= speed;

			break;
		}
		case 3:
		{
			position.y += speed;
		}
		}

		//Warping.
		//When the ghost leaves the map, we move it to the other side.
		if (-CELL_SIZE >= position.x)
		{
			position.x = CELL_SIZE * MAP_WIDTH - speed;
		}
		else if (position.x >= CELL_SIZE * MAP_WIDTH)
		{
			position.x = speed - CELL_SIZE;
		}
	}

	if (pacman_collision(pacman.get_position()))
	{
		if (!frightened_mode) //When the ghost is not frightened and collides with Pacman, we kill Pacman.
		{
			pacman.set_dead(1);
		}
		else //Otherwise, the ghost starts running towards the house.
		{
			is_can_use_door = 1;

			frightened_mode = 2;

			target_position = start_position;
		}
	}
}

void Ghost::update_target(unsigned char pacman_direction, const Position& ghost_0_position, const Position& pacman_position)
{
	if (is_can_use_door) //If the ghost can use the door.
	{
		if (position == target_position)
		{
			if (door_position == target_position) //If the ghost has reached the exit.
			{
				is_can_use_door = false; //It can no longer use the door.
			}
			else if (start_position == target_position) //If the ghost has reached its start_position.
			{
				frightened_mode = 0; //It stops being frightened.

				target_position = door_position; //And starts leaving the house.
			}
		}
	}
	else
	{
		if (!fear_mode) //The scatter mode
		{
			//Each ghost goes to the corner it's assigned to.
			switch (id)
			{
			case 0:
			{
				target_position = { CELL_SIZE * (MAP_WIDTH - 1), 0 };

				break;
			}
			case 1:
			{
				target_position = { 0, 0 };

				break;
			}
			case 2:
			{
				target_position = { CELL_SIZE * (MAP_WIDTH - 1), CELL_SIZE * (MAP_HEIGHT - 1) };

				break;
			}
			case 3:
			{
				target_position = { 0, CELL_SIZE * (MAP_HEIGHT - 1) };

				break;
			}
			default: {
				break;
			}
			}
		}
		else //The chase mode
		{
			switch (id)
			{
			case 0: //The red ghost will chase Pacman.
			{
				target_position = pacman_position;

				break;
			}
			case 1: //The pink ghost will chase the 4th cell in front of Pacman.
			{
				target_position = pacman_position;

				switch (pacman_direction)
				{
				case 0:
				{
					target_position.x += CELL_SIZE * GHOST_1_CHASE;

					break;
				}
				case 1:
				{
					target_position.y -= CELL_SIZE * GHOST_1_CHASE;

					break;
				}
				case 2:
				{
					target_position.x -= CELL_SIZE * GHOST_1_CHASE;

					break;
				}
				case 3:
				{
					target_position.y += CELL_SIZE * GHOST_1_CHASE;

					break;
				}
				default: {
					break;
				}
				}

				break;
			}
			case 2: //The blue ghost.
			{
				target_position = pacman_position;

				//Getting the second cell in front of Pacman.
				switch (pacman_direction)
				{
				case 0:
				{
					target_position.x += CELL_SIZE * GHOST_2_CHASE;

					break;
				}
				case 1:
				{
					target_position.y -= CELL_SIZE * GHOST_2_CHASE;

					break;
				}
				case 2:
				{
					target_position.x -= CELL_SIZE * GHOST_2_CHASE;

					break;
				}
				case 3:
				{
					target_position.y += CELL_SIZE * GHOST_2_CHASE;

					break;
				}
				}

				//We're sending a vector from the red ghost to the second cell in front of Pacman.
				//Then we're doubling it.
				target_position.x += target_position.x - ghost_0_position.x;
				target_position.y += target_position.y - ghost_0_position.y;

				break;
			}
			case 3: //The orange ghost will chase Pacman until it gets close to him. Then it'll switch to the scatter mode.
			{
				//Using the Pythagoras' theorem again.
				if (CELL_SIZE * GHOST_3_CHASE <= sqrt(pow(position.x - pacman_position.x, 2) + pow(position.y - pacman_position.y, 2)))
				{
					target_position = pacman_position;
				}
				else
				{
					target_position = { 0, CELL_SIZE * (MAP_HEIGHT - 1) };
				}
			}
			}
		}
	}
}

Position Ghost::get_position()
{
	return position;
}