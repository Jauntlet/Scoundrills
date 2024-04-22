#include "CameraManager.h"
#include "Jauntlet/Errors.h"
#include "Jauntlet/Inputs/InputManager.h"
#include "Jauntlet/Time.h"
#include "src/interactable/PlayerStation.h"
#include "src/pathfinding/SelectedTileRenderer.h"
#include "src/players/Player.h"
#include "src/scenes/GlobalContext.h"
#include "../PauseMenu.h"

#undef max

const float CAMERA_SPEED = 500;

CameraManager::CameraManager(Camera2D* camera, PlayerManager* players, DrillManager* drill) :
	_moveDown(&GlobalContext::inputManager),
	_moveLeft(&GlobalContext::inputManager),
	_moveUp(&GlobalContext::inputManager),
	_moveRight(&GlobalContext::inputManager),
	_camera(camera),
	_players(players),
	_drill(drill)
{
	_moveDown.addKey({SDLK_s, SDLK_DOWN, CONTROLLER_DPAD_DOWN});
	_moveLeft.addKey({SDLK_a, SDLK_LEFT, CONTROLLER_DPAD_LEFT});
	_moveUp.addKey({SDLK_w, SDLK_UP, CONTROLLER_DPAD_UP});
	_moveRight.addKey({SDLK_d, SDLK_RIGHT, CONTROLLER_DPAD_RIGHT});
}

void CameraManager::processInput() {
	if (GlobalContext::pauseMenu->isPaused()) return;

	// ~ KEYBOARD MOVEMENT

	if (GlobalContext::inputManager.isKeyDown(SDLK_r)) {
		_camera->transitionToPosition(glm::vec2(24 * 64 * 0.5f, 30 * 64 * 0.5f * -1));
		_camera->transitionToScale(0.5f);
	}

	bool clickOnPlayers = _players->processInput(*_camera);
	
	if (cameraUnlocked) {
		if (GlobalContext::inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
			//GlobalContext::usingController = false;
			if (!clickOnPlayers) {
				_camera->clearTransitions();
				_deltaMouse = Jauntlet::Time::getTimeScale() * glm::vec2(_oldMouse.x - GlobalContext::inputManager.getMouseCoords().x, GlobalContext::inputManager.getMouseCoords().y - _oldMouse.y);
			}
		} else {
			_deltaMouse -= _deltaMouse * (Jauntlet::Time::getDeltaTime() * 10);
		}

		if (_moveLeft.isDown()) {
			_camera->translate(glm::vec2(-CAMERA_SPEED * Jauntlet::Time::getDeltaTime(), 0));
			_camera->clearTransition(Camera2D::TRANSITION_TYPE::POSITION);
			GlobalContext::usingController = false;
		}
		
		if (_moveRight.isDown()) {
			_camera->translate(glm::vec2(CAMERA_SPEED * Jauntlet::Time::getDeltaTime(), 0));
			_camera->clearTransition(Camera2D::TRANSITION_TYPE::POSITION);
			GlobalContext::usingController = false;
		}
		
		if (_moveUp.isDown()) {
			_camera->translate(glm::vec2(0, CAMERA_SPEED * Jauntlet::Time::getDeltaTime()));
			_camera->clearTransition(Camera2D::TRANSITION_TYPE::POSITION);
			GlobalContext::usingController = false;
		}
		
		if (_moveDown.isDown()) {
			_camera->translate(glm::vec2(0, -CAMERA_SPEED * Jauntlet::Time::getDeltaTime()));
			_camera->clearTransition(Camera2D::TRANSITION_TYPE::POSITION);
			GlobalContext::usingController = false;
		}

		_camera->translate(_deltaMouse * Jauntlet::Time::getTimeScale());

		if (GlobalContext::inputManager.deltaScroll != 0) {
			GlobalContext::usingController = false;
			_camera->clearTransitions();

			float zoom = pow(1.1f, GlobalContext::inputManager.deltaScroll);

			_camera->multiply(zoom);
		}

		// ~ CONTROLLER MOVEMENT ~
		
		if (GlobalContext::inputManager.getControllerCount() > 0) {
			glm::vec2 leftAxis = GlobalContext::inputManager.getControllerAxis(Axis::LeftStick);

			if (abs(leftAxis.x) > 0.2 || abs(leftAxis.y) > 0.2) {
				GlobalContext::usingController = true;
				_camera->clearTransition(Camera2D::TRANSITION_TYPE::POSITION);
				_camera->translate(glm::vec2(CAMERA_SPEED * Jauntlet::Time::getDeltaTime() * (abs(leftAxis.x) < 0.2 ? 0 : leftAxis.x), -CAMERA_SPEED * Jauntlet::Time::getDeltaTime() * (abs(leftAxis.y) < 0.2 ? 0 : leftAxis.y)));
			}
			
			glm::vec2 rightAxis = GlobalContext::inputManager.getControllerAxis(Axis::RightStick);

			if (abs(rightAxis.x) > 0.8 || abs(rightAxis.y) > 0.8) {
				GlobalContext::usingController = true;
				
				if (_rightAxisPressed) {
					return;
				}

				_rightAxisPressed = true;
				glm::vec2 cameraCenter = _camera->getPosition() * (1.0f / _camera->getScale());
				float closestDistance = std::numeric_limits<float>::max();
				
				if (!_players->isPlayerSelected()) {
					// Selecting Players
					std::vector<Player*> players = _players->getAllPlayers();
					
					Player* selectedPlayer = nullptr;
					for (Player* player : players) {
						if (cameraCenter - player->getPosition() == glm::vec2(32,32)) {
							continue;
						}

						if (rightAxis.x > 0.8) {
							// Testing against all players to the right
							if (player->getPosition().x > cameraCenter.x && player->getPosition().x - cameraCenter.x < closestDistance) {
								closestDistance = player->getPosition().x - cameraCenter.x;
								selectedPlayer = player;
							}
						} else if (rightAxis.x < -0.8) {
							// Testing against all players to the left
							if (player->getPosition().x < cameraCenter.x && cameraCenter.x - player->getPosition().x < closestDistance) {
								closestDistance = cameraCenter.x - player->getPosition().x;
								selectedPlayer = player;
							}
						} else if (rightAxis.y < -0.8) {
							// Testing against all players above
							if (player->getPosition().y > cameraCenter.y && player->getPosition().y - cameraCenter.y < closestDistance) {
								closestDistance = player->getPosition().y - cameraCenter.y;
								selectedPlayer = player;
							}
						} else if (rightAxis.y > 0.8) {
							// Testing against all players below
							if (player->getPosition().y < cameraCenter.y && cameraCenter.y - player->getPosition().y < closestDistance) {
								closestDistance = cameraCenter.y - player->getPosition().y;
								selectedPlayer = player;
							}
						}
					}
					if (selectedPlayer != nullptr) {
						_camera->setPosition((selectedPlayer->getPosition() + glm::vec2(32, 32)) / (1.0f / _camera->getScale()));
					} 
				} else {
					// Selecting stations
					std::vector<PlayerStation*> stations = _drill->getAllStations();
					PlayerStation* selectedStation = nullptr;

					for (PlayerStation* station : stations) {
						if (station == nullptr || station->isOccupied()) {
							continue;
						}

						if (rightAxis.x > 0.8) {
							// Testing against all stations to the right
							if (station->getAnchorPoint().x > cameraCenter.x && station->getAnchorPoint().x - cameraCenter.x < closestDistance) {
								closestDistance = station->getAnchorPoint().x - cameraCenter.x;
								selectedStation = (PlayerStation*)station;
							}
						} else if (rightAxis.x < -0.8) {
							// Testing against all stations to the left
							if (station->getAnchorPoint().x < cameraCenter.x && cameraCenter.x - station->getAnchorPoint().x < closestDistance) {
								closestDistance = cameraCenter.x - station->getAnchorPoint().x;
								selectedStation = (PlayerStation*)station;
							}
						} else if (rightAxis.y < -0.8) {
							// Testing against all stations above
							if (station->getAnchorPoint().y > cameraCenter.y && station->getAnchorPoint().y - cameraCenter.y < closestDistance) {
								closestDistance = station->getAnchorPoint().y - cameraCenter.y;
								selectedStation = (PlayerStation*)station;
							}
						} else if (rightAxis.y > 0.8) {
							// Testing against all stations below
							if (station->getAnchorPoint().y < cameraCenter.y && cameraCenter.y - station->getAnchorPoint().y < closestDistance) {
								closestDistance = cameraCenter.y - station->getAnchorPoint().y;
								selectedStation = (PlayerStation*)station;
							}
						}
					}
					if (selectedStation != nullptr) {
						_camera->setPosition((selectedStation->getAnchorPoint() /*+ glm::vec2(32, 32)*/) / (1.0f / _camera->getScale()));
					}
				}
			} else {
				_rightAxisPressed = false;
			}

			glm::vec2 triggerAxis = GlobalContext::inputManager.getControllerAxis(Axis::Triggers); 
			triggerAxis = glm::vec2((triggerAxis.x + 1) / 2, (triggerAxis.y + 1) / 2);

			if (triggerAxis.x > 0.5 || triggerAxis.y > 0.5) {
				GlobalContext::usingController = true;
				_camera->clearTransitions();

				float zoom = pow(1.001f, glm::sign(triggerAxis.x - triggerAxis.y));
				if (zoom != 0) {
					_camera->multiply(zoom);
				}
			}
		}
	}
	
	_oldMouse = GlobalContext::inputManager.getMouseCoords(); // the old mouse position is now the current mouse position
}

void CameraManager::doExplosionShake() {
	_cameraShakeCooldown = 5.0f;
}

void CameraManager::update() {
	if (_cameraShakeCooldown > 0) {
		_cameraShakeCooldown -= Jauntlet::Time::getDeltaTime();
	}
}