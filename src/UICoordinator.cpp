#include <functional>
#include <Jauntlet/UI/UIButtonToggleableElement.h>

#include "UICoordinator.h"
#include "src/scenes/GlobalContext.h"
#include <Jauntlet/Rendering/TextRenderer.h>
#include <Jauntlet/JMath.h>
#include <string>

UICoordinator::UICoordinator(Jauntlet::Camera2D* hudCamera, Jauntlet::TextRenderer* textRenderer, DrillManager* drillManager) :
	_hudCamera(hudCamera),
	_textRenderer(textRenderer),
	_UIManager(hudCamera),
	_drill(drillManager),
	navigation(&drillManager->navigation),
	_waterIconTextElement(_textRenderer, &waterIconText, &_textColor, &_waterIconTextPosition, 0.25f),
	_foodIconTextElement(_textRenderer, &foodIconText, &_textColor, &_foodIconTextPosition, 0.25f),
	_partsIconTextElement(_textRenderer, &partsIconText, &_textColor, &_partsIconTextPosition, 0.25f),
	_tempIconTextElement(_textRenderer, &tempIconText, &_textColor, &_tempIconTextPosition, 0.25f)
{
	_NavManager = navigation->getUIManager();

	_UIManager.addElement(&_waterIconTextElement, &Jauntlet::TextRenderer::textShader);
	_UIManager.addElement(&_foodIconTextElement, &Jauntlet::TextRenderer::textShader);
	_UIManager.addElement(&_partsIconTextElement,&Jauntlet::TextRenderer::textShader);
	_UIManager.addElement(&_tempIconTextElement, &Jauntlet::TextRenderer::textShader);
	_UIManager.addElement(&_waterIcon, &GlobalContext::normalShader);
	_UIManager.addElement(&_foodIcon, &GlobalContext::normalShader);
	_UIManager.addElement(&_partsIcon, &GlobalContext::normalShader);
	_UIManager.addElement(&_tempIcon, &GlobalContext::normalShader);

	GLuint _buttonTexture = Jauntlet::ResourceManager::getTexture("Textures/button.png").id;
	glm::vec2* buttonPos = new glm::vec2(10, 10);

	// conversion from `void` to `std::function<void ()>` -jk
	std::function<void()> _buttonMethod = std::bind(&DrillManager::toggle, drillManager);

	Jauntlet::UIButtonToggleableElement* _button = new Jauntlet::UIButtonToggleableElement(&GlobalContext::inputManager, _buttonMethod, _buttonTexture, buttonPos, glm::vec2(256, 256), Jauntlet::UIElement::ORIGIN_PIN::BOTTOM_LEFT);
	_UIManager.addElement(_button, &GlobalContext::normalShader);

	
	// optimize batches
	_UIManager.optimize();
	_UIManager.resolvePositions();

	_NavManager->optimize();
	_NavManager->resolvePositions();
}

UICoordinator::~UICoordinator() {
	delete buttonPos;
	delete _button;
}

void UICoordinator::draw() {
	waterIconText = JMath::Split(std::to_string(std::min(_drill->resources.water, 999.0f)), '.')[0];
	foodIconText = std::to_string(std::min(_drill->resources.food, (unsigned int)999));
	partsIconText = std::to_string(_drill->resources.copper);

	tempIconText = std::to_string(_drill->resources.heat).substr(0, 5);

	_UIManager.draw();
	navigation->update();
	_NavManager->draw();
}

void UICoordinator::applyNewScreenSize(glm::ivec2 screenSize) {
	_UIManager.setScale(((screenSize.y / 1080.0f)));
	_UIManager.resolvePositions();

	_NavManager->setScale(((screenSize.y / 1080.0f)));
	_NavManager->resolvePositions();
}