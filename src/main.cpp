#include "game.hpp"

int main ()
{
	int width = 1000, height = 800;

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(width, height, "blizzard");

	Game game = Game((Vector2){width / 2, height / 2});

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		game.update(GetFrameTime());
		DrawText(TextFormat("SCORE: %d", game.score), 10, 10, 25, WHITE);
		DrawText(TextFormat("HP: %d", game.health), 10, 40, 25, WHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
