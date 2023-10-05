#pragma once
#include <Jauntlet/Camera2D.h>
#include <Jauntlet/InputKey.h>
class CameraManager
{
public:
	CameraManager();
	void init(glm::ivec2 windowSize, Jauntlet::InputManager* inputManager);

	void Update();
	void ActivateCamera();
	void updateCameraSize(int width, int height);
	
	Jauntlet::Camera2D camera;
private:
	Jauntlet::InputKey _moveUp;
	Jauntlet::InputKey _moveDown;
	Jauntlet::InputKey _moveLeft;
	Jauntlet::InputKey _moveRight;

	Jauntlet::InputKey _zoomIn;
	Jauntlet::InputKey _zoomOut;
};

