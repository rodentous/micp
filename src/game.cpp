#include "game.hpp"

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


Explosion::Explosion(Edge* e, Vector2 pos, Color c, int r) : Object(e)
{
	radius = r;
	color = c;
	lifetime = 1;
	position = pos;
}

bool Explosion::update(float delta_time)
{
	lifetime -= delta_time * 2;
	radius -= delta_time * 10;

	DrawCircleV(position, radius, color);

	return lifetime < 0;
}


Enemy::Enemy(Edge* e, int s) : Object(e)
{
	radius = 15;
	speed = s;

	if (!edge) return;
	position1 = edge->A2;
	position2 = edge->B2;
}


void Enemy::update(float delta_time)
{
	// jump to adjacent edges
	if (GetRandomValue(0, 200))
		edge = edge->left;
	if (GetRandomValue(0, 200))
		edge = edge->right;

	// draw lines
	// Vector2 p1 = Vector2MoveTowards(position1, edge->A, radius), p2 = Vector2MoveTowards(position2, edge->B, radius), m1, m2;
	DrawLineV(position1, position2, RED);
	DrawCircleV(position1, 5, RED);
	DrawCircleV(position2, 5, RED);
	// DrawLineV(p1, p2, RED);
	

	// is on outer vertex
	if (Vector2Distance(position1, edge->A) < 1 || Vector2Distance(position2, edge->B) < 1)
		edging = true;

	// move
	position1 = Vector2MoveTowards(position1, edge->A, delta_time * speed);
	position2 = Vector2MoveTowards(position2, edge->B, delta_time * speed);
}

bool Enemy::collide(Vector2 pos, int r)
{
    return CheckCollisionCircleLine(pos, r, position1, position2);
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
		float x = (center.x + std::sin(DEG2RAD * 360 / size * i) * 30);
		float y = (center.y - std::cos(DEG2RAD * 360 / size * i) * 30);
		verticies.push_back((Vector2){ x, y });

		x = (offset.x + std::sin(DEG2RAD * 360 / size * i) * 3);
		y = (offset.y - std::cos(DEG2RAD * 360 / size * i) * 3);
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
		edge.A = Vector2Lerp(edge.A, Vector2Add(edge.A, Vector2Subtract(edge.A, offset)), delta_time * 4);
		edge.B = Vector2Lerp(edge.B, Vector2Add(edge.B, Vector2Subtract(edge.B, offset)), delta_time * 4);
		edge.A2 = Vector2Lerp(edge.A2, Vector2Add(edge.A2, Vector2Subtract(edge.A2, offset)), delta_time * 4);
		edge.B2 = Vector2Lerp(edge.B2, Vector2Add(edge.B2, Vector2Subtract(edge.B2, offset)), delta_time * 4);
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

	// draw map
	draw();

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
				explosions.push_back(Explosion(projectiles[i].edge, projectiles[i].position, SKYBLUE, 10));
				PlaySound(boom_sound);
				score_points();
			}
		}
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].update(delta_time);

		if (enemies[i].edging && enemies[i].collide(player.position, player.radius))
		{
			enemies.erase(enemies.begin() + i);
			explosions.push_back(Explosion(player.edge, player.position, RED, 20));
			PlaySound(hurt_sound);
			lose_health();
		}
	}
	for (int i = 0; i < explosions.size(); i++)
	{
		if (explosions[i].update(delta_time))
			explosions.erase(explosions.begin() + i);
	}


	// input
	if (IsKeyPressed(KEY_LEFT))
	{
		player.edge = player.edge->left;
		PlaySound(move_sound);
	}
	if (IsKeyPressed(KEY_RIGHT))
	{
		player.edge = player.edge->right;
		PlaySound(move_sound);
	}
	if (IsKeyPressed(KEY_SPACE))
	{
		projectiles.push_back(Projectile(player.edge, player.position, 400));
		PlaySound(shot_sound);
	}

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
	offset = Vector2Add(center, (Vector2){0, 5});
	player = Player(nullptr);

	move_sound = LoadSound("sounds/move.wav");
	shot_sound = LoadSound("sounds/shot.wav");
	boom_sound = LoadSound("sounds/boom.wav");
	hurt_sound = LoadSound("sounds/hurt.wav");

	generate();
}
