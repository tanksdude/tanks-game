#include "game-thing.h"
#include "game-manager.h"

GameThing::GameThing(Team_ID t_id) {
	this->gameID = GameManager::getNextID();
	this->teamID = t_id;
}
