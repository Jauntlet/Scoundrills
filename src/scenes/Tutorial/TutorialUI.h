/* Purpose: Overrides the Main Games UICoordinator to be able to hide specific UI systems as needed.
 * Main Contributer(s): Xander Mooney
 */
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

