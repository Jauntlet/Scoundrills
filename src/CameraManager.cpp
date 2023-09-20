#include "CameraManager.h"
#include "Jauntlet/TileMap.h"
#include "src/players/PlayerManager.h"
#include <Jauntlet/Time.h>

CameraManager::CameraManager() {
	// Empty
}

void CameraManager::init(Jauntlet::Camera2D* camera, Jauntlet::InputManager* inputManager, PlayerManager* players, Jauntlet::TileMap* level) {
	_camera = camera;
	_inputManager = inputManager;

	_players = players;
	_level = level;

	_moveDown.init(inputManager);
	_moveLeft.init(inputManager);
	_moveUp.init(inputManager);
	_moveRight.init(inputManager);

	_moveDown.addKey(SDLK_s, SDLK_DOWN);
	_moveLeft.addKey(SDLK_a, SDLK_LEFT);
	_moveUp.addKey(SDLK_w, SDLK_UP);
	_moveRight.addKey(SDLK_d, SDLK_RIGHT);
}

void CameraManager::processInput() {
	if (_inputManager->isKeyDown(SDLK_r)) {
		_camera->transitionToPosition(glm::vec2(0));
		_camera->transitionToScale(1);
	}

	if (_inputManager->isKeyDown(SDL_BUTTON_LEFT)) {
		if (!_inputManager->isKeyPressed(SDL_BUTTON_LEFT) || !_players->processInput(_inputManager, _camera, _level)) {
			_camera->clearTransitions();
			_deltaMouse = glm::vec2(_oldMouse.x - _inputManager->getMouseCoords().x, _inputManager->getMouseCoords().y - _oldMouse.y);
		}
	}
	else {
		_deltaMouse -= _deltaMouse * (Jauntlet::Time::getDeltaTime() * 10);
	}

	if (_moveLeft.isDown()) {
		_deltaMouse.x -= 3 * Jauntlet::Time::getDeltaTime();
	}
	if (_moveRight.isDown()) {
		_deltaMouse.x += 3 * Jauntlet::Time::getDeltaTime();
	}
	if (_moveUp.isDown()) {
		_deltaMouse.y += 3 * Jauntlet::Time::getDeltaTime();
	}
	if (_moveDown.isDown()) {
		_deltaMouse.y -= 3 * Jauntlet::Time::getDeltaTime();
	}


	_camera->translate(_deltaMouse);

	glm::vec2 rStick = _inputManager->getControllerAxis(Jauntlet::Axis::RightStick);
	if (glm::abs(rStick.x) > .2 || glm::abs(rStick.y) > .2) {
		_camera->translate(glm::vec2(rStick.x, -rStick.y) * (250.0f * Jauntlet::Time::getDeltaTime()));
	}


	if (_inputManager->deltaScroll != 0) {
		_camera->clearTransitions();

		float zoom = pow(1.25f, _inputManager->deltaScroll);

		glm::vec2 mouse = _camera->convertScreenToWorldDisreguardPosition(_inputManager->getMouseCoords());

		_camera->translate(mouse);
		_camera->multiply(zoom);
		_camera->translate(-mouse);

		_inputManager->deltaScroll = 0;
	}

	_oldMouse = _inputManager->getMouseCoords(); // the old mouse position is now the current mouse position
}