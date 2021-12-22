#pragma once
#include "interaction-bool-holder.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "constants.h"
#include "tank.h"
#include "bullet.h"
#include "circle-hazard.h"
#include "rect-hazard.h"

class EndGameHandler { //also score manager (TODO: maybe move to GameManager)
	friend class DeveloperManager;
	friend class ResetThings;

private:
	struct DoubleTeam_ID {
		//could use std::pair instead
		Team_ID killer;
		Team_ID killee;
		DoubleTeam_ID(Team_ID a, Team_ID b) { killer = a; killee = b; }
		DoubleTeam_ID() { killer = DEFAULT_TEAM; killee = DEFAULT_TEAM; }
	};
	struct Team_IDAndString {
		Team_ID teamID;
		std::string teamName;
		Team_IDAndString(Team_ID id, std::string name) { teamID = id; teamName = name; }
		Team_IDAndString() { teamID = DEFAULT_TEAM; teamName = "DEFAULT_TEAM"; }
	};

	static std::unordered_map<Team_ID, int> teamWins;
	static std::vector<EndGameHandler::Team_IDAndString> teamsParticipating;
	static std::vector<EndGameHandler::DoubleTeam_ID> killEvents;

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

	static void addTeamToWatch(Team_ID teamID, std::string teamName);
	static void clearWatchingTeams();

private:
	EndGameHandler() {}
	EndGameHandler(const EndGameHandler&) {}
};
