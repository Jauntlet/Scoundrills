#include "src/scenes/GlobalContext.h"
#include "Jauntlet/Rendering/Window.h"

Jauntlet::GLSLProgram GlobalContext::normalShader;
Jauntlet::InputManager GlobalContext::inputManager;
glm::ivec2 GlobalContext::screenSize = glm::ivec2(1024, 768);
Jauntlet::Window GlobalContext::window = Jauntlet::Window("Scoundrills", screenSize.x, screenSize.y, Jauntlet::WindowFlags::RESIZEABLE);

void GlobalContext::initContext() {
    window.setBackgroundColor(Jauntlet::Color(97, 60, 47));// 61 3c 2f
	window.setWindowIcon("Textures/Icon.png");

    normalShader.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	normalShader.addAttribute("vertexPosition");
	normalShader.addAttribute("vertexColor");
	normalShader.addAttribute("vertexUV");
	normalShader.linkShaders();
}