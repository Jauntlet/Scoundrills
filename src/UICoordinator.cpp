#include <functional>
#include <Jauntlet/UI/UIButtonToggleableElement.h>

#include "Jauntlet/Rendering/Textures/SpriteBatch.h"
#include "UICoordinator.h"
#include <Jauntlet/Rendering/TextRenderer.h>
#include <Jauntlet/JMath.h>
#include <string>

UICoordinator::UICoordinator(Jauntlet::Camera2D* hudCamera, Jauntlet::TextRenderer* textRenderer, DrillManager* drillManager) :
	_hudCamera(hudCamera),
	_textRenderer(textRenderer),
	_UIManager(hudCamera),
	_drill(drillManager),
	navigation(&drillManager->navigation),
	_waterIconTextElement(_textRenderer, &waterIconText, &_textColor, &_waterIconTextPosition, 0.4f),
	_foodIconTextElement(_textRenderer, &foodIconText, &_textColor, &_foodIconTextPosition, 0.4f),
	_partsIconTextElement(_textRenderer, &partsIconText, &_textColor, &_partsIconTextPosition, 0.4f)
{
	_NavManager = navigation->getUIManager();

	_UIManager.addElement(&_waterIcon, &GlobalContext::normalShader);
	_UIManager.addElement(&_foodIcon, &GlobalContext::normalShader);
	_UIManager.addElement(&_partsIcon, &GlobalContext::normalShader);
	_UIManager.addElement(&_tempProgressBar, &GlobalContext::normalShader);

	GLuint _buttonTexture = Jauntlet::ResourceManager::getTexture("Textures/button.png").id;
	glm::vec2* buttonPos = new glm::vec2(10, 10);

	// conversion from `void` to `std::function<void ()>` -jk
	std::function<void()> _buttonMethod = std::bind(&DrillManager::toggle, drillManager);

	Jauntlet::UIButtonToggleableElement* _button = new Jauntlet::UIButtonToggleableElement(&GlobalContext::inputManager, _buttonMethod, _buttonTexture, buttonPos, glm::vec2(256, 256), Jauntlet::UIElement::ORIGIN_PIN::BOTTOM_LEFT);
	_UIManager.addElement(_button, &GlobalContext::normalShader);
	
	// add text elements
	_UIManager.addElement(&_waterIconTextElement, &Jauntlet::TextRenderer::textShader);
	_UIManager.addElement(&_foodIconTextElement, &Jauntlet::TextRenderer::textShader);
	_UIManager.addElement(&_partsIconTextElement, &Jauntlet::TextRenderer::textShader);

	// optimize batches
	_UIManager.optimize();
	_UIManager.resolvePositions();

	//_NavManager->optimize();
	//_NavManager->resolvePositions();
}

UICoordinator::~UICoordinator() {
	delete buttonPos;
	delete _button;
}

void UICoordinator::draw() {
	
	if (waterIconText != JMath::Split(std::to_string(_drill->resources->water), '.')[0]) {
		waterIconText = JMath::Split(std::to_string(_drill->resources->water), '.')[0];
		_waterIconTextPosition.x = _waterIconPosition.x + (120 - GlobalContext::textRenderer->calculateTextSize(waterIconText, glm::vec2(0.4f)).x) * 0.5f;
		_UIManager.resolvePositions();
	}

	if (foodIconText != std::to_string(_drill->resources->food)) {
		foodIconText = std::to_string(_drill->resources->food);
		_foodIconTextPosition.x = _foodIconPosition.x + (120 - GlobalContext::textRenderer->calculateTextSize(foodIconText, glm::vec2(0.4f)).x) * 0.5f;
		_UIManager.resolvePositions();
	}

	if (partsIconText != std::to_string(_drill->resources->copper)) {
		partsIconText = std::to_string(_drill->resources->copper);
		_partsIconTextPosition.x = _partsIconPosition.x + (120 - GlobalContext::textRenderer->calculateTextSize(partsIconText, glm::vec2(0.4f)).x) * 0.5f;
		_UIManager.resolvePositions();
	}

	_tempProgressBar.progress = (_drill->resources->heat / 300) * 0.7 + 0.3;
	
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