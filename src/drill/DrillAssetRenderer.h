#include <Jauntlet/Rendering/SpriteBatch.h>
#include <Jauntlet/Rendering/ResourceManager.h>

class DrillAssetRenderer {
public:
	DrillAssetRenderer();
	// initialize the drill renderer
	void init();
	// draw the assets on screen
	void draw();
private:
	Jauntlet::SpriteBatch _spriteBatch;
};