#include "main.hpp"



int main ()
{
	float width = 1024, height = 1024;

	SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_WINDOW_HIGHDPI);
	SetTargetFPS(60);
	InitWindow(width, height, "tempest");
	InitAudioDevice();

	Game& game = Game::get_instance();
	game.Start();

	Shader shader = LoadShader(0, "src/shader.fs");
	RenderTexture2D target = LoadRenderTexture(width, height);

	while (!WindowShouldClose())
	{
		// render everything on target texture
		BeginTextureMode(target);
			ClearBackground(WHITE);
			game.Update();
		EndTextureMode();

		// draw target texture on screen with shader applied
		BeginDrawing();
			BeginShaderMode(shader);
			ClearBackground(GREEN);
			DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, Vector2Zero(), WHITE);
			EndShaderMode();
		EndDrawing();
	}

	CloseWindow();
	CloseAudioDevice();
	return 0;
}