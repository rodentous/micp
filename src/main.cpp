#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <cmath>


class Field
{
private:
	int size = 10; // number of points
	std::vector<Vector2> points;
	std::vector<Vector2> far_points;
	Vector2 center;

	int player = 0;
	Vector2 player_pos = {0, 0};
	std::vector<Vector2> projectiles;

public:
	void Update(float delta_time)
	{
		// handle input
		if (IsKeyPressed(KEY_LEFT))
			player = (player + 1) % size;
		if (IsKeyPressed(KEY_RIGHT))
			player = (player + size - 1) % size;
		if (IsKeyPressed(KEY_SPACE))
			projectiles.push_back(player_pos);
		
		// target position for player
		Vector2 target_pos;
		target_pos.x = (points[player].x + points[(player + 1) % size].x) / 2;
		target_pos.y = (points[player].y + points[(player + 1) % size].y) / 2;

		// draw lines
		for (int i = 0; i < size; i++)
		{
			DrawLineV(points[i], points[(i + 1) % size], BLUE);
			DrawLineV(far_points[i], far_points[(i + 1) % size], BLUE);
			DrawLineV(points[i], far_points[i], BLUE);
		}

		// draw lines around player
		DrawLineV(points[player], points[(player + 1) % size], YELLOW);
		DrawLineV(far_points[player], far_points[(player + 1) % size], YELLOW);
		DrawLineV(points[player], far_points[player], YELLOW);
		DrawLineV(points[(player + 1) % size], far_points[(player + 1) % size], YELLOW);

		// move player
		player_pos = Vector2Lerp(player_pos, target_pos, delta_time * 10);
		DrawCircleV(player_pos, 10, YELLOW);

		// move projectiles
		for (int i = 0; i < projectiles.size(); i++)
		{
			Vector2 offset = Vector2Add(center, (Vector2){0, 50});
			projectiles[i] = Vector2MoveTowards(projectiles[i], offset, delta_time * 300);
			if (Vector2Distance(projectiles[i], offset) <= 20)
				projectiles.erase(projectiles.begin() + i);
			DrawCircleV(projectiles[i], 5, YELLOW);
		}
	}

	Field(int n, Vector2 center) : size(n), center(center)
	{
		Vector2 offset = Vector2Add(center, (Vector2){0, 50});

		// place points in regular polygon
		for (int i = 0; i < size; i++)
		{
			int x = (center.x + std::sin(360 / size * i * PI / 180) * 200);
			int y = (center.y - std::cos(360 / size * i * PI / 180) * 200);
			points.push_back((Vector2){ x, y });

			x = (offset.x + std::sin(360 / size * i * PI / 180) * 20);
			y = (offset.y - std::cos(360 / size * i * PI / 180) * 20);
			far_points.push_back((Vector2){ x, y });
		}
	}
};

int main ()
{
	int width = 1000, height = 500;

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(width, height, "blizzard");

	Field field = Field(10, (Vector2){width / 2, height / 2});

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("SCORE: ", 10, 10, 25, WHITE);
		field.Update(GetFrameTime());
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
