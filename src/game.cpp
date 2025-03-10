#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <cmath>
#include <random>


class Game
{
private:
	std::vector<Vector2> points, far_points;
	Vector2 center, offset;

	int player = 0;
	Vector2 player_pos = {0, 0};
	float player_dir = 0;
	std::vector<Vector2> projectiles;

	float next_level = 0;
	int score = 0;

public:
	int get_score()
	{
		return score;
	}

	void generate()
	{
		points.clear();
		far_points.clear();
		player = 0;
		player_pos = offset;

		// place points in regular polygon
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

	void update(float delta_time)
	{
		// next level
		if (next_level > 0)
		{
            next_level -= delta_time * 0.75;
            if (next_level <= 0)
                generate();
			for (Vector2 &point : points)
				point = Vector2Lerp(point, Vector2Add(point, Vector2Subtract(point, offset)), delta_time * 4);
			for (Vector2 &point : far_points)
				point = Vector2Lerp(point, Vector2Add(point, Vector2Subtract(point, offset)), delta_time * 4);
		}

		// handle input
		if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
			player = (player + 1) % points.size();
		if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
			player = (player + points.size() - 1) % points.size();
		if (IsKeyPressed(KEY_SPACE))
		{
			projectiles.push_back(player_pos);
			score += 100;
		}
		if (IsKeyPressed(KEY_BACKSPACE))
            next_level = 1;

		// draw lines
		for (int i = 0; i < points.size(); i++)
		{
			DrawLineV(points[i], points[(i + 1) % points.size()], BLUE);
			DrawLineV(far_points[i], far_points[(i + 1) % points.size()], BLUE);
			DrawLineV(points[i], far_points[i], BLUE);
		}
		// draw lines around player
		DrawLineV(points[player], points[(player + 1) % points.size()], YELLOW);
		DrawLineV(far_points[player], far_points[(player + 1) % points.size()], YELLOW);
		DrawLineV(points[player], far_points[player], YELLOW);
		DrawLineV(points[(player + 1) % points.size()], far_points[(player + 1) % points.size()], YELLOW);

		
		// target position for player
		Vector2 target_pos;
		target_pos.x = (points[player].x + points[(player + 1) % points.size()].x) / 2;
		target_pos.y = (points[player].y + points[(player + 1) % points.size()].y) / 2;
		// move player
		player_pos = Vector2Lerp(player_pos, target_pos, delta_time * 10);
		DrawCircleV(player_pos, 10, YELLOW);

		// move projectiles
		for (int i = 0; i < projectiles.size(); i++)
		{
			projectiles[i] = Vector2MoveTowards(projectiles[i], offset, delta_time * 300);

			// destroy projectile
			if (Vector2Distance(projectiles[i], offset) <= 20)
				projectiles.erase(projectiles.begin() + i);
			
			DrawCircleV(projectiles[i], 5, YELLOW);
		}
	}

	Game(Vector2 c) : center(c)
	{
		offset = Vector2Add(center, (Vector2){0, 100});

		generate();
	}
};