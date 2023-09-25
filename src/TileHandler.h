#pragma once
#include <Jauntlet/TileMap.h>

class TileHandler
{
public:
	TileHandler();

	void addTile(std::string filePath);
	void addTileSet(std::string filePath);

	void loadFile(std::string filePath);

	void draw();
private:
	Jauntlet::TileMap _tileMap;

	// stored for saving the map at the end.
	std::vector<std::string> _tileInfo;
	std::vector<std::vector<std::string>> _levelInfo;

	// required to compile tilemap
	Jauntlet::TextureCache _textureCache;
};

