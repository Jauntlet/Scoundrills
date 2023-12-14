/* Purpose: Holds information about the game that is meant to be loaded at all times.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Inputs/InputManager.h>
#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/Rendering/Window.h>
#include <Jauntlet/Rendering/Textures/TextureCache.h>
 
class PauseMenu;

 class GlobalContext {
private:
    static Jauntlet::TextureCache _textureCache;
public:
    static void initContext();

    static Jauntlet::GLSLProgram normalShader;
    static Jauntlet::InputManager inputManager;
    static glm::ivec2 screenSize;
    static Jauntlet::Window window;
    static PauseMenu pauseMenu;
 };