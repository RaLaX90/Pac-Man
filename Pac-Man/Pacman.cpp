#include "Pacman.h"

Pacman::Pacman(Position i_start_position) :
	Game_object(i_start_position, PACMAN_SPEED)
{
	texture_dead.loadFromFile("Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");
	texture_alive.loadFromFile("Images/Pacman" + std::to_string(CELL_SIZE) + ".png");

	reset();
}

Pacman::~Pacman()
{
}

bool Pacman::get_animation_over()
{
	return animation_over;
}

bool Pacman::is_dead()
{
	return dead;
}

Direction Pacman::get_direction()
{
	return direction;
}

unsigned short Pacman::get_energizer_timer()
{
	return energizer_timer;
}

void Pacman::draw(const State& mode, sf::RenderWindow& window)
{
	unsigned char frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(PACMAN_ANIMATION_SPEED)));

	sf::Sprite sprite;

	sprite.setPosition(position.x, position.y);

	if (mode == State::STATE_PACMAN_DEAD || mode == State::STATE_ALL_PELLETS_COLLECTED)
	{
		if (animation_timer < PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED)
		{
			animation_timer++;

			sprite.setTexture(texture_dead);
			sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));

			window.draw(sprite);
		}
		else
		{
			//You can only die once.
			animation_over = true;
		}
	}
	else
	{
		sprite.setTexture(texture_alive);
		sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, CELL_SIZE * direction, CELL_SIZE, CELL_SIZE));

		window.draw(sprite);

		animation_timer = (1 + animation_timer) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
	}
}

void Pacman::reset()
{
	animation_over = false;
	dead = false;

	direction = Direction::Right;

	animation_timer = 0;
	energizer_timer = 0;

	set_position(start_position.x, start_position.y);
}

void Pacman::set_animation_timer(unsigned short i_animation_timer)
{
	animation_timer = i_animation_timer;
}

void Pacman::set_dead(bool is_dead)
{
	if (dead = is_dead)
	{
		//Making sure that the animation starts from the beginning.
		animation_timer = 0;
	}
}

void Pacman::set_position(float new_position_x, float new_position_y)
{
	position = { new_position_x, new_position_y };
}

void Pacman::move(unsigned char level, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map) //TODO (make 'map' parameter const)
{
	std::array<bool, 4> walls{ };

	walls[0] = is_wall_and_door_collision(speed + position.x, position.y, map);	// If there are walls or doors around pacman:	in right direction
	walls[1] = is_wall_and_door_collision(position.x, position.y - speed, map);	//												in up direction
	walls[2] = is_wall_and_door_collision(position.x - speed, position.y, map);	//												in left direction
	walls[3] = is_wall_and_door_collision(position.x, speed + position.y, map);	//												in down direction

	/*if(is_in_cell_center(position.x, position.y)){*/ // No needed (leave it here just in case)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != Direction::Right && !walls[Direction::Right])
	{
		direction = Direction::Right;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != Direction::Up && !walls[Direction::Up])
	{
		direction = Direction::Up;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != Direction::Left && !walls[Direction::Left])
	{
		direction = Direction::Left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != Direction::Down && !walls[Direction::Down])
	{
		direction = Direction::Down;
	}
	//}


	if (!walls[direction])
	{
		switch (direction)
		{
		case Direction::Right:
		{
			position.x += speed;

			break;
		}
		case Direction::Up:
		{
			position.y -= speed;

			break;
		}
		case Direction::Left:
		{
			position.x -= speed;

			break;
		}
		case Direction::Down:
		{
			position.y += speed;

			break;
		}
		default: {
			break;
		}
		}
	}

	if (-CELL_SIZE >= position.x) // When pacman enters the tunnel on the right side...
	{
		position.x = CELL_SIZE * MAP_WIDTH - speed; // he appears on the left side
	}
	else if (CELL_SIZE * MAP_WIDTH <= position.x) // When pacman enters the tunnel on the left side...
	{
		position.x = speed - CELL_SIZE; // he appears on the right side
	}

	auto cell_y = position.y / CELL_SIZE;
	auto cell_x = position.x / CELL_SIZE;

	if (is_in_cell_center(position.x, position.y)) {
		if (0 <= cell_x && cell_x < MAP_WIDTH && 0 <= cell_y && cell_y < MAP_HEIGHT) {
			switch (map[cell_y][cell_x])
			{
			case Cell::Pellet: {
				map[cell_y][cell_x] = Cell::Empty;

				break;
			}
			case Cell::Energizer: {
				map[cell_y][cell_x] = Cell::Empty;
				energizer_timer = static_cast<unsigned short>(ENERGIZER_DURATION / pow(2, level));
			}
			default:
				energizer_timer = std::max(0, energizer_timer - 1);

				break;
			}
		}
	}
	//if (map_collision(position.new_position_x, position.new_position_y, map) == Cell::Energizer/*is_wall_and_door_collision(true, false, position.new_position_x, position.new_position_y, map)*/) //When Pacman eats an energizer...
	//{
	//	//He becomes energized!
	//	energizer_timer = static_cast<unsigned short>(ENERGIZER_DURATION / pow(2, level));
	//}
	//else
	//{
	//energizer_timer = std::max(0, energizer_timer - 1);
	//}
}

void Pacman::set_start_position(const Position& i_start_position)
{
	start_position.x = i_start_position.x;
	start_position.y = i_start_position.y;
}

Position Pacman::get_position()
{
	return position;
}