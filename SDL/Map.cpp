#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS.h"
#include "Components.h"

extern Manager manager;

Map::Map(const char* mapfile, int mscale, int tilesize) :mapfilePath(mapfile), mapScale(mscale), tileSize(tilesize) {
	scaledSize = mscale * tilesize;
}

Map::~Map() {

}

void Map::LoadMap(std::string path, int sizeX, int sizeY, int p_level) {
	level = p_level;
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			srcX = atoi(&c) * tileSize;
			AddTile(srcX, 0, x * scaledSize, y * scaledSize);
			if (srcX / tileSize == 1) {
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("elev", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			}
			else if (srcX / tileSize == 2) {
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("ladd", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			}
			
			mapFile.ignore();
		}
	}
	
	mapFile.ignore();

	for (int y = -1; y < sizeY; y++) {
		for (int x = -1; x < sizeX; x++) {
			mapFile.get(c);
			if (c == '1') {
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain",(x + 1) * scaledSize, y * scaledSize, scaledSize / 5, scaledSize, level);
				tcol.addGroup(Game::groupColliders);
			}
			mapFile.get(c);
			if (c == '1') {
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, (y + 1) * scaledSize, scaledSize + scaledSize / 5, scaledSize / 5, level);
				tcol.addGroup(Game::groupColliders);
			}
			mapFile.ignore();
		}
	}

	mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, mapfilePath);
	tile.addGroup(Game::groupMap);
}