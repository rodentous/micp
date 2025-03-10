#include "game.hpp"
#include "raymath.h"

#include <cmath>
#include <random>



void Game::generate()
{
	// reset everything
	points.clear();
	far_points.clear();
	player.index =   0;
	player.position = offset;

	// place points in regular polygon:
	int size = GetRandomValue(3, 10); // number of points
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
	{
		projectiles.push_back({player.index, player.position});
		score += 100;
	}

	// force next level
	if (IsKeyPressed(KEY_BACKSPACE))
		next_level();
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


void Game::move_projectiles(float delta_time)
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i].position = Vector2MoveTowards(projectiles[i].position, offset, delta_time * 300);

		// destroy projectile
		if (Vector2Distance(projectiles[i].position, offset) <= 20)
			projectiles.erase(projectiles.begin() + i);
		
		DrawCircleV(projectiles[i].position, 5, YELLOW);
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

	// move projectiles
	move_projectiles(delta_time);
}


Game::Game(Vector2 c) : center(c)
{
	offset = Vector2Add(center, (Vector2){0, 100});

	generate();
}