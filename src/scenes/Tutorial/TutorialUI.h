#pragma once
#include "../MainGame/UICoordinator.h"
class TutorialUI : public UICoordinator {
public:
	TutorialUI(Jauntlet::Camera2D* hudCamera, DrillManager* drillManager);

	void hideAll();

	void showWater();
	void showFood();
	void showParts();
	void showTemperature();
	void showDrillButton();
};

