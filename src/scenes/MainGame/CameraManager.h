/* Purpose: Manages inputs directed towards the camera and any other info the camera needs to function.
 * Main Contributer(s): Jack Kennedy / Xander Mooney
 */
#pragma once

#include "src/drill/DrillManager.h"
#include "src/pathfinding/SelectedTileRenderer.h"
#include "src/players/PlayerManager.h"
#include <Jauntlet/Rendering/Cameras/Camera2D.h>
#include <Jauntlet/Inputs/InputKey.h>

class CameraManager {
public:
	CameraManager(Camera2D* camera, PlayerManager* players, DrillManager* drill);

	void processInput();

	void doExplosionShake();

	void update();


	bool cameraUnlocked = true;
private:
	Camera2D* _camera;

	PlayerManager* _players;
	DrillManager* _drill;

	InputKey _moveLeft;
	InputKey _moveRight;
	InputKey _moveUp;
	InputKey _moveDown;

	bool _rightAxisPressed = false;

	glm::vec2 _oldMouse = glm::vec2(0);
	glm::vec2 _deltaMouse = glm::vec2(0);

	float _cameraShakeCooldown = 5.0f;
};