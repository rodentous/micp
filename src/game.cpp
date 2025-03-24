#include "game.hpp"

#include <cmath>
#include <random>



Object::Object(Edge* e) : edge(e)
{
	dead = false;
	position.x = 0;
	position.y = 0;
}

Player::Player(Edge* e) : Object(e)
{
	edge = e;

	radius = 10;
	speed = 10;

	if (!edge) return;
	position.x = (edge->A.x + edge->B.x) / 2;
	position.y = (edge->A.y + edge->B.y) / 2;
}

void Player::update(float delta_time)
{
	// calculate target position
	Vector2 target_pos;
	target_pos.x = (edge->A.x + edge->B.x) / 2;
	target_pos.y = (edge->A.y + edge->B.y) / 2;

	// move and draw
	position = Vector2Lerp(position, target_pos, delta_time * speed);
	DrawCircleV(position, radius, YELLOW);
}



Projectile::Projectile(Edge* e, Vector2 pos) : Object(e)
{
	edge = e;

	radius = 5;
	speed = 500;

	position = pos;
}

void Projectile::update(float delta_time)
{
	Game& game = Game::get_instance();

	// calculate target position
	Vector2 target_pos;
	target_pos.x = (edge->A2.x + edge->B2.x) / 2;
	target_pos.y = (edge->A2.y + edge->B2.y) / 2;

	// move and draw
	position = Vector2MoveTowards(position, target_pos, delta_time * speed);
	DrawCircleV(position, radius, SKYBLUE);

	for (auto& object : game.objects)
	{
		if (dynamic_cast<Enemy*>(object.get()) && dynamic_cast<Enemy*>(object.get())->collide(position, radius))
		{
			object->dead = true;
			dead = true;
			game.score_points();
			game.objects.push_back(std::make_unique<Explosion>(edge, position, SKYBLUE, 30));
			PlaySound(game.boom_sound);
			return;
		}
	}

	// projectile should be destroyed
	if (!dead)
		dead = Vector2Distance(position, target_pos) < 1;
}


Explosion::Explosion(Edge* e, Vector2 pos, Color c, int r) : Object(e)
{
	edge = e;

	radius = r;
	color = c;
	lifetime = 1;
	position = pos;
	speed = 0;
}

void Explosion::update(float delta_time)
{
	lifetime -= delta_time * 2;
	radius -= delta_time * 10;

	DrawCircleV(position, radius, color);

	dead = lifetime < 0;
}


Enemy::Enemy(Edge* e, int s) : Object(e)
{
	edge = e;

	radius = 15;
	speed = s;

	if (!edge) return;
	position = edge->A2;
	position2 = edge->B2;
	edging = false;
}


void Enemy::update(float delta_time)
{
	Game& game = Game::get_instance();

	// if (Vector2Scale(Vector2Subtract(edge->A2, edge->A), ))
	// jump to adjacent edges
	if (GetRandomValue(0, 400))
		edge = edge->left;
	if (GetRandomValue(0, 400))
		edge = edge->right;

	// draw lines
	DrawLineV(position, position2, RED);
	DrawCircleV(position, 5, RED);
	DrawCircleV(position2, 5, RED);

	// is on outer vertex
	if (Vector2Distance(position, edge->A) < 1 || Vector2Distance(position2, edge->B) < 1)
		edging = true;

	// move
	position = Vector2MoveTowards(position, edge->A, delta_time * speed);
	position2 = Vector2MoveTowards(position2, edge->B, delta_time * speed);

	if (collide(game.player.position, game.player.radius))
		game.lose_health();
}

bool Enemy::collide(Vector2 pos, int r)
{
    return CheckCollisionCircleLine(pos, r, position, position2);
}


void Game::generate()
{
	std::vector<Vector2> verticies, inner_verticies; // verticies of the shape

	// reset everything
	edges.clear();
	level_transition = 0.5;
	int size = score / 1000 + 5; // number of points

	// star
	if (score / 1000 % 2 == 1)
	{
		for (int i = 0; i < size; i++)
		{
			float angle = DEG2RAD * 360.0f / size * i;

			// outer verticies
			float outer_radius = (i % 2 == 0) ? 40.0f : 20.0f;
			float x = offset.x + sin(angle) * outer_radius;
			float y = offset.y - cos(angle) * outer_radius;
			verticies.push_back((Vector2){x, y});

			// inner verticies
			float inner_radius = (i % 2 == 0) ? 4.0f : 2.0f;
			x = center.x + sin(angle) * inner_radius;
			y = center.y - cos(angle) * inner_radius;
			inner_verticies.push_back((Vector2){x, y});
		}
	}
	// regular polygon
	else
	{
		for (int i = 0; i < size; i++)
		{
			float angle = DEG2RAD * 360.0f / size * i;

			float x = offset.x + sin(angle) * 40.0f;
			float y = offset.y - cos(angle) * 40.0f;
			verticies.push_back((Vector2){x, y});

			x = center.x + sin(angle) * 4.0f;
			y = center.y - cos(angle) * 4.0f;
			inner_verticies.push_back((Vector2){x, y});
		}
	}

	// create edges
	for (int i = 0; i < size; i++)
		edges.push_back((Edge){nullptr, nullptr, verticies[i], verticies[(i + 1) % size], inner_verticies[i], inner_verticies[(i + 1) % size]});

	// connect edges
	for (int i = 0; i < size; i++)
	{
		edges[i].right = &edges[(i + 1) % size];
		edges[(i + 1) % size].left = &edges[i];
	}
	player.edge = &edges[size/2];
}


void Game::next_level()
{
	level_transition = 2; // start animation timer
	objects.clear();
}


void Game::transition(float delta_time)
{
	level_transition -= delta_time * 0.9;

	// when animation ends, generate new level
	if (level_transition <= 1.1 && level_transition > 1)
		generate();

	// move everything on screen
	for (Edge &edge : edges)
	{
		edge.A = Vector2Lerp(edge.A, Vector2Add(edge.A, Vector2Subtract(edge.A, offset)), delta_time * 4);
		edge.B = Vector2Lerp(edge.B, Vector2Add(edge.B, Vector2Subtract(edge.B, offset)), delta_time * 4);
		edge.A2 = Vector2Lerp(edge.A2, Vector2Add(edge.A2, Vector2Subtract(edge.A2, center)), delta_time * 4);
		edge.B2 = Vector2Lerp(edge.B2, Vector2Add(edge.B2, Vector2Subtract(edge.B2, center)), delta_time * 4);
	}

	draw();
}


void Game::draw()
{
	// draw normal lines
	for (Edge &edge : edges)
	{
		DrawLineV(edge.A, edge.B, BLUE);
		DrawLineV(edge.A2, edge.B2, BLUE);
		DrawLineV(edge.A, edge.A2, BLUE);
		DrawLineV(edge.B, edge.B2, BLUE);
	}

	// draw highlighted lines
	DrawLineV(player.edge->A, player.edge->B, YELLOW);
	DrawLineV(player.edge->A2, player.edge->B2, YELLOW);
	DrawLineV(player.edge->A, player.edge->A2, YELLOW);
	DrawLineV(player.edge->B, player.edge->B2, YELLOW);
}


void Game::update(float delta_time)
{
	// level transition (if happening)
	if (level_transition > 0)
	{
		transition(delta_time);
		return;
	}

	// draw level
	draw();

	// offset
	Vector2 new_offset = Vector2Add(offset, Vector2Scale(Vector2Subtract(player.position, offset), 0.1));
	for (Edge &edge : edges)
	{
		edge.A = Vector2Add(new_offset, Vector2Scale(Vector2Subtract(edge.A2, offset), 10));
		edge.B = Vector2Add(new_offset, Vector2Scale(Vector2Subtract(edge.B2, offset), 10));
	}

	// move player, projectiles and enemies
	player.update(delta_time);

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->dead)
		{
			objects.erase(objects.begin() + i);
			continue;
		}
		objects[i]->update(delta_time);
	}


	// input
	if (IsKeyPressed(KEY_LEFT))
	{
		player.edge = player.edge->left;
		PlaySound(move_sound);
	}
	if (IsKeyPressed(KEY_RIGHT))
	{
		player.edge = player.edge->right;
		PlaySound(move_sound);
	}
	if (IsKeyPressed(KEY_SPACE))
	{
		objects.push_back(std::make_unique<Projectile>(player.edge, player.position));
		PlaySound(shot_sound);
	}
	if (IsKeyPressed(KEY_BACKSPACE))
		score += 1000;

	// spawn enemies
	if (GetRandomValue(0, 200 - score / 50) == 0)
		objects.push_back(std::make_unique<Enemy>(&edges[GetRandomValue(0, edges.size() - 1)], 50 + score / 50));
}


void Game::lose_health()
{
	health--;
	objects.push_back(std::make_unique<Explosion>(player.edge, player.position, RED, 50));
	PlaySound(hurt_sound);
}


void Game::score_points()
{
	score += 100;
	if (score % 1000 == 0)
		next_level();
}


Game::Game()
{
	center = (Vector2){1920/2, 1080/2};
	offset = Vector2Add(center, (Vector2){0, 10});
	player = Player(nullptr);

	health = 3;
	score = 0;
	level_transition = 0;

	move_sound = LoadSound("sounds/move.wav");
	shot_sound = LoadSound("sounds/shot.wav");
	SetSoundVolume(shot_sound, 0.25);
	boom_sound = LoadSound("sounds/boom.wav");
	hurt_sound = LoadSound("sounds/hurt.wav");
	SetSoundVolume(hurt_sound, 5);

	// TODO: more sounds
	// over_sound = LoadSound("sounds/over.wav");
	// main_music = LoadSound("sounds/main.wav");
	// game_music = LoadSound("sounds/game.wav");

	generate();
}
