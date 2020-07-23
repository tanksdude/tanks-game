#pragma once

class GameThing {
	friend class GameManager;
protected:
	long gameID;
	char teamID;
public:
	long getGameID() const { return gameID; }
	char getTeamID() const { return teamID; }
};
