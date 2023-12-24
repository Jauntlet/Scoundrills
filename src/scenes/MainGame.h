/* Purpose: Initializes all parts of the game and runs the main game loop.
 * Main Contributer(s): Xander Mooney / Jack Kennedy / Christopher Kowalewski
 */
#pragma once
#include "../CameraManager.h"
#include "../drill/DrillManager.h"
#include <Jauntlet/Jauntlet.h>
#include <Jauntlet/Rendering/Window.h>
#include <Jauntlet/Time.h>
#include "../pathfinding/SelectedTileRenderer.h"
#include "../UICoordinator.h"

class MainGame {
public:
	MainGame(const std::vector<uint8_t>& playerIDs);
	MainGame(int saveID);

	void windowResized();
	void gameLoop();
private:
	void processInput();
	void drawGame();
	void drawHUD();

	// called whenever the window is resized.

	Jauntlet::Camera2D _camera = Jauntlet::Camera2D(GlobalContext::screenSize.x, GlobalContext::screenSize.y),
					   _hudCamera = Jauntlet::Camera2D(GlobalContext::screenSize.x, GlobalContext::screenSize.y);

	PlayerResources _resources;

	DrillManager _drill = DrillManager(&_cameraManager, _resources, &_hudCamera);
	
	UICoordinator _uiCoordinator = UICoordinator(&_hudCamera, &_drill);

	CameraManager _cameraManager = CameraManager(&_camera, &_players, &_drill);

	PlayerManager _players = PlayerManager(&_drill);

	SelectedTileRenderer _selectedTile = SelectedTileRenderer(&_drill, &_players);
	
	Jauntlet::SpriteBatch _playerSpriteBatch;

	// defines scale of movement for the camera. if set to 1, the camera will follow the mouse, if set to 0, the mouse has no control over the camera.
	const float _CAMERA_MOVEMENT_SCALE = 0.5f;
};