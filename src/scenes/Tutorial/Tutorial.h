/* Purpose: Tutorial to teach the player how to play the game, as well as sync with the main game.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/Rendering/Cameras/Camera2D.h>

#include "../../drill/DrillManager.h"
#include "../../drill/PlayerResources.h"
#include "../../pathfinding/SelectedTileRenderer.h"
#include "../GlobalContext.h"
#include "../MainGame/CameraManager.h"
#include "Dialogue.h"
#include "Jauntlet/Rendering/ProgressBar.h"
#include "Officer.h"
#include "TutorialUI.h"

class Tutorial
{
public:
	Tutorial(int saveID, const std::vector<uint8_t>& playerIDs);

	void windowResized();
	void gameLoop();

	void nextDialogue();
private:
	void processInput();
	void drawGame();
	void drawHUD();

	Camera2D _camera = Camera2D(GlobalContext::screenSize), _hudCamera = Camera2D(GlobalContext::screenSize);

	PlayerResources _resources = PlayerResources(100,0,0,0);

	DrillManager _drill = DrillManager(&_cameraManager, _resources, &_hudCamera);

	TutorialUI _uiCoordinator = TutorialUI(&_hudCamera, &_drill);

	CameraManager _cameraManager = CameraManager(&_camera, &_players, &_drill);

	PlayerManager _players = PlayerManager(&_drill);

	SelectedTileRenderer _selectedTile = SelectedTileRenderer(&_drill, &_players);

	SpriteBatch _playerSpriteBatch;

	InputKey _toggleNavButton = InputKey(&GlobalContext::inputManager);

	// defines scale of movement for the camera. if set to 1, the camera will follow the mouse, if set to 0, the mouse has no control over the camera.
	const float _CAMERA_MOVEMENT_SCALE = 0.5f;
	
	// Tutorial Specific data
	uint32_t _sequence = 0;
	Dialogue _dialogue = Dialogue(&_hudCamera);
	bool _hasControl = false;
	Officer _officer = Officer(glm::vec2(7 * 64, -18 * 64), "Textures/Cops/John Police.png");
	std::vector<ProgressBar> _displayPlayers;

	bool useControllerPrompts = false;
	bool _pressedW = false, _pressedA = false, _pressedS = false, _pressedD = false, _scrolledUp = false, _scrolledDown = false;

	int _saveID;
};