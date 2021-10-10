#pragma once
#include "interactionboolholder.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "constants.h"
#include "tank.h"
#include "bullet.h"
#include "circlehazard.h"
#include "recthazard.h"

class EndGameHandler { //also score manager (TODO: maybe move to GameManager)
	friend class DeveloperManager;
	friend class ResetThings;
private:
	struct DoubleChar {
		char killer;
		char killee;
		DoubleChar(char a, char b) { killer = a; killee = b; }
		DoubleChar() { killer = DEFAULT_TEAM; killee = DEFAULT_TEAM; }
	};
	struct CharAndString {
		char teamID;
		std::string teamName;
		CharAndString(char id, std::string name) { teamID = id; teamName = name; }
		CharAndString() { teamID = DEFAULT_TEAM; teamName = "DEFAULT_TEAM"; }
	};
	static std::unordered_map<char, int> teamWins;
	static std::vector<EndGameHandler::CharAndString> teamsParticipating;
	static std::vector<EndGameHandler::DoubleChar> killEvents;

	static void finalizeScores(); //ResetThings calls this
public:
	static InteractionBoolHolder determineWinner(Tank*, Bullet*); //note: can kill tank
	static InteractionBoolHolder determineWinner(Tank*, Tank*); //note: can kill tanks
	static InteractionBoolHolder determineWinner(Bullet*, Bullet*);
	static InteractionBoolHolder determineWinner(Tank*, CircleHazard*); //note: can kill tank
	static InteractionBoolHolder determineWinner(Tank*, RectHazard*); //note: can kill tank
	static InteractionBoolHolder determineWinner(Bullet*, CircleHazard*);
	static InteractionBoolHolder determineWinner(Bullet*, RectHazard*);

	static bool shouldGameEnd();

	static void addTeamToWatch(char teamID, std::string teamName);
	static void clearWatchingTeams();

private:
	EndGameHandler() {}
	EndGameHandler(const EndGameHandler&) {}
};
