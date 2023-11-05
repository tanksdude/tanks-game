#pragma once
#include "constants.h"

class GameThing {
	friend class GameManager;
	friend struct GameThingComparator;

private:
	Game_ID gameID;
	Team_ID teamID;

public:
	Game_ID getGameID() const { return this->gameID; }
	Team_ID getTeamID() const { return this->teamID; }

protected:
	void changeTeamID(Team_ID t_id) { this->teamID = t_id; }
	GameThing(Team_ID t_id);

private:
	GameThing() = delete;
	GameThing(const GameThing&) = delete;
};

struct GameThingComparator {
	//allows something like an std::map to sort
	constexpr bool operator()(const GameThing& lhs, const GameThing& rhs) const noexcept {
		return lhs.gameID < rhs.gameID;
	}
};
