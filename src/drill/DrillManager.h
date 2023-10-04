#include "DrillAssetRenderer.h"

#include <Jauntlet/TileMap.h>
class DrillManager {
public:
	DrillManager();
	
	// initialize the drill elements
	void init();
	// draw the drill elements
	void draw();

	// turn on the drill
	void on();
	// turn off the drill
	void off();

	Jauntlet::TileMap drillWalls;
	Jauntlet::TileMap drillFloor;
private:
	DrillAssetRenderer _drillAssets;

	Jauntlet::TextureCache _textureCache;

	bool _drillOn = true;
};