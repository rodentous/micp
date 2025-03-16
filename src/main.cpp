#include "game.hpp"



int HIGH_SCORE = 0;

int main ()
{
	constexpr int width = 1000, height = 800;

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(width, height, "blizzard");

	Game game = Game((Vector2){width / 2, height / 2});

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("TEMPEST\n --press enter to start--", 10, 10, 25, WHITE);
		
		if (IsKeyPressed(KEY_ENTER))
			break;

		EndDrawing();
	}
	if (WindowShouldClose())
	{
		CloseWindow();
		return 0;
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		game.update(GetFrameTime());
		if (game.health <= 0)
		{
			if (game.score > HIGH_SCORE) HIGH_SCORE = game.score;
			break;
		}

		DrawText(TextFormat("SCORE: %d", game.score), 10, 10, 25, WHITE);
		DrawText(TextFormat("HIGH SCORE: %d", HIGH_SCORE), 10, 40, 25, WHITE);
		DrawText(TextFormat("HP: %d", game.health), 10, 70, 25, WHITE);

		EndDrawing();
	}
	if (WindowShouldClose())
	{
		CloseWindow();
		return 0;
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("GAME OVER\n --press enter--", 10, 10, 25, WHITE);
		
		if (IsKeyPressed(KEY_ENTER))
		{
			CloseWindow();
			main();
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
