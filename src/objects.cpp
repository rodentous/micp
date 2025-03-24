#include "game.hpp"


Object::Object(Edge* e) : edge(e)
{
	dead = false;
	position.x = 0;
	position.y = 0;
}

Player::Player(Edge* e) : Object(e)
{
	edge = e;
	move_sound = LoadSound("sounds/move.wav");
	shoot_sound = LoadSound("sounds/shoot.wav");
	SetSoundVolume(shoot_sound, 0.25);

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

	// input
	if (IsKeyPressed(KEY_LEFT))
		move_left();
	if (IsKeyPressed(KEY_RIGHT))
		move_right();
	if (IsKeyPressed(KEY_SPACE))
		shoot();
}

void Player::shoot()
{
	Game& game = Game::get_instance();
	game.objects.push_back(std::make_unique<Projectile>(edge, position));
	PlaySound(shoot_sound);
}

void Player::move_left()
{
	edge = edge->left;
	PlaySound(move_sound);
}

void Player::move_right()
{
	edge = edge->right;
	PlaySound(move_sound);
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
			PlaySound(game.explosion_sound);
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
	{
		game.lose_health();
		dead = true;
	}
}

bool Enemy::collide(Vector2 pos, int r)
{
    return CheckCollisionCircleLine(pos, r, position, position2);
}