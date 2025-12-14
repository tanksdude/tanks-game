#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "constants.h"

#include "interaction-bool-holder.h"
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

public: //note: these kill tanks and bullets; const pointers only to prevent position changes, but killing isn't const, so const_cast is used
	static InteractionBoolHolder                                                 determineWinner(const Tank*,   const Bullet*,       bool attemptTankDeath,    bool attemptBulletDeath);
	static InteractionUpdateHolder<TankUpdateStruct, TankUpdateStruct>           determineWinner(const Tank*,   const Tank*,         bool attemptTank1Death,   bool attemptTank2Death);
	static InteractionBoolHolder                                                 determineWinner(const Bullet*, const Bullet*,       bool attemptBullet1Death, bool attemptBullet2Death);
	static InteractionUpdateHolder<TankUpdateStruct, CircleHazardUpdateStruct>   determineWinner(const Tank*,   const CircleHazard*, bool attemptTankDeath,    bool attemptCircleHazardDeath);
	static InteractionUpdateHolder<TankUpdateStruct, RectHazardUpdateStruct>     determineWinner(const Tank*,   const RectHazard*,   bool attemptTankDeath,    bool attemptRectHazardDeath);
	static InteractionUpdateHolder<BulletUpdateStruct, CircleHazardUpdateStruct> determineWinner(const Bullet*, const CircleHazard*, bool attemptBulletDeath,  bool attemptCircleHazardDeath);
	static InteractionUpdateHolder<BulletUpdateStruct, RectHazardUpdateStruct>   determineWinner(const Bullet*, const RectHazard*,   bool attemptBulletDeath,  bool attemptRectHazardDeath);
	static InteractionUpdateHolder<TankUpdateStruct, WallUpdateStruct>           determineWinner(const Tank*,   const Wall*,         bool attemptTankDeath,    bool attemptWallDeath); //temporary?

	//just for GameMainLoop and EndGameHandler
	static void killTank(Tank*); //suicide (does not get an extra life)
	static bool killTank(Tank*, const GameThing* killer); //returns whether the tank is dead
	static void killBullet(Bullet*);
	static bool killBullet(Bullet*, const GameThing* killer);

	static bool shouldGameEnd();

	static void addTeamToWatch(Team_ID teamID, std::string teamName);
	static void clearWatchingTeams();

private:
	EndGameHandler() = delete;
	EndGameHandler(const EndGameHandler&) = delete;
};
