#include "game.hpp"

#include "raymath.h"
#include <cmath>
#include <random>



Player::Player(Edge* e) : Object(e)
{
	radius = 10;
	speed = 10;

	if (!edge) return;
	position.x = (edge->A.x + edge->B.x) / 2;
	position.y = (edge->A.y + edge->B.y) / 2;
}

void Player::update(float delta_time)
{
	// move player left/right
	if (IsKeyPressed(KEY_LEFT))
		edge = edge->left;
	if (IsKeyPressed(KEY_RIGHT))
		edge = edge->right;

	// calculate target position
	Vector2 target_pos;
	target_pos.x = (edge->A.x + edge->B.x) / 2;
	target_pos.y = (edge->A.y + edge->B.y) / 2;

	// move and draw
	position = Vector2Lerp(position, target_pos, delta_time * speed);
	DrawCircleV(position, radius, YELLOW);
}



Projectile::Projectile(Edge* e, Vector2 pos, int s) : Object(e)
{
	radius = 5;
	speed = s;

	position = pos;
}

bool Projectile::update(float delta_time)
{
	// calculate target position
	Vector2 target_pos;
	target_pos.x = (edge->A2.x + edge->B2.x) / 2;
	target_pos.y = (edge->A2.y + edge->B2.y) / 2;

	// move and draw
	position = Vector2MoveTowards(position, target_pos, delta_time * speed);
	DrawCircleV(position, radius, SKYBLUE);

	// return true if projectile should be destroyed
	return Vector2Distance(position, target_pos) < 1;
}



Enemy::Enemy(Edge* e, int s) : Object(e)
{
	radius = 15;
	speed = s;

	if (!edge) return;
	position1 = edge->A2;
	position2 = edge->B2;
}

class Explosion {
	public:
		Vector2 position;
		float lifetime; 
	
		Explosion(Vector2 pos) : position(pos), lifetime(0.1f) {}
	
		void update(float delta_time) {
			lifetime -= delta_time;
		}
	
		bool isAlive() const {
			return lifetime > 0;
		}

		// draw a star-explosion
		void draw() const {
			DrawLineV(position, (Vector2){position.x - 10, position.y}, RED);
			DrawLineV(position, (Vector2){position.x + 10, position.y}, RED);
			DrawLineV(position, (Vector2){position.x, position.y - 10}, RED);
			DrawLineV(position, (Vector2){position.x, position.y + 10}, RED);
			DrawLineV(position, (Vector2){position.x - 7, position.y - 7}, RED);
			DrawLineV(position, (Vector2){position.x + 7, position.y + 7}, RED);
			DrawLineV(position, (Vector2){position.x - 7, position.y + 7}, RED);
			DrawLineV(position, (Vector2){position.x + 7, position.y - 7}, RED);
		}
	};
	

void Enemy::update(float delta_time)
{
	// jump to adjacent edges
	if (GetRandomValue(0, 200))
		edge = edge->left;
	if (GetRandomValue(0, 200))
		edge = edge->right;
	
	// draw lines
	Vector2 p1 = Vector2MoveTowards(position1, edge->A, radius), p2 = Vector2MoveTowards(position2, edge->B, radius), m1, m2;
	DrawLineV(position1, p2, RED);
	DrawLineV(position2, p1, RED);

	// is on outer vertex
	if (Vector2Distance(p1, edge->A) < 1 || Vector2Distance(p2, edge->B) < 1)
	{
		edging = true;
		return;
	}

	// move
	position1 = Vector2MoveTowards(position1, edge->A, delta_time * speed);
	position2 = Vector2MoveTowards(position2, edge->B, delta_time * speed);
}

std::vector<Explosion> explosions;

bool Enemy::collide(Vector2 pos, int r)
{
    if (CheckCollisionCircleLine(pos, r, position1, position2)) {  // if killed
        explosions.emplace_back((Explosion){(position1 + position2) / 2}); // create explosion
        return true;
    }
    return false;
}


void Game::generate()
{
	std::vector<Vector2> verticies, back_verticies; // verticies of the shape

	// reset everything
	edges.clear();
	level_transition = 0.5;

	// place points in regular polygon:
	int size = score / 1000 + 10; // number of points
	for (int i = 0; i < size; i++)
	{
		float x = (offset.x + std::sin(360 / size * i * PI / 180) * 30);
		float y = (offset.y - std::cos(360 / size * i * PI / 180) * 30);
		verticies.push_back((Vector2){ x, y });

		x = (center.x + std::sin(360 / size * i * PI / 180) * 2);
		y = (center.y - std::cos(360 / size * i * PI / 180) * 2);
		back_verticies.push_back((Vector2){ x, y });
	}

	// create edges
	for (int i = 0; i < size; i++)
		edges.push_back((Edge){nullptr, nullptr, verticies[i], verticies[(i + 1) % size], back_verticies[i], back_verticies[(i + 1) % size]});

	// connect edges
	for (int i = 0; i < size; i++)
	{
		edges[i].right = &edges[(i + 1) % size];
		edges[(i + 1) % size].left = &edges[i];
	}

	player.edge = &edges[0];
}


void Game::next_level()
{
	level_transition = 2; // start animation timer
	enemies.clear();
	projectiles.clear();
}


void Game::transition(float delta_time)
{
	level_transition -= delta_time * 0.9;

	explosions.clear();
	// when animation ends, generate new level
	if (level_transition <= 1.1 && level_transition > 1)
		generate();

	// move everything on screen
	for (Edge &edge : edges)
	{
		edge.A = Vector2Lerp(edge.A, Vector2Add(edge.A, Vector2Subtract(edge.A, center)), delta_time * 4);
		edge.B = Vector2Lerp(edge.B, Vector2Add(edge.B, Vector2Subtract(edge.B, center)), delta_time * 4);
		edge.A2 = Vector2Lerp(edge.A2, Vector2Add(edge.A2, Vector2Subtract(edge.A2, center)), delta_time * 4);
		edge.B2 = Vector2Lerp(edge.B2, Vector2Add(edge.B2, Vector2Subtract(edge.B2, center)), delta_time * 4);
	}
	
	draw();
}


void Game::draw()
{
	// draw normal lines
	for (Edge edge : edges)
	{
		DrawLineV(edge.A, edge.B, BLUE);
		DrawLineV(edge.A2, edge.B2, BLUE);
		DrawLineV(edge.A, edge.A2, BLUE);
		DrawLineV(edge.B, edge.B2, BLUE);
	}

	// draw highlighted lines
	DrawLineV(player.edge->A, player.edge->B, YELLOW);
	DrawLineV(player.edge->A2, player.edge->B2, YELLOW);
	DrawLineV(player.edge->A, player.edge->A2, YELLOW);
	DrawLineV(player.edge->B, player.edge->B2, YELLOW);
}


void Game::update(float delta_time)
{
	// level transition (if happening)
	if (level_transition > 0)
	{
		transition(delta_time);
		return;
	}

	// move player, projectiles and enemies
	player.update(delta_time);

	for (int i = 0; i < projectiles.size(); i++)
	{
		if (projectiles[i].update(delta_time))
			projectiles.erase(projectiles.begin() + i);

		for (int j = 0; j < enemies.size(); j++)
		{
			if (enemies[j].collide(projectiles[i].position, projectiles[i].radius))
			{
				enemies.erase(enemies.begin() + j);
				projectiles.erase(projectiles.begin() + i);
				score_points();
			}
		}
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].update(delta_time);

		if (enemies[i].edging && enemies[i].edge == player.edge)
		{
			enemies.erase(enemies.begin() + i);
			lose_health();
		}
		// for (int j = 0; j < enemies.size(); j++)
		// {
		// 	if (i != j && enemies[i].edging && enemies[j].edging && enemies[i].edge == enemies[j].edge)
		// 	{
		// 		enemies.erase(enemies.begin() + i);
		// 		enemies.erase(enemies.begin() + j);
		// 	}
		// }
	}
	// draw explosion
	for (auto it = explosions.begin(); it != explosions.end();) {
		it->update(delta_time);
		if (!it->isAlive()) {
			it = explosions.erase(it);
		} else {
			it->draw();
			++it;
		}
	}

	// draw
	draw();

	// shoot
	if (IsKeyPressed(KEY_SPACE))
		projectiles.push_back(Projectile(player.edge, player.position, 400));
	
	// spawn enemies
	if (GetRandomValue(0, 200 - score / 50) == 0)
		enemies.push_back(Enemy(&edges[GetRandomValue(0, edges.size() - 1)], 50 + score / 50));
}


void Game::lose_health()
{
	health--;	
}


void Game::score_points()
{
	score += 100;
	if (score % 1000 == 0)
		next_level();
}


Game::Game(Vector2 c) : center(c)
{
	offset = Vector2Add(center, (Vector2){0, -5});
	player = Player(nullptr);

	generate();
}