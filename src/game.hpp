#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <memory>



struct Edge
{
	Edge *left, *right; // adjacent edges
	Vector2 A, B, A2, B2; // points of the edge
};


class Object
{
public:
	Edge* edge;
	bool dead;
	Vector2 position;
	int radius;
	int speed;

	Object(Edge* e);
	virtual void update(float delta_time) {};
};


class Player : public Object
{
public:
	Player(Edge* e);
	void update(float delta_time) override;
};


class Projectile : public Object
{
public:
	Projectile(Edge* e, Vector2 pos);
	void update(float delta_time) override;
};


class Explosion : public Object
{
protected:
	float lifetime;
	Color color;

public:
	Explosion(Edge* e, Vector2 pos, Color c, int r);
	void update(float delta_time) override;
};


class Enemy : public Object
{
protected:
	bool edging;

public:
	Vector2 position2;

	Enemy(Edge* e, int s);
	void update(float delta_time) override;
	bool collide(Vector2 pos, int r);
};


class Game
{
private:
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
	Vector2 center, offset; // coordinates of center/offset
	Sound move_sound, shot_sound, boom_sound, hurt_sound;

	Player player = Player(nullptr);
	std::vector<std::unique_ptr<Object>> objects;

	int score, health;

	void lose_health();
	void score_points();
	void update(float delta_time);

	// get instance of the singleton
	static Game& get_instance()
	{
		static Game instance;
		return instance;
	}
};

#endif