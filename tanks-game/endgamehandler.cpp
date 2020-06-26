#include "endgamehandler.h"
#include "priorityhandler.h"
#include "collisionhandler.h"
#include "tankmanager.h"

//TODO?: CircleHazard and RectHazard use CollisionHandler for their modified tank/bullet collision by default

void EndGameHandler::finalizeScores() {
	//TODO: GameManager holds information about objects and their IDs (and what teamIDs are being used)
	//TODO: this logic isn't done
	//notes: in the determineWinner functions that involve a tank: add killer (and killee) to list; when finalizing scores, add wins as needed (to GameManager, because that's supposed to hold the team wins)

	bool* deadStatus = new bool[TankManager::getNumTanks()];
	bool everyTankIsDead = true;
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		deadStatus[i] = (TankManager::getTank(i)->dead);
		if (!deadStatus[i]) {
			everyTankIsDead = false;
		}
	}

	if (!everyTankIsDead) {
		for (int i = 0; i < TankManager::getNumTanks(); i++) {
			if (!deadStatus[i]) {
				TankManager::getTank(i)->wins++;
			}
		}
	}
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
	}
	if (secondTankDies) {
		secondTankDies = t2->kill();
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
