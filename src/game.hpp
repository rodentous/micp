#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <vector>



struct Edge
{
	Edge *left, *right; // adjacent edges
	Vector2 A, B, A2, B2; // points of the edge
};


class Object
{
public:
	Edge* edge;
	Vector2 position;
	int speed;
	int radius;

	Object(Edge* e) : edge(e) {}
	void update(float delta_time);
};


class Player : public Object
{
public:
	Player(Edge* e);
	void update(float delta_time);
};


class Projectile : public Object
{
public:
	Projectile(Edge* e, Vector2 pos, int s);
	bool update(float delta_time);
};


class Explosion : public Object
{
public:
	float lifetime;
	Color color;
	Explosion(Edge* e, Vector2 pos, Color c, int r);
	bool update(float delta_time);
};


class Enemy : public Object
{
public:
	Vector2 position1, position2;
	bool edging;

	Enemy(Edge* e, int s);
	void update(float delta_time);
	bool collide(Vector2 pos, int r);
};


class Game
{
private:
	std::vector<Edge> edges;
	
	Player player = Player(nullptr);
	std::vector<Enemy> enemies;
	std::vector<Projectile> projectiles;
	std::vector<Explosion> explosions;

	float level_transition = 0; // timer for level transition
	Sound move_sound, shot_sound, boom_sound, hurt_sound;

	void generate();
	
	void transition(float delta_time);
	
	void draw();
	
public:
	Vector2 center, offset; // coordinates of center/offset
	int score = 0, health = 5;

	void next_level();
	
	void lose_health();
	
	void score_points();

	void update(float delta_time);

	Game(Vector2 c);
};

#endif