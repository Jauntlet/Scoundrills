/* Purpose: The scene for allowing the player to select the Crew they want to take on their journey.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/UI/UIManager.h>
#include <Jauntlet/Audio/AudioSource.h>
#include "../GlobalContext.h"

class SceneManager;

class RogueGallery {
public:
	RogueGallery(SceneManager* sceneManager);

	void windowResized();
	void gameLoop();
private:
	Jauntlet::Camera2D _camera = Jauntlet::Camera2D(GlobalContext::screenSize);
	Jauntlet::UIManager _uiManager = Jauntlet::UIManager(&_camera);

	Jauntlet::AudioSource _music = Jauntlet::AudioSource(glm::vec3(0,0,0));
};

