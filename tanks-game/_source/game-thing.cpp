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
}

void GameThing::updateAABB() noexcept {
	Circle* self_c;
	Rect* self_r;

	switch (objectType) {
		default: [[fallthrough]];
		case ObjectType::None:
			xStart = 0; xEnd = 0;
			break;

		case ObjectType::Tank:
			{
				Tank* t = static_cast<Tank*>(this);
				self_c = static_cast<Circle*>(t);
			}
			xStart = self_c->getX() - self_c->getR();
			xEnd   = self_c->getX() + self_c->getR();
			break;

		case ObjectType::Bullet:
			{
				Bullet* b = static_cast<Bullet*>(this);
				self_c = static_cast<Circle*>(b);
			}
			xStart = self_c->getX() - self_c->getR();
			xEnd   = self_c->getX() + self_c->getR();
			break;

		case ObjectType::Wall:
			{
				Wall* w = static_cast<Wall*>(this);
				self_r = static_cast<Rect*>(w);
			}
			xStart = self_r->getX();
			xEnd   = self_r->getX() + self_r->getW();
			break;

		case ObjectType::Powerup:
			{
				PowerSquare* p = static_cast<PowerSquare*>(this);
				self_r = static_cast<Rect*>(p);
			}
			xStart = self_r->getX();
			xEnd   = self_r->getX() + self_r->getW();
			break;

		case ObjectType::Hazard_C:
			{
				CircleHazard* ch = static_cast<CircleHazard*>(this);
				self_c = static_cast<Circle*>(ch);
			}
			xStart = self_c->getX() - self_c->getR();
			xEnd   = self_c->getX() + self_c->getR();
			break;

		case ObjectType::Hazard_R:
			{
				RectHazard* rh = static_cast<RectHazard*>(this);
				self_r = static_cast<Rect*>(rh);
			}
			xStart = self_r->getX();
			xEnd   = self_r->getX() + self_r->getW();
			break;
	}
}
