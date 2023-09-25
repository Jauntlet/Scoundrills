#pragma once
#include <Jauntlet/TileMap.h>

class TileHandler
{
public:
	TileHandler();

	void addTile(std::string filePath);
	void addTileSet(std::string filePath);

	void loadLevel(std::string filePath);

	void draw();
private:
	Jauntlet::TileMap _tileMap;

	// required to compule tilemap
	Jauntlet::TextureCache _textureCache;
};

