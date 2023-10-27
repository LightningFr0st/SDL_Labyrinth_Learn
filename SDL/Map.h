#pragma once
#include <string>

class Map {
public:
	Map(const char* mapfile, int mscale, int tilesize);
	~Map();
	void LoadMap(std::string path, int sizeX, int sizeY, int p_level);
	void AddTile(int srcX, int srcY, int xpos, int ypos);
private:
	const char* mapfilePath;
	int mapScale;
	int tileSize;
	int scaledSize;
	int level;
};

