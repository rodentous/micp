#include "game.hpp"



int HIGH_SCORE = 0;

int main ()
{
	constexpr int width = 1000, height = 800;

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(width, height, "tempest");
	InitAudioDevice();

	Game game = Game((Vector2){width / 2, height / 2});
	enum State {
		Main, Gameplay, Over
	};
	State state = State::Main;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		switch (state)
		{
			case Main:
			{
				DrawText("TEMPEST\n --press enter to start--", 10, 10, 25, WHITE);
				
				if (IsKeyPressed(KEY_ENTER))
					state = State::Gameplay;
			} break;
			case Gameplay:
			{
				game.update(GetFrameTime());
				if (game.health <= 0)
				{
					if (game.score > HIGH_SCORE) HIGH_SCORE = game.score;
					state = State::Over;
				}
		
				DrawText(TextFormat("SCORE: %d", game.score), 10, 10, 25, WHITE);
				DrawText(TextFormat("HIGH SCORE: %d", HIGH_SCORE), 10, 40, 25, WHITE);
				DrawText(TextFormat("HP: %d", game.health), 10, 70, 25, WHITE);
			} break;
			case Over:
			{				
				DrawText("GAME OVER\n --press enter--", 10, 10, 25, WHITE);
				DrawText(TextFormat("SCORE: %d", game.score), 10, 70, 25, WHITE);
				DrawText(TextFormat("HIGH SCORE: %d", HIGH_SCORE), 10, 100, 25, WHITE);
				
				if (IsKeyPressed(KEY_ENTER))
				{
					game = Game((Vector2){width / 2, height / 2});
					state = State::Main;
				}
			} break;
			default: break;
		}

		EndDrawing();
	}

	CloseWindow();
	CloseAudioDevice();
	return 0;
}
