#pragma once
#include <Jauntlet/Rendering/Cameras/Camera2D.h>
#include <Jauntlet/Inputs/InputKey.h>
class CameraManager
{
public:
	CameraManager(glm::ivec2 windowSize, Jauntlet::InputManager* inputManager);

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

