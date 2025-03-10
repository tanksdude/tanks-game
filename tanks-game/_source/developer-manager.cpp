#include "developer-manager.h"

#include "constants.h"
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "renderer.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "level-manager.h"

bool DeveloperManager::leftMouse = false;
bool DeveloperManager::rightMouse = false;
int DeveloperManager::insertIndex = 0;

void DeveloperManager::setX(Circle* c, double x) { c->x = x; }
void DeveloperManager::setY(Circle* c, double y) { c->y = y; }
void DeveloperManager::setR(Circle* c, double r) { c->r = r; }

void DeveloperManager::setX(Rect* r, double x) { r->x = x; }
void DeveloperManager::setY(Rect* r, double y) { r->y = y; }
void DeveloperManager::setW(Rect* r, double w) { r->w = w; }
void DeveloperManager::setH(Rect* r, double h) { r->h = h; }

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl3.h"

void DeveloperManager::doMouseStuff() {
	//mouse click (for tank moving)
	leftMouse = ImGui::IsMouseDown(ImGuiMouseButton_Left);
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
		rightMouse = !rightMouse;
	}

	//mouse drag (tank moving)
	auto mousePos = ImGui::GetMousePos();
	int real_x = mousePos.x;
	int real_y = mousePos.y - (Renderer::window_height - Renderer::gamewindow_height);
	if (leftMouse) {
		if (!rightMouse) {
			TankManager::getTank(0)->x = (real_x / double(Renderer::gamewindow_width)) * GAME_WIDTH;
			TankManager::getTank(0)->y = (1 - real_y / double(Renderer::gamewindow_height)) * GAME_HEIGHT;
		} else {
			TankManager::getTank(1)->x = (real_x / double(Renderer::gamewindow_width)) * GAME_WIDTH;
			TankManager::getTank(1)->y = (1 - real_y / double(Renderer::gamewindow_height)) * GAME_HEIGHT;
		}
	}

	//middle click insert stuff
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
		double true_x = (real_x / double(Renderer::gamewindow_width)) * GAME_WIDTH;
		double true_y = (1 - real_y / double(Renderer::gamewindow_height)) * GAME_HEIGHT;
		devInsert(true_x, true_y);
	}

	//middle click scroll through available stuff
	if (ImGui::IsKeyPressed(ImGuiKey_MouseWheelY)) {
		const int insertIndexMax = insertListIdentifiers.size();
		const bool scrollUp = ImGui::GetIO().MouseWheel > 0; //does not work, because ImGui stuff is handled during drawing and all physics happens before that (which includes this)
		if (scrollUp) {
			if (insertIndexMax > 0) {
				insertIndex = ((insertIndex % insertIndexMax) + 1 + insertIndexMax) % insertIndexMax; //accounting for potential mistakes/trolls
			} else {
				insertIndex = 0;
			}
		} else {
			if (insertIndexMax > 0) {
				insertIndex = ((insertIndex % insertIndexMax) - 1 + insertIndexMax) % insertIndexMax; //accounting for potential mistakes/trolls
			} else {
				insertIndex = 0;
			}
		}
		std::cout << "DeveloperManager insertIdentifier: " << insertListIdentifiers[insertIndex] << std::endl;
	}
}

std::vector<std::string> DeveloperManager::insertListIdentifiers = { "longinvincible", "temp", "banana", "homing", "barrier", "bounce", "mines", "multishot", "grenade", "blast", "godmode", "big", "inversion", "annoying", "stationary_turret", "vert_wall", "horz_wall" };
int DeveloperManager::insertListIdentifiers_normalSize = insertListIdentifiers.size();
void DeveloperManager::devInsert(int x, int y) {
	GenericFactoryConstructionData constructionData;
	double* posArr;
	switch (insertIndex) {
		case 0:
			#if _DEBUG
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "colorless_longinvincible"));
			#else
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "longinvincible"));
			#endif
			break;
		case 1:
			//PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla", "megadeath"));
			//PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla-extra", "shotgun"));
			//PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "banana_split"));
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla-extra", "swarm"));
			break;
		case 2:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla", "banana"));
			break;
		case 3:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla", "homing"));
			break;
		case 4:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla-extra", "barrier"));
			break;
		case 5:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla", "bounce"));
			break;
		case 6:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla-extra", "mines"));
			break;
		case 7:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla", "multishot"));
			break;
		case 8:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla", "grenade"));
			break;
		case 9:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla", "blast"));
			break;
		case 10:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla", "godmode"));
			break;
		case 11:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "vanilla", "big"));
			break;
		case 12:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "inversion"));
			break;
		case 13:
			PowerupManager::pushPowerup(new PowerSquare(x, y, "dev", "annoying"));
			break;
		case 14:
			posArr = new double[3]{ double(x), double(y), 0 };
			constructionData = GenericFactoryConstructionData(3, posArr);
			HazardManager::pushCircleHazard("vanilla", "stationary_turret", constructionData);
			break;
		case 15:
			WallManager::pushWall(new Wall(x, y, 20, 60, LevelManager::getLevel(0)->getDefaultColor()));
			break;
		case 16:
			WallManager::pushWall(new Wall(x, y, 60, 20, LevelManager::getLevel(0)->getDefaultColor()));
			break;
		default:
			const std::pair<std::string, std::string>& powerIdentifier = insertList_extra[insertIndex - insertListIdentifiers_normalSize];
			PowerupManager::pushPowerup(new PowerSquare(x, y, powerIdentifier.first, powerIdentifier.second));
			break;
	}
}

std::vector<std::pair<std::string, std::string>> DeveloperManager::insertList_extra;
void DeveloperManager::addInsertPower(std::string identifier, std::string type, std::string name) {
	insertListIdentifiers.push_back(identifier);
	insertList_extra.push_back({ type, name });
}
