#include "game-thing.h"

#include "game-manager.h"
#include "circle.h"
#include "rect.h"

#include "tank.h"
#include "bullet.h"
#include "wall.h"
#include "power-square.h"
#include "circle-hazard.h"
#include "rect-hazard.h"

GameThing::GameThing(Team_ID t_id, ObjectType type) {
	this->gameID = GameManager::getNextID();
	this->teamID = t_id;
	this->objectType = type;
	//does not initialize xStart/xEnd
}

void GameThing::updateAABB() noexcept {
	Circle* self_c;
	Rect* self_r;

	switch (objectType) {
		default: [[fallthrough]];
		case ObjectType::None:
			xStart = xEnd = 0;
			break;

		case ObjectType::Tank:
			self_c = static_cast<Circle*>(static_cast<Tank*>(this));
			xStart = self_c->getX() - self_c->getR();
			xEnd   = self_c->getX() + self_c->getR();
			break;

		case ObjectType::Bullet:
			self_c = static_cast<Circle*>(static_cast<Bullet*>(this));
			xStart = self_c->getX() - self_c->getR();
			xEnd   = self_c->getX() + self_c->getR();
			break;

		case ObjectType::Wall:
			self_r = static_cast<Rect*>(static_cast<Wall*>(this));
			xStart = self_r->getX();
			xEnd   = self_r->getX() + self_r->getW();
			break;

		case ObjectType::Powerup:
			self_r = static_cast<Rect*>(static_cast<PowerSquare*>(this));
			xStart = self_r->getX();
			xEnd   = self_r->getX() + self_r->getW();
			break;

		case ObjectType::Hazard_C:
			self_c = static_cast<Circle*>(static_cast<CircleHazard*>(this));
			xStart = self_c->getX() - self_c->getR();
			xEnd   = self_c->getX() + self_c->getR();
			break;

		case ObjectType::Hazard_R:
			self_r = static_cast<Rect*>(static_cast<RectHazard*>(this));
			xStart = self_r->getX();
			xEnd   = self_r->getX() + self_r->getW();
			break;
	}
}
