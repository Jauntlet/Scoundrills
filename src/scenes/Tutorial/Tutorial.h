#pragma once
#include <Jauntlet/Rendering/Cameras/Camera2D.h>
#include "../../drill/DrillManager.h"
#include "../../drill/PlayerResources.h"
#include "TutorialUI.h"
#include "../MainGame/CameraManager.h"
#include "../../pathfinding/SelectedTileRenderer.h"
#include "../GlobalContext.h"
#include "Dialogue.h"
#include "Officer.h"
class Tutorial
{
public:
	Tutorial(const std::vector<uint8_t>& playerIDs);

	void windowResized();
	void gameLoop();
private:
	void processInput();
	void drawGame();
	void drawHUD();


	Jauntlet::Camera2D _camera = Jauntlet::Camera2D(GlobalContext::screenSize),
		_hudCamera = Jauntlet::Camera2D(GlobalContext::screenSize);

	PlayerResources _resources;

	DrillManager _drill = DrillManager(&_cameraManager, _resources, &_hudCamera);

	TutorialUI _uiCoordinator = TutorialUI(&_hudCamera, &_drill);

	CameraManager _cameraManager = CameraManager(&_camera, &_players, &_drill);

	PlayerManager _players = PlayerManager(&_drill);

	SelectedTileRenderer _selectedTile = SelectedTileRenderer(&_drill, &_players);


	Jauntlet::SpriteBatch _playerSpriteBatch;

	// defines scale of movement for the camera. if set to 1, the camera will follow the mouse, if set to 0, the mouse has no control over the camera.
	const float _CAMERA_MOVEMENT_SCALE = 0.5f;
	
	// Tutorial Specific data
	Dialogue _dialogue = Dialogue(&_hudCamera);
	bool _cameraLocked = true;
	Officer _officer = Officer(glm::vec2(7 * 64, -19 * 64));
};

