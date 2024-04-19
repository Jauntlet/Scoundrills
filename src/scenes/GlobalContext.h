/* Purpose: Holds information about the game that is meant to be loaded at all times.
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/Audio/AudioListener.h>
#include <Jauntlet/Inputs/InputManager.h>
#include <Jauntlet/Rendering/GLSLProgram.h>
#include <Jauntlet/Rendering/TextRenderer.h>
#include <Jauntlet/Rendering/Textures/TextureCache.h>
#include <Jauntlet/Rendering/Window.h>

class PauseMenu;
class SceneManager;

 class GlobalContext {
private:
    static TextureCache _textureCache;
public:
    static void initContext();
    static void destroyContext();

    static std::string playerIDtoTexture(uint8_t playerID, bool policeOutfit = false);

    static SceneManager* sceneManager;
    static GLSLProgram normalShader;
    static InputManager inputManager;
    static glm::ivec2 screenSize;
    static Window window;
    static TextRenderer* textRenderer;
    static PauseMenu* pauseMenu;
    static AudioListener listener;
    static bool usingController;
 };