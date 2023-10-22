/* Purpose: Manages inputs directed towards the camera and any other info the camera needs to function.
 * Main Contributer(s): Jack Kennedy / Xander Mooney
 */
#pragma once

#include "drill/DrillManager.h"
#include "Jauntlet/TileMap.h"
#include "src/players/PlayerManager.h"
#include <Jauntlet/Camera2D.h>
#include <Jauntlet/InputManager.h>
#include <Jauntlet/InputKey.h>

class CameraManager {
public:
	CameraManager(Jauntlet::Camera2D* camera, Jauntlet::InputManager* inputManager, PlayerManager* players, DrillManager* drill);

	void processInput();
private:
	Jauntlet::Camera2D* _camera;
	Jauntlet::InputManager* _inputManager;

	PlayerManager* _players;
	DrillManager* _drill;

	Jauntlet::InputKey _moveLeft;
	Jauntlet::InputKey _moveRight;
	Jauntlet::InputKey _moveUp;
	Jauntlet::InputKey _moveDown;


	glm::vec2 _oldMouse = glm::vec2(0);
	glm::vec2 _deltaMouse = glm::vec2(0);
};