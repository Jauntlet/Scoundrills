#include "GlobalContext.h"
#include "Jauntlet/Rendering/Window.h"
#include "PauseMenu.h"

Jauntlet::TextureCache GlobalContext::_textureCache;
Jauntlet::GLSLProgram GlobalContext::normalShader;
Jauntlet::InputManager GlobalContext::inputManager;
glm::ivec2 GlobalContext::screenSize = glm::ivec2(1024, 768);
Jauntlet::Window GlobalContext::window = Jauntlet::Window("Scoundrills", screenSize.x, screenSize.y, Jauntlet::WindowFlags::RESIZEABLE);
PauseMenu GlobalContext::pauseMenu = PauseMenu(_textureCache);

void GlobalContext::initContext() {
	window.setWindowIcon("Textures/Icon.png");

    normalShader.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	normalShader.addAttribute("vertexPosition");
	normalShader.addAttribute("vertexColor");
	normalShader.addAttribute("vertexUV");
	normalShader.linkShaders();
}