#include "raylib.h"
#include <vector>



struct Object
{
	int index;
	Vector2 position;
};


class Game
{

private:
	std::vector<Vector2> points, far_points; // points of level
	Vector2 center, offset; // coordinates of center/offset

	Object player;
	
	std::vector<Object> projectiles;

	float level_transition = 0; // timer for level transition

public:
	int score = 0;

	void generate();

	void handle_input();

	void move_player(float delta_time);

	void next_level();

	void transition(float delta_time);

	void move_projectiles(float delta_time);

	void draw();

	void update(float delta_time);

	Game(Vector2 c);

};