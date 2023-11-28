#pragma once
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>

class SceneManager;

class MainMenu
{
public:
	MainMenu(SceneManager* sceneManager);

	void gameLoop();

	void windowResized();
private:
	void startGame();

	Jauntlet::Camera2D _camera;
	Jauntlet::UIManager _uiManager;

	Jauntlet::UIButtonElement _startButton;
	glm::vec2 _startButtonPos = glm::vec2(20,-10);

	SceneManager* _sceneManager;
};

