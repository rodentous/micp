#include "game.hpp"
#include "raymath.h"

#include <cmath>
#include <random>



void Game::generate()
{
	// reset everything
	points.clear();
	far_points.clear();
	player.index = 0;
	player.position = offset;

	// place points in regular polygon:
	int size = GetRandomValue(4, 10); // number of points
	for (int i = 0; i < size; i++)
	{
		int x = (center.x + std::sin(360 / size * i * PI / 180) * 300);
		int y = (center.y - std::cos(360 / size * i * PI / 180) * 300);
		points.push_back((Vector2){ x, y });

		x = (offset.x + std::sin(360 / size * i * PI / 180) * 20);
		y = (offset.y - std::cos(360 / size * i * PI / 180) * 20);
		far_points.push_back((Vector2){ x, y });
	}
}


void Game::next_level()
{
	level_transition = 1; // start animation timer
	projectiles.clear(); // delete all projectiles
	spikes.clear(); // delete all spikes
}


void Game::handle_input()
{
	// move player left/right
	if (IsKeyPressed(KEY_LEFT))
		player.index = (player.index + 1) % points.size();
	if (IsKeyPressed(KEY_RIGHT))
		player.index = (player.index + points.size() - 1) % points.size();

	// shoot
	if (IsKeyPressed(KEY_SPACE))
		projectiles.push_back((Object){player.index, player.position});
}


void Game::move_player(float delta_time)
{
	// target position
	Vector2 target_pos;
	target_pos.x = (points[player.index].x + points[(player.index + 1) % points.size()].x) / 2;
	target_pos.y = (points[player.index].y + points[(player.index + 1) % points.size()].y) / 2;

	// move player
	player.position = Vector2Lerp(player.position, target_pos, delta_time * 10);
	DrawCircleV(player.position, 10, YELLOW);
}


void Game::move_objects(float delta_time)
{
	// move projectiles
	for (int i = 0; i < projectiles.size(); i++)
	{
		Vector2 target_pos;
		target_pos.x = (far_points[projectiles[i].index].x + far_points[(projectiles[i].index + 1) % far_points.size()].x) / 2;
		target_pos.y = (far_points[projectiles[i].index].y + far_points[(projectiles[i].index + 1) % far_points.size()].y) / 2;

		projectiles[i].position = Vector2MoveTowards(projectiles[i].position, target_pos, delta_time * 300);
		DrawCircleV(projectiles[i].position, 5, YELLOW);
		
		// destroy projectile
		if (Vector2Distance(projectiles[i].position, target_pos) <= 20)
			projectiles.erase(projectiles.begin() + i);
		
		// destroy spike on collision
		for (int j = 0; j < spikes.size(); j++)
		{
			if (spikes[j].index == projectiles[i].index && Vector2Distance(spikes[j].position, projectiles[i].position) <= 20)
			{
				projectiles.erase(projectiles.begin() + i);
				spikes.erase(spikes.begin() + j);
				score += 100;
				if (score % 1000 == 0)
					next_level();
			}
		}
	}

	// generate spikes
	if (GetRandomValue(0, 200) == 0)
	{
		int r = GetRandomValue(0, far_points.size() - 1);
		Vector2 pos;
		pos.x = (far_points[r].x + far_points[(r + 1) % far_points.size()].x) / 2;
		pos.y = (far_points[r].y + far_points[(r + 1) % far_points.size()].y) / 2;
	
		spikes.push_back((Object){r, pos});
	}

	// move spikes
	for (int i = 0; i < spikes.size(); i++)
	{
		Vector2 target_pos;
		target_pos.x = (points[spikes[i].index].x + points[(spikes[i].index + 1) % points.size()].x) / 2;
		target_pos.y = (points[spikes[i].index].y + points[(spikes[i].index + 1) % points.size()].y) / 2;

		spikes[i].position = Vector2MoveTowards(spikes[i].position, target_pos, delta_time * 20);
		DrawCircleV(spikes[i].position, 10, RED);
		
		if (Vector2Distance(spikes[i].position, target_pos) <= 20)
		{
			spikes.erase(spikes.begin() + i);
			health--;
		}
	}
}


void Game::transition(float delta_time)
{
	// if transition is not happening, return
	if (level_transition <= 0)
		return;

	level_transition -= delta_time * 0.75;

	// when animation ends, generate new level
	if (level_transition <= 0)
		generate();

	// move everything on screen
	for (Vector2 &point : points)
		point = Vector2Lerp(point, Vector2Add(point, Vector2Subtract(point, offset)), delta_time * 4);
	for (Vector2 &point : far_points)
		point = Vector2Lerp(point, Vector2Add(point, Vector2Subtract(point, offset)), delta_time * 4);
}


void Game::draw()
{
	// draw normal lines
	for (int i = 0; i < points.size(); i++)
	{
		DrawLineV(points[i], points[(i + 1) % points.size()], BLUE);
		DrawLineV(far_points[i], far_points[(i + 1) % points.size()], BLUE);
		DrawLineV(points[i], far_points[i], BLUE);
	}

	// draw highlighted lines
	DrawLineV(points[player.index], points[(player.index + 1) % points.size()], YELLOW);
	DrawLineV(far_points[player.index], far_points[(player.index + 1) % points.size()], YELLOW);
	DrawLineV(points[player.index], far_points[player.index], YELLOW);
	DrawLineV(points[(player.index + 1) % points.size()], far_points[(player.index + 1) % points.size()], YELLOW);
}


void Game::update(float delta_time)
{
	// level transition (if happening)
	transition(delta_time);

	// handle input
	handle_input();

	// draw lines
	draw();
	
	// handle player
	move_player(delta_time);

	// move projectiles and spikes
	move_objects(delta_time);
}


Game::Game(Vector2 c) : center(c)
{
	offset = Vector2Add(center, (Vector2){0, 100});

	generate();
}