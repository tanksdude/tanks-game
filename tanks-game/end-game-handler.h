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

public: //note: these kill tanks and bullets
	static InteractionBoolHolder determineWinner(Tank*, Bullet*);
	static InteractionBoolHolder determineWinner(Tank*, Tank*);
	static InteractionBoolHolder determineWinner(Bullet*, Bullet*);
	static InteractionBoolHolder determineWinner(Tank*, CircleHazard*);
	static InteractionBoolHolder determineWinner(Tank*, RectHazard*);
	static InteractionBoolHolder determineWinner(Bullet*, CircleHazard*);
	static InteractionBoolHolder determineWinner(Bullet*, RectHazard*);
	static InteractionBoolHolder determineWinner(Tank*, Wall*, bool tankDies); //temporary
	static InteractionBoolHolder determineWinner(Bullet*, Wall*, bool bulletDies); //temporary
	//TODO: rewrite these functions to be more of a "handleCollisionEvent" thing

	//just for GameMainLoop and EndGameHandler
	static void killTank(Tank*); //suicide (does not get an extra life)
	static bool killTank(Tank*, GameThing* killer); //returns whether the tank is dead
	static void killBullet(Bullet*);
	static bool killBullet(Bullet*, GameThing* killer);

	static bool shouldGameEnd();

	static void addTeamToWatch(Team_ID teamID, std::string teamName);
	static void clearWatchingTeams();

private:
	EndGameHandler() {}
	EndGameHandler(const EndGameHandler&) {}
};
