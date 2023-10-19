#include "CameraManager.h"

#include <Jauntlet/Time.h>

CameraManager::CameraManager(glm::ivec2 windowSize, Jauntlet::InputManager* inputManager) :
	camera(windowSize.x, windowSize.y),
	_moveDown(inputManager, SDLK_s),
	_moveLeft(inputManager, SDLK_a),
	_moveUp(inputManager, SDLK_w),
	_moveRight(inputManager, SDLK_d),
	_zoomIn(inputManager, SDLK_q),
	_zoomOut(inputManager, SDLK_e)
{
	// Empty
}

void CameraManager::Update() {
	if (_moveDown.isDown()) {
		camera.translate(glm::vec2(0, -400 * Jauntlet::Time::getDeltaTime()));
	}
	if (_moveUp.isDown()) {
		camera.translate(glm::vec2(0, 400 * Jauntlet::Time::getDeltaTime()));
	}
	if (_moveLeft.isDown()) {
		camera.translate(glm::vec2(-400 * Jauntlet::Time::getDeltaTime(), 0));
	}
	if (_moveRight.isDown()) {
		camera.translate(glm::vec2(400 * Jauntlet::Time::getDeltaTime(), 0));
	}

	if (_zoomIn.isDown()) {
		camera.multiplyScale(0.975);
		camera.translate(glm::vec2(0));
	}
	if (_zoomOut.isDown()) {
		camera.multiplyScale(1.025);
		camera.translate(glm::vec2(0));
	}

	camera.update();
}

void CameraManager::ActivateCamera() {
	camera.setActiveCamera(); 
}

void CameraManager::updateCameraSize(int width, int height) {
	camera.updateCameraSize(width, height);
}