#include "RogueGallery.h"
#include "../SceneManager.h"

RogueGallery::RogueGallery(SceneManager* sceneManager) {
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(200,200,200));

}

void RogueGallery::windowResized() {
	_camera.updateCameraSize(GlobalContext::screenSize);

	_uiManager.setScale(GlobalContext::screenSize.y / 1080.0f);
	_uiManager.resolvePositions();
}
void RogueGallery::gameLoop() {
	_camera.update();
	_music.update();
	_uiManager.draw();
}