#pragma once

class Pacman
{
	//This is used for the death animation.
	bool animation_over;
	//Am I dead?
	bool dead;

	Position start_position;

	unsigned char direction;

	sf::Texture texture_dead;
	sf::Texture texture_alive;

	//More timers!
	unsigned short animation_timer;
	unsigned short energizer_timer;

	//Current location of this creature, commonly known as Pacman.
	Position position;
public:
	Pacman();

	bool get_animation_over();
	bool is_dead();

	unsigned char get_direction();

	unsigned short get_energizer_timer();

	void draw(bool is_victory, sf::RenderWindow& window);
	void reset();
	void set_animation_timer(unsigned short i_animation_timer);
	void set_dead(bool is_dead);
	void set_position(short x, short y);
	void move(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map);

	void set_start_position(const Position& i_start_position);
	Position get_position();
};