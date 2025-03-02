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
	int player = 0;
	Vector2 player_pos = {0, 0};
	Vector2 target_pos;

public:
	void Update(float delta_time)
	{
		if (IsKeyPressed(KEY_LEFT))
			player = (player + size - 1) % size;
		if (IsKeyPressed(KEY_RIGHT))
			player = (player + 1) % size;

		for (int i = 0; i < size; i++)
		{
			DrawLineV(points[i], points[(i + 1) % size], WHITE);

			if (i == player)
			{
				target_pos.x = (points[i].x + points[(i + 1) % size].x) / 2;
				target_pos.y = (points[i].y + points[(i + 1) % size].y) / 2;
			}
		}

		player_pos = Vector2Lerp(player_pos, target_pos, delta_time * 10);
		DrawCircleV(player_pos, 10, RED);
	}

	Field(int center_x, int center_y, int n) : size(n)
	{
		int point_x, point_y;
		for (int i = 0; i < size; i++)
		{
			point_x = (center_x + std::sin(360 / size * i * PI / 180) * 200);
			point_y = (center_y - std::cos(360 / size * i * PI / 180) * 200);
			points.push_back({ point_x, point_y });
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
