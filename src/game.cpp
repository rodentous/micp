#include "game.hpp"
#include <cmath>
#include <random>



void Game::generate()
{
	std::vector<Vector2> verticies, inner_verticies; // verticies of the shape

	// reset everything
	edges.clear();
	objects.clear();
	level_transition = 0.5;
	int size = score / 1000 + 5; // number of points

	// star
	if (score / 1000 % 2 == 1)
	{
		for (int i = 0; i < size; i++)
		{
			float angle = DEG2RAD * 360.0f / size * i;

			// outer verticies
			float outer_radius = (i % 2 == 0) ? 40.0f : 20.0f;
			float x = offset.x + sin(angle) * outer_radius;
			float y = offset.y - cos(angle) * outer_radius;
			verticies.push_back((Vector2){x, y});

			// inner verticies
			float inner_radius = (i % 2 == 0) ? 4.0f : 2.0f;
			x = center.x + sin(angle) * inner_radius;
			y = center.y - cos(angle) * inner_radius;
			inner_verticies.push_back((Vector2){x, y});
		}
	}
	// regular polygon
	else
	{
		for (int i = 0; i < size; i++)
		{
			float angle = DEG2RAD * 360.0f / size * i;

			float x = offset.x + sin(angle) * 40.0f;
			float y = offset.y - cos(angle) * 40.0f;
			verticies.push_back((Vector2){x, y});

			x = center.x + sin(angle) * 4.0f;
			y = center.y - cos(angle) * 4.0f;
			inner_verticies.push_back((Vector2){x, y});
		}
	}

	// create edges
	for (int i = 0; i < size; i++)
		edges.push_back((Edge){nullptr, nullptr, verticies[i], verticies[(i + 1) % size], inner_verticies[i], inner_verticies[(i + 1) % size]});

	// connect edges
	for (int i = 0; i < size; i++)
	{
		edges[i].right = &edges[(i + 1) % size];
		edges[(i + 1) % size].left = &edges[i];
	}
	player.edge = &edges[size/2];
	player.position.x = (player.edge->A.x + player.edge->B.x) / 2;
	player.position.y = (player.edge->A.y + player.edge->B.y) / 2;
}


void Game::next_level()
{
	level_transition = 2; // start animation timer
	objects.clear();
}


void Game::transition(float delta_time)
{
	level_transition -= delta_time * 0.9;

	// when animation ends, generate new level
	if (level_transition <= 1.1 && level_transition > 1)
		generate();

	// move everything on screen
	for (Edge &edge : edges)
	{
		edge.A = Vector2Lerp(edge.A, Vector2Add(edge.A, Vector2Subtract(edge.A, offset)), delta_time * 4);
		edge.B = Vector2Lerp(edge.B, Vector2Add(edge.B, Vector2Subtract(edge.B, offset)), delta_time * 4);
		edge.A2 = Vector2Lerp(edge.A2, Vector2Add(edge.A2, Vector2Subtract(edge.A2, center)), delta_time * 4);
		edge.B2 = Vector2Lerp(edge.B2, Vector2Add(edge.B2, Vector2Subtract(edge.B2, center)), delta_time * 4);
	}

	draw();
}


void Game::draw()
{
	// draw normal lines
	for (Edge &edge : edges)
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

	// offset
	Vector2 new_offset = Vector2Add(offset, Vector2Scale(Vector2Subtract(player.position, offset), 0.1));
	for (Edge &edge : edges)
	{
		edge.A = Vector2Add(new_offset, Vector2Scale(Vector2Subtract(edge.A2, offset), 10));
		edge.B = Vector2Add(new_offset, Vector2Scale(Vector2Subtract(edge.B2, offset), 10));
	}

	// draw level
	draw();


	// move player, projectiles and enemies
	player.update(delta_time);

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->dead)
			objects.erase(objects.begin() + i);
		else
			objects[i]->update(delta_time);
	}

	// spawn enemies
	if (GetRandomValue(0, 200 - score / 50) == 0)
		objects.push_back(std::make_unique<Enemy>(&edges[GetRandomValue(0, edges.size() - 1)], 50 + score / 50));
}


void Game::lose_health()
{
	health--;
	objects.push_back(std::make_unique<Explosion>(player.edge, player.position, RED, 50));
	PlaySound(hurt_sound);
}


void Game::score_points()
{
	score += 100;
	if (score % 1000 == 0)
		next_level();
}


void Game::reset()
{
	health = 3;
	score = 0;
	generate();
}


Game::Game()
{
	center = (Vector2){1920/2, 1080/2};
	offset = Vector2Add(center, (Vector2){0, 10});
	player = Player(nullptr);

	health = 3;
	score = 0;
	level_transition = 0;

	hurt_sound = LoadSound("sounds/hurt.wav");
	SetSoundVolume(hurt_sound, 5);
	explosion_sound = LoadSound("sounds/explosion.wav");

	// TODO: more sounds
	// over_sound = LoadSound("sounds/over.wav");
	// main_music = LoadSound("sounds/main.wav");
	// game_music = LoadSound("sounds/game.wav");

	generate();
}
