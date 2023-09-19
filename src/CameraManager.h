#pragma once

#include <Jauntlet/Camera2D.h>
#include <Jauntlet/InputManager.h>
#include <Jauntlet/InputKey.h>

class CameraManager {
public:
	CameraManager();

	void init(Jauntlet::Camera2D* camera, Jauntlet::InputManager* inputManager);

	void processInput();
private:
	Jauntlet::Camera2D* _camera;
	Jauntlet::InputManager* _inputManager;

	Jauntlet::InputKey _moveLeft;
	Jauntlet::InputKey _moveRight;
	Jauntlet::InputKey _moveUp;
	Jauntlet::InputKey _moveDown;


	glm::vec2 _oldMouse = glm::vec2(0);
	glm::vec2 _deltaMouse = glm::vec2(0);
};