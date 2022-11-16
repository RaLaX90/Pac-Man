#include "Pacman.h"

Pacman::Pacman(Position i_start_position) :
	start_position(i_start_position)
{
	texture_dead.loadFromFile("Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");
	texture_alive.loadFromFile("Images/Pacman" + std::to_string(CELL_SIZE) + ".png");

	reset();
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

void Pacman::draw(bool is_victory, sf::RenderWindow& window)
{
	unsigned char frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(PACMAN_ANIMATION_SPEED)));

	sf::Sprite sprite;

	sprite.setPosition(position.x, position.y);

	if (dead || is_victory)
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
	dead = is_dead;

	if (dead)
	{
		//Making sure that the animation starts from the beginning.
		animation_timer = 0;
	}
}

void Pacman::set_position(float x, float y)
{
	position = { x, y };
}

void Pacman::move(unsigned char level, std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map)
{
	auto debug1 = position.x / CELL_SIZE;
	auto debug2 = (PACMAN_SPEED + position.x) / CELL_SIZE;
	auto debug3 = position.y / CELL_SIZE;

	//std::array<bool, 4> walls{ };
	/*map[debug3][debug2] = Cell::Energizer;*/
	//auto debug = map_collision(PACMAN_SPEED + position.x, position.y, map) == Cell::Wall;
	
	//cells_around[0] = map_collision(PACMAN_SPEED + position.x, position.y, direction, map) == Cell::Wall;
	//cells_around[1] = map_collision(position.x, position.y - PACMAN_SPEED, direction, map) == Cell::Wall;
	//cells_around[2] = map_collision(position.x - PACMAN_SPEED, position.y, direction, map) == Cell::Wall;
	//cells_around[3] = map_collision(position.x, PACMAN_SPEED + position.y, direction, map) == Cell::Wall;	
	
	std::array<bool, 4> cells_around{ };	
																				
																				
																				
	cells_around[0] = map_collision2(PACMAN_SPEED + position.x, position.y, map);//0 - top left
	cells_around[1] = map_collision2(position.x, position.y - PACMAN_SPEED, map);//1 - top right
	cells_around[2] = map_collision2(position.x - PACMAN_SPEED, position.y, map);//2 - bottom left
	cells_around[3] = map_collision2(position.x, PACMAN_SPEED + position.y, map);//3 - bottom right

	//walls[0] = is_map_collision(false, false, PACMAN_SPEED + position.x, position.y, map);
	//walls[1] = is_map_collision(false, false, position.x, position.y - PACMAN_SPEED, map);
	//walls[2] = is_map_collision(false, false, position.x - PACMAN_SPEED, position.y, map);
	//walls[3] = is_map_collision(false, false, position.x, PACMAN_SPEED + position.y, map);


	auto qwe = is_in_cell_center(position.x, position.y);
	auto wer = !cells_around[Direction::Up];

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != Direction::Right /*&& !walls[direction]*/ /*&& is_in_cell_center(position.x, position.y)*/ && !cells_around[Direction::Right] /*&& cells_around[1] != Cell::Wall && cells_around[1] != Cell::Door && cells_around[3] != Cell::Wall && cells_around[3] != Cell::Door*/)
	{
		direction = Direction::Right;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != Direction::Up /*&& !walls[direction]*/ /*&& is_in_cell_center(position.x, position.y)*/ && !cells_around[Direction::Up] /*&& cells_around[0] != Cell::Wall && cells_around[0] != Cell::Door && cells_around[1] != Cell::Wall && cells_around[1] != Cell::Door*/)
	{
		direction = Direction::Up;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != Direction::Left /*&& !walls[direction]*/ /*&& is_in_cell_center(position.x, position.y)*/ && !cells_around[Direction::Left] /*&& cells_around[0] != Cell::Wall && cells_around[0] != Cell::Door && cells_around[2] != Cell::Wall && cells_around[2] != Cell::Door*/)
	{
		direction = Direction::Left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != Direction::Down /*&& !walls[direction]*/ /*&& is_in_cell_center(position.x, position.y)*/ && !cells_around[Direction::Down] /*&& cells_around[2] != Cell::Wall && cells_around[2] != Cell::Door && cells_around[3] != Cell::Wall && cells_around[3] != Cell::Door*/)
	{
		direction = Direction::Down;
	}

	//if (!walls[direction])
	if (!cells_around[direction])
	{
		switch (direction)
		{
		case Direction::Right:
		{
			//if (cells_around[1] != Cell::Wall && cells_around[1] != Cell::Door && cells_around[3] != Cell::Wall && cells_around[3] != Cell::Door) 
			//{
				position.x += PACMAN_SPEED;
			//}
			//else {
			//	auto qwe = 0;
			//}

			break;
		}
		case Direction::Up:
		{
			//if (cells_around[0] != Cell::Wall && cells_around[0] != Cell::Door && cells_around[1] != Cell::Wall && cells_around[1] != Cell::Door)
			//{
				position.y -= PACMAN_SPEED;
			//}

			break;
		}
		case Direction::Left:
		{
			//if (cells_around[0] != Cell::Wall && cells_around[0] != Cell::Door && cells_around[2] != Cell::Wall && cells_around[2] != Cell::Door)
			//{
				position.x -= PACMAN_SPEED;
			//}

			break;
		}
		case Direction::Down:
		{
			//if (cells_around[2] != Cell::Wall && cells_around[2] != Cell::Door && cells_around[3] != Cell::Wall && cells_around[3] != Cell::Door)
			//{
				position.y += PACMAN_SPEED;
			//}

			break;
		}
		default: {
			break;
		}
		}
	}
	else {
		auto qwe = 0;
	}

	if (-CELL_SIZE >= position.x) // When pacman enters the tunnel on the right side...
	{
		position.x = CELL_SIZE * MAP_WIDTH - PACMAN_SPEED; // he appears on the left side
	}
	else if (CELL_SIZE * MAP_WIDTH <= position.x) // When pacman enters the tunnel on the left side...
	{
		position.x = PACMAN_SPEED - CELL_SIZE; // he appears on the right side
	}

	//if (map_collision(position.x, position.y, map) == Cell::Energizer/*is_map_collision(true, false, position.x, position.y, map)*/) //When Pacman eats an energizer...
	//{
	//	//He becomes energized!
	//	energizer_timer = static_cast<unsigned short>(ENERGIZER_DURATION / pow(2, level));
	//}
	//else
	//{
		energizer_timer = std::max(0, energizer_timer - 1);
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