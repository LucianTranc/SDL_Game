#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int rng, int sp, Vector2D vel) : range(rng), speed(sp), velocity(vel)
	{}
	~ProjectileComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
		transform->speed = speed;
		//std::cout<< "velocity: " << velocity << std::endl;
	}

	void update() override
	{

		//std::cout << "x pos: " << transform->position.x << "\n";
		//std::cout << "y pos: " << transform->position.y << "\n";

		distance += 1;

		if (distance > range)
		{
			//std::cout << "Out of range\n";
			entity->destory();
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w ||
			transform->position.x < Game::camera.x ||
			transform->position.y > Game::camera.y + Game::camera.h ||
			transform->position.y < Game::camera.y)
		{
			//std::cout << "Game camera: {" << Game::camera.x << ", " << Game::camera.y << ", " <<  Game::camera.w << ", " << Game::camera.h << "}\n";
			//std::cout << "Projectile Position: (" << transform->position.x << ", " << transform->position.y << ")\n";
			//std::cout << "Out of bounds\n";
			entity->destory();
		}

	}

private:

	TransformComponent* transform;

	int range = 0;
	int speed = 0;
	int distance = 0;

	Vector2D velocity;
};