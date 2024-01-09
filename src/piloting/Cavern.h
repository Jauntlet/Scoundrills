/* Purpose: Instance(s) will handle the UI and Resource management aspects of when you arrive at a Navigation Destination
 * Main Contributor(s): Christopher Kowalewski
 */
#pragma once

#include "src/drill/PlayerResources.h"

#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UISpriteElement.h>
#include <Jauntlet/UI/UITextElement.h>
#include <random>

class PlayerManager;

class Cavern {
public:
	Cavern(PlayerResources* resourceManager, Jauntlet::Camera2D* Camera);

	void setType(int type);
	void display();
	void updateResources();

	Jauntlet::UIManager* getUIManager();

	bool isOpen();

	void setPlayerManager(PlayerManager* manager);

	void hide();
private:
	//random
	std::mt19937 _random;

	//camera
	Jauntlet::Camera2D* _camera;

	//necessary managers
	Jauntlet::UIManager _uiManager;
	PlayerResources* _resources;
	PlayerManager* _playerManager;
	
	//textures / text & color
	GLuint _backgroundTexture = 0;
	GLuint _confirmTexture = 0;
	std::string _description = "";
	Jauntlet::Color _descriptionColor = Jauntlet::Color("FFFFFF");
	
	//UI elements
	Jauntlet::UISpriteElement _backgroundElement;
	Jauntlet::UIButtonElement _confirmButton;
	Jauntlet::UITextElement _descriptionElement;
	
	//UI positions
	glm::vec2 _backgroundPos = glm::vec2(0);
	glm::vec2 _confirmPos = glm::vec2(0, 380);
	glm::vec2 _descriptionPos = glm::vec2(0, 150);
	
	//cavern-specific
	int _type = 0; //determines what you find -- set by navigation
	bool _open = false;

	//resources added upon "accepting" the pop-up
	int _addHeat, _addFood, _addWater, _addCopper = 0;
};