#pragma once

class GameThing {
	friend class GameManager;
protected:
	long gameID;
	char teamID;
public:
	long getGameID() { return gameID; }
	char getTeamID() { return teamID; }
};
