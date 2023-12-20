/* Purpose: Instance(s) will handle the UI and Resource management aspects of when you arrive at a Navigation Destination
 * Main Contributor(s): Christopher Kowalewski
 */
#pragma once

#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/UI/UIManager.h>
#include "../drill/PlayerResources.h"

class Cavern {
public:
	Cavern(PlayerResources* resourceManager, Jauntlet::Camera2D* Camera, int type);

	void setType(int type);
	void display();
	void updateResources();

private:
	Jauntlet::UIManager _uiManager;
	PlayerResources* _resources;
	int _type = 0; //determines what you find -- set by navigation
};