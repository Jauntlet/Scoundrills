#pragma once
#include "Jauntlet/UI/UITextElement.h"
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include "GlobalContext.h"
#include <Jauntlet/Rendering/TextRenderer.h>

class SceneManager;

class MainMenu
{
public:
	MainMenu(SceneManager* sceneManager);

	void gameLoop();

	void windowResized();
private:
	void startGame();

	bool _switch = false; // true when switching to next scene

	Jauntlet::Camera2D _camera;
	Jauntlet::UIManager _uiManager;

	Jauntlet::UIButtonElement _startButton;
	glm::vec2 _startButtonPos = glm::vec2(0,0);
	
	Jauntlet::Color _textColor = Jauntlet::Color(255,255,255);
	glm::vec2 _titlePosition = glm::vec2(0);
	std::string _titleText = "Scoundrills";
	Jauntlet::UITextElement _titleTextElement = Jauntlet::UITextElement(GlobalContext::textRenderer, &_titleText, &_textColor, &_titlePosition, Jauntlet::UIButtonElement::ORIGIN_PIN::TOP, 0.5f);

	SceneManager* _sceneManager;
};

