#pragma once
#include "../Game.h"
#include "ECS.h"
#include "Components.h"
#include <cmath>

class KeyboardController : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	bool up, left, down, right = false;

	int mousex, mousey;

	bool mousedown = false;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN && Game::event.key.repeat == 0)
		{
			//std::cout << "Game::event.key.repeat: " << unsigned(Game::event.key.repeat) << std::endl;

			switch (Game::event.key.keysym.sym)
			{
				case SDLK_UP:
					up = true;
					break;
				case SDLK_LEFT:
					left = true;
					sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					break;
				case SDLK_DOWN:
					down = true;
					if (!right)
						sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					break;
				case SDLK_RIGHT:
					right = true;
					if (sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
						sprite->spriteFlip = SDL_FLIP_NONE;
					break;
				default:
					break;
			}
			
		}

		if (Game::event.type == SDL_KEYUP && Game::event.key.repeat == 0)
		{
			//std::cout << "Game::event.key.repeat: " << unsigned(Game::event.key.repeat) << std::endl;

			switch (Game::event.key.keysym.sym)
			{
			case SDLK_UP:
				up = false;
				break;
			case SDLK_LEFT:
				left = false;
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_DOWN:
				down = false;
				if (!left)
					sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_RIGHT:
				right = false;
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
				break;
			default:
				break;
			}

		}
		if (Game::event.type == SDL_MOUSEBUTTONDOWN)
		{
			mousedown = true;
		}
		if (Game::event.type == SDL_MOUSEBUTTONUP)
		{
			mousedown = false;
		}
		if (Game::event.type == SDL_MOUSEMOTION)
		{

			SDL_GetMouseState(&mousex, &mousey);

			if (mousedown)
			{
				float mouseangle = std::atan2(Game::camera.x + mousex - transform->position.x - 64, Game::camera.y + mousey - transform->position.y - 64);
				Game::assets->CreateProjectile(Vector2D(transform->position.x + 50, transform->position.y + 64), Vector2D(sin(mouseangle), cos(mouseangle)), 400, 2, "projectile");
			}

		}

		
		updateVelocity();

	}

	void updateVelocity() 
	{

		if ((!left && !right) || (left && right))
		{
			transform->velocity.x = 0;
		}
		else if (left)
		{
			transform->velocity.x = -1;
		}
		else if (right)
		{
			transform->velocity.x = 1;
		}
		

		if ((!up && !down) || (up && down))
		{
			transform->velocity.y = 0;
		}
		else if (up)
		{
			transform->velocity.y = -1;
		}
		else if (down)
		{
			transform->velocity.y = 1;
		}
		
		if (transform->velocity.x != 0 || transform->velocity.y != 0)
		{
			sprite->Play("walk");
		}
		else
		{
			sprite->Play("idle");
		}
		
		transform->angle = std::atan2(transform->velocity.x, transform->velocity.y);

		if (transform->velocity.x != 0)
			transform->velocity.x = std::sin(transform->angle);
		if (transform->velocity.y != 0)
			transform->velocity.y = std::cos(transform->angle);

	}


private:
};