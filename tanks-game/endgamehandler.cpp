#include "endgamehandler.h"
#include "priorityhandler.h"
#include "collisionhandler.h"
#include "tankmanager.h"
#include "constants.h"
#include <iostream>

//TODO?: CircleHazard and RectHazard use CollisionHandler for their modified tank/bullet collision by default

std::unordered_map<char, int> EndGameHandler::teamWins;
std::vector<EndGameHandler::CharAndString> EndGameHandler::teamsParticipating;
std::vector<EndGameHandler::DoubleChar> EndGameHandler::killEvents;

void EndGameHandler::addTeamToWatch(char teamID, std::string teamName) {
	if (teamID == DEFAULT_TEAM) {
		std::cerr << "WARNING: DEFAULT_TEAM is not allowed to have a score" << std::endl;
	} else {
		teamsParticipating.push_back({ teamID, teamName });
		teamWins.insert({ teamID, 0 });
	}
}

void EndGameHandler::clearWatchingTeams() {
	for (int i = 0; i < teamsParticipating.size(); i++) {
		teamWins.erase(teamsParticipating[i].teamID);
	}
	teamsParticipating.clear();
	killEvents.clear(); //just in case
}

void EndGameHandler::finalizeScores() {
	//TODO: GameManager holds information about objects and their IDs (and what teamIDs are being used?)
	//TODO: is this done? (will definitely update when multi-tank mode becomes a thing)

	//process kill events
	std::vector<EndGameHandler::DoubleChar> pastEvents; //in case tank dies from multiple bullets at the same time
	for (int i = 0; i < killEvents.size(); i++) {
		bool previouslyHappened = false;
		for (int j = 0; j < pastEvents.size(); j++) {
			if ((killEvents[i].killer == pastEvents[j].killer) && (killEvents[i].killee == pastEvents[j].killee)) {
				previouslyHappened = true;
				break;
			}
		}
		if (!previouslyHappened) {
			if (killEvents[i].killer != killEvents[i].killee) { //friendly fire score increase prevention
				teamWins[killEvents[i].killer]++;
				pastEvents.push_back(killEvents[i]);
			}
			/*
			//... maybe
			else {
				teamWins[killEvents[i].killer]--;
			}
			*/
		}
	}
	killEvents.clear();

	//TODO: if two tanks kill each other at the same time, then they'll both get a point; might change it so that can't happen, though I think the logic will be complicated when num of tanks > 2
	//other note: if hazard and tank kill other tank at same time, they both get a point; this is the intended behavior
	//quick note: there can be many hazards in the level, so the hazard team can get multiple points in one round (TODO: the logic can't deal with team mode, so fix that)
}

InteractionBoolHolder EndGameHandler::determineWinner(Tank* t, Bullet* b) {
	char result = PriorityHandler::determinePriority(t, b);
	bool tankDies = false, bulletDies = false;
	if (result <= -2) {
		bool firstDies = rand()%2;
		if (firstDies) {
			tankDies = true;
		} else {
			bulletDies = true;
		}
	} else if (result == -1) { //both die
		tankDies = true;
		bulletDies = true;
	} else if (result >= 2) { //it's a draw, so neither dies (probably not going to happen)
		//nothing
	} else {
		if (result == 0) {
			tankDies = true;
		} else {
			bulletDies = true;
		}
	}
	if (tankDies) {
		tankDies = t->kill();
		if (tankDies) {
			killEvents.push_back({ b->getTeamID(), t->getTeamID() });
		}
	}
	return { tankDies, bulletDies };
}

InteractionBoolHolder EndGameHandler::determineWinner(Tank* t1, Tank* t2) {
	char result = PriorityHandler::determinePriority(t1, t2);
	bool firstTankDies = false, secondTankDies = false;
	if (result <= -2) {
		bool firstDies = rand()%2;
		if (firstDies) {
			firstTankDies = true;
		} else {
			secondTankDies = true;
		}
	} else if (result == -1) { //both die
		firstTankDies = true;
		secondTankDies = true;
	} else if (result >= 2) { //it's a draw, so neither dies (normal result)
		CollisionHandler::pushMovableAwayFromMovable(t1, t2);
	} else {
		if (result == 0) {
			firstTankDies = true;
		} else {
			secondTankDies = true;
		}
	}
	if (firstTankDies) {
		firstTankDies = t1->kill();
		if (firstTankDies) {
			killEvents.push_back({ t2->getTeamID(), t1->getTeamID() });
		}
	}
	if (secondTankDies) {
		secondTankDies = t2->kill();
		if (secondTankDies) {
			killEvents.push_back({ t1->getTeamID(), t2->getTeamID() });
		}
	}
	return { firstTankDies, secondTankDies };
}

InteractionBoolHolder EndGameHandler::determineWinner(Bullet* b1, Bullet* b2) {
	char result = PriorityHandler::determinePriority(b1, b2);
	bool firstBulletDies = false, secondBulletDies = false;
	if (result <= -2) {
		bool firstDies = rand()%2;
		if (firstDies) {
			firstBulletDies = true;
		} else {
			secondBulletDies = true;
		}
	} else if (result == -1) { //both die
		firstBulletDies = true;
		secondBulletDies = true;
	} else if (result >= 2) { //it's a draw, so neither dies
		//nothing
	} else {
		if (result == 0) {
			firstBulletDies = true;
		} else {
			secondBulletDies = true;
		}
	}
	return { firstBulletDies, secondBulletDies };
}

InteractionBoolHolder EndGameHandler::determineWinner(Tank* t, CircleHazard* ch) {
	char result = PriorityHandler::determinePriority(t, ch);
	bool tankDies = false, circleHazardDies = false;
	if (result <= -2) {
		bool firstDies = rand()%2;
		if (firstDies) {
			if (ch->hasSpecialEffectTankCollision) {
				ch->specialEffectTankCollision(t);
			}
			tankDies = true;
		} else {
			circleHazardDies = true;
		}
	} else if (result == -1) { //both die
		tankDies = true;
		circleHazardDies = true;
	} else if (result >= 2) { //it's a draw, so neither dies
		if (ch->modifiesTankCollision) {
			if (ch->hasSpecialEffectTankCollision) {
				ch->specialEffectTankCollision(t);
			}
			ch->modifiedTankCollision(t);
		} else {
			if (ch->hasSpecialEffectTankCollision) {
				ch->specialEffectTankCollision(t);
			}
			CollisionHandler::pushMovableAwayFromImmovable(t, ch);
		}
	} else {
		if (result == 0) {
			if (ch->hasSpecialEffectTankCollision) {
				ch->specialEffectTankCollision(t);
			}
			tankDies = true;
		} else {
			circleHazardDies = true;
		}
	}
	if (tankDies) {
		tankDies = t->kill();
		if (tankDies) {
			killEvents.push_back({ ch->getTeamID(), t->getTeamID() });
		}
	}
	return { tankDies, circleHazardDies };
}

InteractionBoolHolder EndGameHandler::determineWinner(Tank* t, RectHazard* rh) {
	char result = PriorityHandler::determinePriority(t, rh);
	bool tankDies = false, rectHazardDies = false;
	if (result <= -2) {
		bool firstDies = rand()%2;
		if (firstDies) {
			if (rh->hasSpecialEffectTankCollision) {
				rh->specialEffectTankCollision(t);
			}
			tankDies = true;
		} else {
			rectHazardDies = true;
		}
	} else if (result == -1) { //both die
		tankDies = true;
		rectHazardDies = true;
	} else if (result >= 2) { //it's a draw, so neither dies
		if (rh->modifiesTankCollision) {
			if (rh->hasSpecialEffectTankCollision) {
				rh->specialEffectTankCollision(t);
			}
			rh->modifiedTankCollision(t);
		} else {
			if (rh->hasSpecialEffectTankCollision) {
				rh->specialEffectTankCollision(t);
			}
			CollisionHandler::pushMovableAwayFromImmovable(t, rh);
		}
	} else {
		if (result == 0) {
			if (rh->hasSpecialEffectTankCollision) {
				rh->specialEffectTankCollision(t);
			}
			tankDies = true;
		} else {
			rectHazardDies = true;
		}
	}
	if (tankDies) {
		tankDies = t->kill();
		if (tankDies) {
			killEvents.push_back({ rh->getTeamID(), t->getTeamID() });
		}
	}
	return { tankDies, rectHazardDies };
}

InteractionBoolHolder EndGameHandler::determineWinner(Bullet* b, CircleHazard* ch) {
	char result = PriorityHandler::determinePriority(b, ch);
	bool bulletDies = false, circleHazardDies = false;
	if (result <= -2) {
		bool firstDies = rand()%2;
		if (firstDies) {
			if (ch->hasSpecialEffectBulletCollision) {
				ch->specialEffectBulletCollision(b);
			}
			bulletDies = true;
		} else {
			circleHazardDies = true;
		}
	} else if (result == -1) { //both die
		bulletDies = true;
		circleHazardDies = true;
	} else if (result >= 2) { //it's a draw, so neither dies
		if (ch->modifiesBulletCollision) {
			if (ch->hasSpecialEffectBulletCollision) {
				ch->specialEffectBulletCollision(b);
			}
			ch->modifiedBulletCollision(b);
		} else {
			if (ch->hasSpecialEffectBulletCollision) {
				ch->specialEffectBulletCollision(b);
			}
			CollisionHandler::pushMovableAwayFromImmovable(b, ch);
		}
	} else {
		if (result == 0) {
			if (ch->hasSpecialEffectBulletCollision) {
				ch->specialEffectBulletCollision(b);
			}
			bulletDies = true;
		} else {
			circleHazardDies = true;
		}
	}
	return { bulletDies, circleHazardDies };
}

InteractionBoolHolder EndGameHandler::determineWinner(Bullet* b, RectHazard* rh) {
	char result = PriorityHandler::determinePriority(b, rh);
	bool bulletDies = false, rectHazardDies = false;
	if (result <= -2) {
		bool firstDies = rand()%2;
		if (firstDies) {
			if (rh->hasSpecialEffectBulletCollision) {
				rh->specialEffectBulletCollision(b);
			}
			bulletDies = true;
		} else {
			rectHazardDies = true;
		}
	} else if (result == -1) { //both die
		bulletDies = true;
		rectHazardDies = true;
	} else if (result >= 2) { //it's a draw, so neither dies
		if (rh->modifiesBulletCollision) {
			if (rh->hasSpecialEffectBulletCollision) {
				rh->specialEffectBulletCollision(b);
			}
			rh->modifiedBulletCollision(b);
		} else {
			if (rh->hasSpecialEffectBulletCollision) {
				rh->specialEffectBulletCollision(b);
			}
			CollisionHandler::pushMovableAwayFromImmovable(b, rh);
		}
	} else {
		if (result == 0) {
			if (rh->hasSpecialEffectBulletCollision) {
				rh->specialEffectBulletCollision(b);
			}
			bulletDies = true;
		} else {
			rectHazardDies = true;
		}
	}
	return { bulletDies, rectHazardDies };
}
