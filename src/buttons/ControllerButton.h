/* Purpose: Buttons with controller support
 * Main Contributer(s): Xander Mooney
 */
#pragma once

#include <Jauntlet/UI/UIButton.h>

class ControllerButton : public UIButton {
public:
    ControllerButton(InputManager* inputManager, std::function<void()> onClick, GLuint textureId, glm::vec2* position, glm::vec2 size, ORIGIN_PIN positionPinType)
    : UIButton(inputManager, onClick, textureId, position, size, positionPinType) {}

    void setButtons(ControllerButton* leftButton, ControllerButton* rightButton, ControllerButton* downButton, ControllerButton* upButton);

    ControllerButton* selectRightButton();
    ControllerButton* selectLeftButton();
    ControllerButton* selectUpButton();
    ControllerButton* selectDownButton();

    void select();
private:
    ControllerButton* _rightButton = nullptr;
    ControllerButton* _leftButton = nullptr;
    ControllerButton* _downButton = nullptr;
    ControllerButton* _upButton = nullptr;
};