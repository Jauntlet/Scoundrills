/* Purpose: Instance(s) will handle the UI and Resource management aspects of when you arrive at a Navigation Destination
 * Main Contributor(s): Christopher Kowalewski
 */
#pragma once

#include "src/drill/PlayerResources.h"

#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/UI/UIButton.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UISprite.h>
#include <Jauntlet/UI/UIText.h>
#include <random>

class PlayerManager;

class Cavern {
public:
	Cavern(PlayerResources* resourceManager, Camera2D* Camera);

	void setType(int type);
	void display();
	void updateResources();

	UIManager* getUIManager();

	bool isOpen();

	void setPlayerManager(PlayerManager* manager);

	void hide();
private:
	//random
	std::mt19937 _random;

	//camera
	Camera2D* _camera;

	//necessary managers
	UIManager _uiManager;
	PlayerResources* _resources;
	PlayerManager* _playerManager;
	
	//textures / text & color
	GLuint _backgroundTexture = 0;
	GLuint _confirmTexture = 0;
	std::string _description = "";
	Color _descriptionColor = Color("FFFFFF");
	
	//UI elements
	UISprite _backgroundElement;
	UIButton _confirmButton;
	UIText _descriptionElement;
	
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