#pragma once
#include <Jauntlet/TileMap.h>

class TileHandler
{
public:
	TileHandler();

	void addTile(std::string filePath);
	void addTileSet(std::string filePath);
	void addFunction(std::function<void(int, int)> function);
private:
	int _currentChar;
	
	Jauntlet::TileMap _tileMap;

	// required to compule tilemap
	Jauntlet::TextureCache _textureCache;
};

