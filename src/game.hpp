#ifndef GAME_H
#define GAME_H

#include "objects.hpp"



class Game
{
private:
	Vector2 center, offset; // coordinates of center/offset
	std::vector<Edge> edges;
	float level_transition; // timer for level transition

	void generate();
	void transition(float delta_time);
	void draw();
	void next_level();

	// singleton stuff
	Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

public:
	Sound hurt_sound, explosion_sound;

	Player player = Player(nullptr);
	std::vector<std::unique_ptr<Object>> objects;

	int score, health;

	void lose_health();
	void score_points();
	void reset();
	void update(float delta_time);

	// get instance of the singleton
	static Game& get_instance()
	{
		static Game instance;
		return instance;
	}
};

#endif