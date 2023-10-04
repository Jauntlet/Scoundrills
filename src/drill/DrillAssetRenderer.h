#include <Jauntlet/Rendering/SpriteBatch.h>
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Rendering/Animation.h>

class DrillAssetRenderer {
public:
	DrillAssetRenderer();
	// initialize the drill renderer
	void init();
	// draw the assets on screen
	void draw();

	// stop the drill animation
	void startAnimation();
	// start the drill animation
	void stopAnimation();
private:
	Jauntlet::SpriteBatch _spriteBatch;
	Jauntlet::Animation _drillAnimation;

	bool _shake = false;
};