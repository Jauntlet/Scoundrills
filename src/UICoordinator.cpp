#include <functional>
#include <Jauntlet/UI/UIButtonToggleableElement.h>

#include "UICoordinator.h"
#include "src/scenes/GlobalContext.h"
#include <Jauntlet/Rendering/TextRenderer.h>
#include <Jauntlet/JMath.h>

UICoordinator::UICoordinator(Jauntlet::Camera2D* hudCamera, Jauntlet::TextRenderer* textRenderer, DrillManager* drillManager, PlayerResources* resources)
	:
	_hudCamera(hudCamera),
	_textRenderer(textRenderer),
	_UIManager(hudCamera),
	navigation(hudCamera),
	_playerResources(resources),
	_waterIconTextElement(_textRenderer, &waterIconText, &_textColor, &_waterIconTextPosition, 0.5f)
{
	_NavManager = navigation.genNav();

	_UIManager.addElement(&_waterIconTextElement, &Jauntlet::TextRenderer::textShader);
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
	waterIconText = std::to_string(_playerResources->water).substr(0,1);
	
	_UIManager.draw();
	navigation.update();
	_NavManager->draw();
}

void UICoordinator::applyNewScreenSize(glm::ivec2 screenSize) {
	_UIManager.setScale(((screenSize.y / 1080.0f)));
	_UIManager.resolvePositions();

	_NavManager->setScale(((screenSize.y / 1080.0f)));
	_NavManager->resolvePositions();
}