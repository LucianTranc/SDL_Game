#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include "UIStatistics.h"
#include <sstream>
#include <cmath>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 800, 640 };

AssetManager* Game::assets = new AssetManager(&manager);

//UIStatistics* Game::statManager = new UIStatistics(&manager);

auto& player(manager.addEntity());

auto& positionLabel(manager.addEntity());
auto& velocityLabel(manager.addEntity());
auto& keysLabel(manager.addEntity());
auto& mousePosLabel(manager.addEntity());
auto& angleLabel(manager.addEntity());
	

bool Game::isRunning = false;


Game::Game() {

}
Game::~Game() {

}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {

	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		std::cout << "Subsystems initialized!...\n";
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window Created\n";
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created\n";
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}

	assets->AddTexture("terrain", "assets/terrain_ss.png", renderer);
	assets->AddTexture("player", "assets/player_anims.png", renderer);
	assets->AddTexture("projectile", "assets/proj.png", renderer);

	assets->AddFont("arial", "assets/arial.ttf", 16);

	map = new Map("terrain", 3, 32);

	map->LoadMap("assets/map.map", 25, 20);

	player.addComponent<TransformComponent>(800.0f, 640.0f, 32, 32, 4);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - 400 + 64);
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - 320 + 64);

	//->newStat(10, 110, "testlabel");
	

	SDL_Color white = {255, 255, 255, 255};
	positionLabel.addComponent<UILabel>(10, 10, "Test String", "arial", white);
	velocityLabel.addComponent<UILabel>(10, 30, "Test String", "arial", white);
	keysLabel.addComponent<UILabel>(10, 50, "Test String", "arial", white);
	mousePosLabel.addComponent<UILabel>(10, 70, "Test String", "arial", white);
	angleLabel.addComponent<UILabel>(10, 90, "Test String", "arial", white);

	//assets->CreateProjectile(Vector2D(600, 600), Vector2D(1 ,0), 400, 2, "projectile");
	assets->CreateProjectile(Vector2D(1000, 600), Vector2D(sin(1.4), cos(1.4)), 400, 2, "projectile");
	//assets->CreateProjectile(Vector2D(400, 600), Vector2D(1, 1), 400, 2, "projectile");
	//assets->CreateProjectile(Vector2D(600, 400), Vector2D(1, -1), 400, 2, "projectile");
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& uilabels(manager.getGroup(Game::groupUILables));

void Game::handleEvents() {

	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
	

}

void Game::update() {

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	Vector2D playerVel = player.getComponent<TransformComponent>().velocity;
	bool up = player.getComponent<KeyboardController>().up;
	bool left = player.getComponent<KeyboardController>().left;
	bool down = player.getComponent<KeyboardController>().down;
	bool right = player.getComponent<KeyboardController>().right;
	int mousex = player.getComponent<KeyboardController>().mousex;
	int mousey = player.getComponent<KeyboardController>().mousey;
	double mouseangle = std::atan2(camera.x + mousex - playerPos.x - 64, camera.y + mousey - playerPos.y - 64);


	std::stringstream pos;
	std::stringstream vel;
	std::stringstream keys;
	std::stringstream mousepos;
	std::stringstream angle;
	pos << "Player position: " << playerPos << ", Camera position: (" << camera.x << ", " << camera.y << ")";
	positionLabel.getComponent<UILabel>().SetLabelText(pos.str(), "arial");
	vel << "Player velocity: " << playerVel;
	velocityLabel.getComponent<UILabel>().SetLabelText(vel.str(), "arial");
	keys << "up: " <<  up << ", left: " << left << ", down: " << down << ", right: "<< right; 
	keysLabel.getComponent<UILabel>().SetLabelText(keys.str(), "arial");
	mousepos << "mouse position: (" << mousex << ", " << mousey << ")";
	mousePosLabel.getComponent<UILabel>().SetLabelText(mousepos.str(), "arial");
	angle << "mouse direction angle: " << mouseangle;
	angleLabel.getComponent<UILabel>().SetLabelText(angle.str(), "arial");


	//statManager->updateStat("testlabel", "testlabel", "testdata");


	manager.refresh();
	manager.update();

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	/*for (auto& p : projectiles)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			std::cout << "Hit player\n";
			p->destory();
		}
	}*/

	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - 400 + 64);
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - 320 + 64);

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

}


void Game::render() 
{
	SDL_RenderClear(renderer);

	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& c : colliders)
	{
		c->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}

	for (auto& p : projectiles)
	{
		p->draw();
		//SDL_Rect destRect = p->getComponent<ColliderComponent>().destR;
		//std::cout << "destR: x = " << destRect.x << ", y = " << destRect.y << ", w = " << destRect.w << ", h = " << destRect.h << std::endl;
	}

	for (auto& u : uilabels)
	{
		u->draw();
	}

	

	positionLabel.draw();
	velocityLabel.draw();
	keysLabel.draw();
	mousePosLabel.draw();
	angleLabel.draw();



	SDL_RenderPresent(renderer);
}

void Game::clean() {

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned\n";
}

