#include <iostream> // remove when done debugging
#include <Jauntlet/Time.h>

#include "CameraManager.h"
#include "Jauntlet/Camera2D.h"
#include "Jauntlet/TileMap.h"
#include "src/players/PlayerManager.h"

const float CAMERA_SPEED = 500;

CameraManager::CameraManager(Jauntlet::Camera2D* camera, Jauntlet::InputManager* inputManager, PlayerManager* players, Jauntlet::TileMap* level) {
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
		_camera->transitionToPosition(glm::vec2(24 * 64 * 0.5f, 30 * 64 * 0.5f * -1));
		_camera->transitionToScale(0.5f);
	}

	bool clickOnPlayers = _players->processInput(_inputManager, _camera, _level);
	
	if (_inputManager->isKeyDown(SDL_BUTTON_LEFT)) {
		if (!clickOnPlayers) {
			_camera->clearTransitions();
			_deltaMouse = glm::vec2(_oldMouse.x - _inputManager->getMouseCoords().x, _inputManager->getMouseCoords().y - _oldMouse.y);
		}
	}
	else {
		_deltaMouse -= _deltaMouse * (Jauntlet::Time::getDeltaTime() * 10);
	}

	if (_moveLeft.isDown()) {
		_camera->translate(glm::vec2(-CAMERA_SPEED * Jauntlet::Time::getDeltaTime(), 0));
		_camera->clearTransition(Jauntlet::Camera2D::TRANSITION_TYPE::POSITION);
	}
	if (_moveRight.isDown()) {
		_camera->translate(glm::vec2(CAMERA_SPEED * Jauntlet::Time::getDeltaTime(), 0));
		_camera->clearTransition(Jauntlet::Camera2D::TRANSITION_TYPE::POSITION);
	}
	if (_moveUp.isDown()) {
		_camera->translate(glm::vec2(0, CAMERA_SPEED * Jauntlet::Time::getDeltaTime()));
		_camera->clearTransition(Jauntlet::Camera2D::TRANSITION_TYPE::POSITION);
	}
	if (_moveDown.isDown()) {
		_camera->translate(glm::vec2(0, -CAMERA_SPEED * Jauntlet::Time::getDeltaTime()));
		_camera->clearTransition(Jauntlet::Camera2D::TRANSITION_TYPE::POSITION);
	}


	_camera->translate(_deltaMouse);

	glm::vec2 rStick = _inputManager->getControllerAxis(Jauntlet::Axis::RightStick);
	if (glm::abs(rStick.x) > .2 || glm::abs(rStick.y) > .2) {
		_camera->translate(glm::vec2(rStick.x, -rStick.y) * (250.0f * Jauntlet::Time::getDeltaTime()));
	}


	if (_inputManager->deltaScroll != 0) {
		_camera->clearTransitions();

		float zoom = pow(1.1f, _inputManager->deltaScroll);

		//glm::vec2 mouse = _camera->convertScreenToWorldDisregardPosition(_inputManager->getMouseCoords());

		_camera->multiply(zoom);

		_inputManager->deltaScroll = 0;
	}

	_oldMouse = _inputManager->getMouseCoords(); // the old mouse position is now the current mouse position
}