#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <cmath>

#define PI 3.1415

class Field
{
private:
	int size = 10;
	std::vector<Vector2> points;
	std::vector<Vector2> far_points;
	int center_x, center_y;

	int player = 0;
	Vector2 player_pos = {0, 0};
	std::vector<Vector2> projectiles;

public:
	void Update(float delta_time)
	{
		Vector2 target_pos;
		for (int i = 0; i < size; i++)
		{
			DrawLineV(points[i], points[(i + 1) % size], WHITE);
			DrawLineV(far_points[i], far_points[(i + 1) % size], GRAY);
			DrawLineV(points[i], far_points[i], GRAY);

			if (i == player)
			{
				target_pos.x = (points[i].x + points[(i + 1) % size].x) / 2;
				target_pos.y = (points[i].y + points[(i + 1) % size].y) / 2;
			}
		}

		if (IsKeyPressed(KEY_LEFT))
			player = (player + size - 1) % size;
		if (IsKeyPressed(KEY_RIGHT))
			player = (player + 1) % size;
		if (IsKeyPressed(KEY_SPACE))
			projectiles.push_back(target_pos);

		player_pos = Vector2Lerp(player_pos, target_pos, delta_time * 10);
		for (Vector2 &projectile : projectiles)
		{
			projectile = Vector2Lerp(projectile, {center_x, center_y}, delta_time * 10);
			if (Vector2Distance(projectile, {center_x, center_y}) > 10)
				DrawCircleV(projectile, 5, YELLOW);
				
		}
		DrawCircleV(player_pos, 10, RED);
	}

	Field(int x, int y, int n) : size(n), center_x(x), center_y(y)
	{
		for (int i = 0; i < size; i++)
		{
			int point_x = (center_x + std::sin(360 / size * i * PI / 180) * 200);
			int point_y = (center_y - std::cos(360 / size * i * PI / 180) * 200);
			points.push_back({ point_x, point_y });

			point_x = (center_x + std::sin(360 / size * i * PI / 180) * 20);
			point_y = (center_y - std::cos(360 / size * i * PI / 180) * 20);
			far_points.push_back({ point_x, point_y });
		}
	}
};

int main ()
{
	int width = 1000, height = 500;

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(width, height, "blizzard");

	Field field = Field(width / 2, height / 2, 10);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("SCORE: ", 10, 10, 30, WHITE);
		field.Update(GetFrameTime());
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
