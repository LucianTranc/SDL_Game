#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS/ECS.h"
#include "ECS/Components.h"

extern Manager manager;

Map::Map( std::string tID, int ms, int ts) :texID(tID), mapScale(ms), tileSize(ts)
{
	scaledSize = ms * ts;
}

Map::~Map()
{

}

void Map::LoadMap(std::string path, int sizeX, int sizeY) {

	char tile, c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++) {

		for (int x = 0; x < sizeX; x++) {
			mapFile.get(tile);
			//std::cout << "tile1: " << tile << std::endl;
			srcY = atoi(&tile) * tileSize;
			mapFile.get(tile);
			//std::cout << "tile2: " << tile << std::endl;
			srcX = atoi(&tile) * tileSize;
			AddTile(srcX, srcY, x*(tileSize*mapScale), y*(tileSize*mapScale));
			mapFile.ignore();

		}

		mapFile.ignore();


	}

	mapFile.ignore();
	mapFile.ignore();

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			//std::cout << "c: " << c << ", x: " << x <<", y: "<< y << "\n";
			if (c == '1')
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			}
			mapFile.ignore();
		}
		mapFile.ignore();

	}

	mapFile.close();

}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos) {

	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
	tile.addGroup(Game::groupMap);
	
}



