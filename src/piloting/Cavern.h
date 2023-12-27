/* Purpose: Instance(s) will handle the UI and Resource management aspects of when you arrive at a Navigation Destination
 * Main Contributor(s): Christopher Kowalewski
 */
#pragma once

#include "src/drill/PlayerResources.h"

#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/UI/UIButtonElement.h>
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/UI/UISpriteElement.h>

class Cavern {
public:
	Cavern(PlayerResources* resourceManager, Jauntlet::Camera2D* Camera);

	void setType(int type);
	void display();
	void updateResources();

	Jauntlet::UIManager* getUIManager();

private:
	Jauntlet::UIManager _uiManager;
	PlayerResources* _resources;
	int _type = 0; //determines what you find -- set by navigation

	GLuint _confirmTexture = 0;
	Jauntlet::UIButtonElement _confirmButton;
	glm::vec2 _confirmPos = glm::vec2(0, 0);
};