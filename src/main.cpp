#include "game.hpp"
#include <string>
#include <fstream>



void save_score(int value)
{
	std::fstream data("data.lol", std::ios::out | std::ios::in | std::ios::app);
	if (!data.is_open())
		return;
	data << value << std::endl;
	data.close();
}
int load_score()
{
	std::fstream data("data.lol", std::ios::out | std::ios::in | std::ios::app);
	int value = 0;
	if (!data.is_open())
		return 0;
	while (data >> value);
	data.close();
	return value;
}

int main ()
{
	float width = 1920, height = 1080;

	SetConfigFlags(FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(120);
	InitWindow(width, height, "tempest");
	InitAudioDevice();

	Game& game = Game::get_instance();
	enum State {Main, Gameplay, Over, Paused}; State state = State::Main;
	int HIGH_SCORE = load_score();

	// shader
	RenderTexture2D target = LoadRenderTexture(width, height);
	Shader bloom = LoadShader(0, "src/bloom.fs");

	while (!WindowShouldClose())
	{
		BeginTextureMode(target);
		ClearBackground(BLACK);

		switch (state)
		{
			case Main:
			{
				Vector2 title = (Vector2){width / 2, height / 4};
				Rectangle button = (Rectangle){width / 2 - 200, height / 1.5f, 400, 100};

				int size = 10;
				for (int i = 0; i < size; i++)
				{
					float text_width = MeasureText("TEMPEST", 100 + i*10);
					Vector2 pos = Vector2Add(title, Vector2Scale(Vector2Subtract(GetMousePosition(), Vector2Add(title, (Vector2){-2*text_width, 0})), 0.1 / size * i));
					DrawText("TEMPEST", pos.x - i*20 - text_width/2, pos.y - i*5, 100 + i*10, i != size - 1 ? ColorAlpha(WHITE, 1.0 / size * (size - i)) : RED);
				}

				if (CheckCollisionPointRec(GetMousePosition(), button))
				{
					DrawRectangleRounded(button, 0.1, 10, WHITE);
					float text_width = MeasureText("START", 50);
					DrawText("START", button.x + button.width/2 - text_width/2, button.y + 30, 50, BLACK);
					if (IsMouseButtonPressed(0))
						state = State::Gameplay;
				}
				else
				{
					DrawRectangleRoundedLines(button, 0.1, 10, WHITE);
					float text_width = MeasureText("START", 50);
					DrawText("START", button.x + button.width/2 - text_width/2, button.y + 30, 50, WHITE);
				}
				DrawText(TextFormat("HIGH SCORE: %d", HIGH_SCORE), 10, 10, 50, WHITE);

				DrawText("MADE BY :(){ :|:& };: TEAM", 10, height-160, 40, WHITE);
				DrawText("HEAD - JULIA G.", 10, height-110, 40, WHITE);
				DrawText("DEVELOPERS - MARK M., ANNA M.", 10, height-60, 40, WHITE);
			} break;
			case Gameplay:
			{
				if (IsKeyPressed(KEY_TAB))
				{
					state = State::Paused;
					break;
				}

				game.update(GetFrameTime());
				if (game.health <= 0)
				{
					if (game.score > HIGH_SCORE)
					{
						HIGH_SCORE = game.score;
						save_score(HIGH_SCORE);
					}
					state = State::Over;
				}

				DrawText(TextFormat("SCORE: %d", game.score), 10, 10, 50, WHITE);
				DrawText(TextFormat("HIGH SCORE: %d", HIGH_SCORE), 10, 80, 50, WHITE);
				DrawText("ARROWS - MOVE", 10, height-160, 40, WHITE);
				DrawText("SPACE - SHOOT", 10, height-110, 40, WHITE);
				DrawText("TAB - PAUSE", 10, height-60, 40, WHITE);
				for (int i = 0; i < game.health; i++) DrawText("#", width - 300 + 100*i, 10, 100, RED);
			} break;
			case Over:
			{
				Vector2 title = (Vector2){width / 2, height / 4};
				Rectangle button = (Rectangle){width / 2 - 200, height / 1.5f, 400, 100};

				int size = 10;
				for (int i = 0; i < size; i++)
				{
					float text_width = MeasureText("GAME OVER", 100 + i*10);
					Vector2 pos = Vector2Add(title, Vector2Scale(Vector2Subtract(GetMousePosition(), Vector2Add(title, (Vector2){-2*text_width, 0})), 0.1 / size * i));
					DrawText("GAME OVER", pos.x - i*20 - text_width/2, pos.y - i*5, 100 + i*10, i != size - 1 ? ColorAlpha(WHITE, 1.0 / size * (size - i)) : RED);
				}

				if (CheckCollisionPointRec(GetMousePosition(), button))
				{
					DrawRectangleRounded(button, 0.1, 10, WHITE);
					float text_width = MeasureText("RESTART", 50);
					DrawText("RESTART", button.x + button.width/2 - text_width/2, button.y + 30, 50, BLACK);
					if (IsMouseButtonPressed(0))
					{
						state = State::Main;
						game.reset();
					}
				}
				else
				{
					DrawRectangleRoundedLines(button, 0.1, 10, WHITE);
					float text_width = MeasureText("RESTART", 50);
					DrawText("RESTART", button.x + button.width/2 - text_width/2, button.y + 30, 50, WHITE);
				}
				DrawText(TextFormat("SCORE: %d", game.score), 10, 10, 50, WHITE);
			} break;
			case Paused:
			{
				DrawText(TextFormat("SCORE: %d", game.score), 10, 10, 50, WHITE);
				DrawText(TextFormat("HIGH SCORE: %d", HIGH_SCORE), 10, 80, 50, WHITE);
				DrawText("ARROWS - MOVE", 10, height-160, 40, WHITE);
				DrawText("SPACE - SHOOT", 10, height-110, 40, WHITE);
				DrawText("TAB - PAUSE", 10, height-60, 40, WHITE);
				for (int i = 0; i < game.health; i++) DrawText("#", width - 300 + 100*i, 10, 100, RED);

				float text_width = MeasureText("PAUSED", 150);
				DrawText("PAUSED", width/2 - text_width/2, height/3, 150, WHITE);
				if (IsKeyPressed(KEY_TAB))
					state = State::Gameplay;
			}
			default: break;
		}

		EndTextureMode();

		// apply shader
		BeginDrawing();
			ClearBackground(BLACK);
			BeginShaderMode(bloom);
			DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 0, 0 }, WHITE);
			EndShaderMode();
		EndDrawing();
	}

	CloseWindow();
	CloseAudioDevice();
	return 0;
}