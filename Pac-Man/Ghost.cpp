#include "Ghost.h"

Ghost::Ghost(unsigned char i_id, const Position& i_start_position, const Position& door_position) :
	id(i_id),
	Game_object(i_start_position, GHOST_SPEED),
	door_position(door_position)
{
	reset();
}

Ghost::~Ghost()
{
}

float Ghost::get_target_distance(unsigned char direction)
{
	float x = position.x;
	float y = position.y;

	//We'll imaginarily move the ghost in a given direction and calculate the distance to the target position.
	switch (direction)
	{
	case 0:
	{
		x += speed;

		break;
	}
	case 1:
	{
		y -= speed;

		break;
	}
	case 2:
	{
		x -= speed;

		break;
	}
	case 3:
	{
		y += speed;
	}
	}

	//Used the Pythagoras' theorem.
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

void Ghost::reset(const Position& i_start_position, const Position& door_position)
{
	fear_mode = false;
	//Everyone can use the door except the red ghost.
	is_can_use_door = 0 < id;

	direction = Direction::Right;
	frightened_mode = 0;
	frightened_speed_timer = 0;
	animation_timer = 0;

	target_position = door_position;

	if (i_start_position.x != 0 && i_start_position.y != 0) {
		set_start_position(i_start_position);
		set_door_position(door_position);
	}

	set_position(start_position.x, start_position.y);
}

void Ghost::set_position(float x, float y)
{
	position = { x, y };
}

void Ghost::switch_mode()
{
	fear_mode = !fear_mode;
}

void Ghost::move(unsigned char level, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, Ghost& ghost_0, Pacman& pacman)
{
	//Can the ghost move?
	bool move = false;

	//If this is greater than 1, that means that the ghost has reached the intersection.
	//We don't consider the way back as an available way.
	unsigned char available_ways = 0;

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
	if (2 == frightened_mode && 0 == (int)position.x % GHOST_ESCAPE_SPEED && 0 == (int)position.y % GHOST_ESCAPE_SPEED)
	{
		speed = GHOST_ESCAPE_SPEED;
	}

	update_target(pacman.get_direction(), ghost_0.get_position(), pacman.get_position());

	std::array<bool, 4> walls{};
	walls[0] = is_wall_and_door_collision({ speed + position.x, position.y }, map, is_can_use_door);
	walls[1] = is_wall_and_door_collision({ position.x, position.y - speed }, map, is_can_use_door);
	walls[2] = is_wall_and_door_collision({ position.x - speed, position.y }, map, is_can_use_door);
	walls[3] = is_wall_and_door_collision({ position.x, speed + position.y }, map, is_can_use_door);
	
	if (1 != frightened_mode)
	{
		//I used 4 because using a number between 0 and 3 will make the ghost move in a direction it can't move.
		unsigned char optimal_direction = 4;

		//The ghost can move.
		move = true;

		for (unsigned char a = 0; a < 4; a++)
		{
			//ghosts can't turn back
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
			direction = (Direction)optimal_direction;
		}
		else
		{
			if (4 == optimal_direction)
			{
				//"Unless they have to" part.
				direction = (Direction)((2 + direction) % 4);
			}
			else
			{
				direction = (Direction)optimal_direction;
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
			move = true;

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

				direction = (Direction)random_direction;
			}
			else
			{
				//If there's no other way, it turns back.
				direction = (Direction)((2 + direction) % 4);
			}
		}
		else
		{
			frightened_speed_timer--;
		}
	}

	if (move)
	{
		short cell_y = position.y / CELL_SIZE;
		if (cell_y < 0) {
			cell_y = 0;
		}
		else if (cell_y > MAP_HEIGHT) {
			cell_y = MAP_HEIGHT;
		}

		short cell_x = position.x / CELL_SIZE;
		if (cell_x < 0) {
			cell_x = 0;
		}
		else if (cell_x > MAP_WIDTH) {
			cell_x = MAP_WIDTH;
		}

		switch (direction)
		{
		case Direction::Right:
		{
			if (0 <= cell_x && cell_x < MAP_WIDTH && 0 <= cell_y && cell_y < MAP_HEIGHT) {
				if (map[cell_y][cell_x] == Cell::Tunnel) {
					position.x += speed / 2;
				}
				else {
					position.x += speed;
				}
			}

			break;
		}
		case Direction::Up:
		{
			if (0 <= cell_x && cell_x < MAP_WIDTH && 0 <= cell_y && cell_y < MAP_HEIGHT) {
				if (map[cell_y][cell_x] == Cell::Tunnel) {
					position.y -= speed / 2;
				}
				else {
					position.y -= speed;
				}
			}

			break;
		}
		case Direction::Left:
		{
			if (0 <= cell_x && cell_x < MAP_WIDTH && 0 <= cell_y && cell_y < MAP_HEIGHT) {
				if (map[cell_y][cell_x] == Cell::Tunnel) {
					position.x -= speed / 2;
				}
				else {
					position.x -= speed;
				}
			}

			break;
		}
		case Direction::Down:
		{
			if (0 <= cell_x && cell_x < MAP_WIDTH && 0 <= cell_y && cell_y < MAP_HEIGHT) {
				if (map[cell_y][cell_x] == Cell::Tunnel) {
					position.y += speed / 2;
				}
				else {
					position.y += speed;
				}
			}

			break;
		}
		default: {
			break;
		}
		}

		if (-CELL_SIZE >= position.x) // When ghost enters the tunnel on the right side...
		{
			position.x = CELL_SIZE * MAP_WIDTH - speed; // he appears on the left side
		}
		else if (CELL_SIZE * MAP_WIDTH <= position.x) // When ghost enters the tunnel on the left side...
		{
			position.x = speed - CELL_SIZE; // he appears on the right side
		}
		else if (-CELL_SIZE >= position.y) // When ghost enters the tunnel on the up side...
		{
			position.y = CELL_SIZE * MAP_HEIGHT - speed; // he appears on the down side
		}
		else if (CELL_SIZE * MAP_HEIGHT <= position.y) // When ghost enters the tunnel on the down side...
		{
			position.y = speed - CELL_SIZE; // he appears on the up side
		}
	}

	if (is_collision(this->position, pacman.get_position())) //If pacman and ghost is touch then...
	{
		if (frightened_mode != 0) {
			is_can_use_door = true;

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
			else if (start_position == target_position) //If the ghost has reached its i_start_position.
			{
				frightened_mode = 0; //It stops being frightened.

				target_position = door_position; //And starts leaving the house.

				speed = GHOST_SPEED;
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

bool Ghost::is_frightened()
{
	return frightened_mode;
}

Position Ghost::get_position()
{
	return position;
}

void Ghost::set_start_position(const Position& i_start_position)
{
	start_position.x = i_start_position.x;
	start_position.y = i_start_position.y;
}

void Ghost::set_door_position(const Position& i_door_position)
{
	door_position = i_door_position;
}