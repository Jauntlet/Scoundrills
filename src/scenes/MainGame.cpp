#include "GlobalContext.h"
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/Rendering/Textures/TextureCache.h>
#include <Jauntlet/UI/UIElement.h>

#include <Jauntlet/Rendering/Particles/Particle.h>
#include <Jauntlet/Rendering/Particles/Properties/ParticleProperty.h>
#include <Jauntlet/Time.h>
#include "MainGame.h"
#include "src/UICoordinator.h"

#include <Jauntlet/Rendering/Particles/Properties/ParticleGrow.h>

const float PLAYER_HURT_HEAT = 200.0f; // The minimum heat for players to take damage from it.

MainGame::MainGame() :
	_camera(GlobalContext::screenSize.x, GlobalContext::screenSize.y),
	_hudCamera(GlobalContext::screenSize.x, GlobalContext::screenSize.y),
	_drill(&_cameraManager, _resources, &_hudCamera),
	_cameraManager(&_camera, &_players, &_drill),
	_players(3, &_drill),
	_selectedTile(&_drill, &_players),
	_textRenderer(&_hudCamera, "Fonts/HandelGo.ttf", 256),
	_uiCoordinator(&_hudCamera, &_textRenderer, &_drill),
	_resources(100,100,0,200)
{
	GlobalContext::window.setBackgroundColor(Jauntlet::Color(97, 60, 47));
	_uiCoordinator.applyNewScreenSize(glm::ivec2(GlobalContext::screenSize.x, GlobalContext::screenSize.y));
}

void MainGame::gameLoop() {
	processInput();

	_camera.update();
	_hudCamera.update();
	_drill.update();

	drawGame();
}

void MainGame::processInput() {
	_players.update(_drill);

	if (_resources.heat > PLAYER_HURT_HEAT) {
		_players.damageTick(_resources.heat);
	}

	_cameraManager.processInput();

	//open nav
	if (GlobalContext::inputManager.isKeyPressed(SDLK_EQUALS)) {
		_uiCoordinator.navigation->toggleNav();
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
	if (_uiCoordinator.navigation->isNavOpen()) {
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

	_selectedTile.draw(&_camera);

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