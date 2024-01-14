#include "TutorialUI.h"

TutorialUI::TutorialUI(Jauntlet::Camera2D* hudCamera, DrillManager* drillManager) :
	UICoordinator(hudCamera, drillManager)
{
	// empty the moving text so that it never renders during the tutorial
	_drillMovingText = "";
}

void TutorialUI::hideAll() {
	_waterIcon.visible = false;
	_waterIconTextElement.visible = false;
	_foodIcon.visible = false;
	_foodIconTextElement.visible = false;
	_partsIcon.visible = false;
	_partsIconTextElement.visible = false;
	_tempProgressBar.visible = false;
	_button->visible = false;
}

void TutorialUI::showWater() {
	_waterIcon.visible = true;
	_waterIconTextElement.visible = true;
}
void TutorialUI::showFood() {
	_foodIcon.visible = true;
	_foodIconTextElement.visible = true;
}
void TutorialUI::showParts() {
	_partsIcon.visible = true;
	_partsIconTextElement.visible = true;
}
void TutorialUI::showTemperature() {
	_tempProgressBar.visible = true;
}

void TutorialUI::showDrillButton() {
	_button->visible = true;
}