#include "GlobalContext.h"
#include "Jauntlet/Rendering/Window.h"
#include "PauseMenu.h"
#include "SceneManager.h"

SceneManager* GlobalContext::sceneManager;
Jauntlet::TextureCache GlobalContext::_textureCache;
Jauntlet::GLSLProgram GlobalContext::normalShader;
Jauntlet::InputManager GlobalContext::inputManager;
glm::ivec2 GlobalContext::screenSize = glm::ivec2(1024, 768);
Jauntlet::Window GlobalContext::window = Jauntlet::Window("Scoundrills", screenSize.x, screenSize.y, Jauntlet::WindowFlags::RESIZEABLE);
Jauntlet::TextRenderer* GlobalContext::textRenderer;
PauseMenu* GlobalContext::pauseMenu;
Jauntlet::AudioListener GlobalContext::listener;

void GlobalContext::initContext() {
	window.setWindowIcon("Textures/Icon.png");

    normalShader.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	normalShader.addAttribute("vertexPosition");
	normalShader.addAttribute("vertexColor");
	normalShader.addAttribute("vertexUV");
	normalShader.linkShaders();

	textRenderer = new Jauntlet::TextRenderer("Fonts/HandelGo.ttf", 256);
	sceneManager = new SceneManager();
	sceneManager->gameLoop();
}
void GlobalContext::destroyContext() {
	if (pauseMenu != nullptr) {
		delete pauseMenu;
	}
	if (textRenderer != nullptr) {
		delete textRenderer;
	}
	if (sceneManager != nullptr) {
		delete sceneManager;
	}
}

std::string GlobalContext::playerIDtoTexture(uint8_t playerID) {
	switch (playerID) {
	case (1):
		return "Textures/Criminals/Will Sabot Togue.png";
	case (2):
		return "Textures/Criminals/Sean Arson Burnes.png";
	case (3):
		return "Textures/Criminals/Rob evan truly.png";
	case (4):
		return "Textures/Criminals/jay walt kins.png";
	case (5):
		return "Textures/Criminals/Robin banks.png";
	default:
		Jauntlet::error("Player ID " + std::to_string(playerID) + " is not a valid ID!");
		return "Textures/Craig.png";
	}
}