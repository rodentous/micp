#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <memory>
#include "raylib.h"
#include "raymath.h"



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
protected:
	Sound move_sound, shoot_sound;

public:
	Player(Edge* e);
	void update(float delta_time) override;
	void shoot();
	void move_left();
	void move_right();
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
	Vector2 position2;

public:
	Enemy(Edge* e, int s);
	void update(float delta_time) override;
	bool collide(Vector2 pos, int r);
};

#endif