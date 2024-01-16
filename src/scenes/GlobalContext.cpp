#include "GlobalContext.h"
#include "Jauntlet/Rendering/Window.h"
#include "PauseMenu.h"
#include "SceneManager.h"

SceneManager* GlobalContext::sceneManager;
TextureCache GlobalContext::_textureCache;
GLSLProgram GlobalContext::normalShader;
InputManager GlobalContext::inputManager;
glm::ivec2 GlobalContext::screenSize = glm::ivec2(1024, 768);
Window GlobalContext::window = Window("Scoundrills", screenSize.x, screenSize.y, WindowFlags::RESIZEABLE);
TextRenderer* GlobalContext::textRenderer;
PauseMenu* GlobalContext::pauseMenu;
AudioListener GlobalContext::listener;

void GlobalContext::initContext() {
	window.setWindowIcon("Textures/Icon.png");

    normalShader.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	normalShader.addAttribute("vertexPosition");
	normalShader.addAttribute("vertexColor");
	normalShader.addAttribute("vertexUV");
	normalShader.linkShaders();

	textRenderer = new TextRenderer("Fonts/HandelGo.ttf", 256);
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

std::string GlobalContext::playerIDtoTexture(uint8_t playerID, bool policeOutfit) {
	switch (playerID) {
	case (1):
		return policeOutfit ? "Textures/Cops/Will Sabot Togue.png" : "Textures/Criminals/Will Sabot Togue.png";
	case (2):
		return policeOutfit ? "Textures/Cops/Sean Arson Burnes.png" : "Textures/Criminals/Sean Arson Burnes.png";
	case (3):
		return policeOutfit ? "Textures/Cops/Rob evan truly.png" : "Textures/Criminals/Rob evan truly.png";
	case (4):
		return policeOutfit ? "Textures/Cops/jay walt kins.png" : "Textures/Criminals/jay walt kins.png";
	case (5):
		return policeOutfit ? "Textures/Cops/Robin banks.png" : "Textures/Criminals/Robin banks.png";
	default:
		Jauntlet::error("Player ID " + std::to_string(playerID) + " is not a valid ID!");
		return "Textures/Craig.png";
	}
}