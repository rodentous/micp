#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include "raylib.h"
#include "raymath.h"



class Game
{
private:
	// singleton stuff
	Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

public:
	// get instance of the singleton
	static Game& get_instance()
	{
		static Game instance;
		return instance;
	}

	void Start();

	void Update();
};

#endif