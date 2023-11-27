#include "GlobalContext.h"
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Rendering/TextureCache.h>
#include <Jauntlet/UI/UIElement.h>

#include <Jauntlet/Rendering/Particles/Particle.h>
#include <Jauntlet/Rendering/Particles/Properties/ParticleProperty.h>
#include <Jauntlet/Time.h>
#include "MainGame.h"
#include "src/UICoordinator.h"

#include <Jauntlet/Rendering/Particles/Properties/ParticleGrow.h>

MainGame::MainGame()
	:
	_camera(GlobalContext::screenSize.x, GlobalContext::screenSize.y),
	_hudCamera(GlobalContext::screenSize.x, GlobalContext::screenSize.y),
	_drill(_resources, &_hudCamera),
	_cameraManager(&_camera, &_players, &_drill),
	_players(3, &_drill),
	_selectedTile(&_drill, &_players),
	_textRenderer(&_hudCamera, "Fonts/HandelGo.ttf", 256),
	_uiCoordinator(&_hudCamera, &_textRenderer, &_drill, &_resources)
{
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(97, 60, 47));
	_uiCoordinator.applyNewScreenSize(glm::ivec2(GlobalContext::screenSize.x, GlobalContext::screenSize.y));
}

void MainGame::gameLoop() {
	processInput();

	_camera.update();
	_hudCamera.update();

	drawGame();
}

void MainGame::processInput() {
	_players.update(_drill);
	_cameraManager.processInput();

	//open nav
	if (GlobalContext::inputManager.isKeyPressed(SDLK_EQUALS)) {
		_uiCoordinator.navigation.toggleNav();
	}

	if (GlobalContext::inputManager.isKeyPressed(SDLK_SPACE)) {
		paused = !paused;

		if (paused) {
			Jauntlet::Time::setTimeScale(0.0f);
		} else {
			Jauntlet::Time::setTimeScale(1.0f);
		}
	}

	//mouse hover over navigation
	if (_uiCoordinator.navigation.isNavOpen()) {
		//real
	}
}

void MainGame::drawGame() {	
	GlobalContext::normalShader.use();
	_camera.setActiveCamera();

	_drill.drawLayerOne();

	// Draw the player using a spriteBatch
	_playerSpriteBatch.begin();
	_players.draw(_playerSpriteBatch);
	_playerSpriteBatch.endAndRender();
	
	_drill.drawLayerTwo();

	_selectedTile.draw(&_camera, &GlobalContext::inputManager); // #TODO: make not pass in inputmanager

	GlobalContext::normalShader.unuse();
	
	drawHUD();
}

void MainGame::drawHUD() {
	_hudCamera.setActiveCamera();

	_uiCoordinator.draw();
}

void MainGame::windowResized() {
	_camera.updateCameraSize(GlobalContext::screenSize);
	_hudCamera.updateCameraSize(GlobalContext::screenSize);
	_uiCoordinator.applyNewScreenSize(GlobalContext::screenSize);
}