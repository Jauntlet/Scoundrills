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

class Cavern {
public:
	Cavern(PlayerResources* resourceManager, Jauntlet::Camera2D* Camera);

	void setType(int type);
	void display();
	void updateResources();

	Jauntlet::UIManager* getUIManager();

private:
	//necessary managers
	Jauntlet::UIManager _uiManager;
	PlayerResources* _resources;
	
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
	glm::vec2 _descriptionPos = glm::vec2(150, 600);
	
	//cavern-specific
	int _type = 0; //determines what you find -- set by navigation
};