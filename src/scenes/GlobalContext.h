/* Purpose: Holds information about the game that is meant to be loaded at all times.
 * Main Contributer(s): Xander Mooney
 */
 #pragma once
 #include <Jauntlet/Inputs/InputManager.h>
 #include <Jauntlet/Rendering/GLSLProgram.h>
 #include <Jauntlet/Rendering/Window.h>

 class GlobalContext {
public:
    static void initContext();

    static Jauntlet::GLSLProgram normalShader;
    static Jauntlet::InputManager inputManager;
    static Jauntlet::Window window;
    static glm::ivec2 screenSize;
 };