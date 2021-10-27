#pragma once
#include "constants.h"

class GameThing {
	friend class GameManager;
protected:
	Game_ID gameID;
	Team_ID teamID;
public:
	Game_ID getGameID() const { return gameID; }
	Team_ID getTeamID() const { return teamID; }
};
