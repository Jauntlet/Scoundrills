#include <Jauntlet/Time.h>
#include <Jauntlet/Rendering/Cameras/Camera2D.h>
#include "CameraManager.h"
#include "drill/DrillManager.h"
#include "src/players/PlayerManager.h"
#include "src/scenes/GlobalContext.h"

const float CAMERA_SPEED = 500;

CameraManager::CameraManager(Jauntlet::Camera2D* camera, PlayerManager* players, DrillManager* drill) 
	: _moveDown(&GlobalContext::inputManager),
	_moveLeft(&GlobalContext::inputManager),
	_moveUp(&GlobalContext::inputManager),
	_moveRight(&GlobalContext::inputManager),
	_camera(camera),
	_players(players),
	_drill(drill)
{
	_moveDown.addKey(SDLK_s, SDLK_DOWN);
	_moveLeft.addKey(SDLK_a, SDLK_LEFT);
	_moveUp.addKey(SDLK_w, SDLK_UP);
	_moveRight.addKey(SDLK_d, SDLK_RIGHT);
}

void CameraManager::processInput() {
	if (GlobalContext::inputManager.isKeyDown(SDLK_r)) {
		_camera->transitionToPosition(glm::vec2(24 * 64 * 0.5f, 30 * 64 * 0.5f * -1));
		_camera->transitionToScale(0.5f);
	}

	bool clickOnPlayers = _players->processInput(*_drill, *_camera);
	
	if (GlobalContext::inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		if (!clickOnPlayers) {
			_camera->clearTransitions();
			_deltaMouse = Jauntlet::Time::getTimeScale() * glm::vec2(_oldMouse.x - GlobalContext::inputManager.getMouseCoords().x, GlobalContext::inputManager.getMouseCoords().y - _oldMouse.y);
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


	_camera->translate(_deltaMouse * Jauntlet::Time::getTimeScale());

	glm::vec2 rStick = GlobalContext::inputManager.getControllerAxis(Jauntlet::Axis::RightStick);
	if (glm::abs(rStick.x) > .2 || glm::abs(rStick.y) > .2) {
		_camera->translate(glm::vec2(rStick.x, -rStick.y) * (250.0f * Jauntlet::Time::getDeltaTime()));
	}


	if (GlobalContext::inputManager.deltaScroll != 0) {
		_camera->clearTransitions();

		float zoom = pow(1.1f, GlobalContext::inputManager.deltaScroll);

		//glm::vec2 mouse = _camera->convertScreenToWorldDisregardPosition(_inputManager->getMouseCoords());

		_camera->multiply(zoom);

		GlobalContext::inputManager.deltaScroll = 0;
	}

	_oldMouse = GlobalContext::inputManager.getMouseCoords(); // the old mouse position is now the current mouse position
}