#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <cmath>


class Field
{
private:
	int size = 4;
	std::vector<Vector2> points;
	int player = 0;
	Vector2 player_pos = {0, 0};
	Vector2 pos;

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
				pos.x = (points[i].x + points[(i + 1) % size].x) / 2;
				pos.y = (points[i].y + points[(i + 1) % size].y) / 2;
			}
		}

		player_pos = Vector2Lerp(player_pos, pos, delta_time * 10);
		DrawCircleV(player_pos, 10, RED);
	}

	Field(int n) : size(n)
	{
		points = { {100, 100}, {400, 100}, {400, 400}, {100, 400} };
	}
};

int main ()
{
	int width = 1280, height = 800;

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(width, height, "Hello Raylib");

	Field field = Field(4);
	
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("Hello Raylib", 100, 100, 20, WHITE);
		field.Update(GetFrameTime());
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
