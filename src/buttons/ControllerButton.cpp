#include "ControllerButton.h"

void ControllerButton::setButtons(ControllerButton* leftButton, ControllerButton* rightButton, ControllerButton* downButton, ControllerButton* upButton) {
    _leftButton = leftButton;
    _rightButton = rightButton;
    _downButton = downButton;
    _upButton = upButton;
}

ControllerButton* ControllerButton::selectRightButton() {
    if (_rightButton != nullptr) {
        _rightButton->highlighted = true;
        highlighted = false;
        return _rightButton;
    } else return this;
}
ControllerButton* ControllerButton::selectLeftButton() {
    if (_leftButton != nullptr) {
        _leftButton->highlighted = true;
        highlighted = false;
        return _leftButton;
    } else return this;
}
ControllerButton* ControllerButton::selectUpButton() {
    if (_upButton != nullptr) {
        _upButton->highlighted = true;
        highlighted = false;
        return _upButton;
    } else return this;
}
ControllerButton* ControllerButton::selectDownButton() {
    if (_downButton != nullptr) {
        _downButton->highlighted = true;
        highlighted = false;
        return _downButton;
    } else return this;
}

void ControllerButton::select() {
    highlighted = true;
}