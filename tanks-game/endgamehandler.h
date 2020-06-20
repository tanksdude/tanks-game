#pragma once
#include "inheritedpowercommon.h" //maybe the struct should be somewhere else, or called something else
#include "tank.h"
#include "bullet.h"
#include "circlehazard.h"
#include "recthazard.h"

class EndGameHandler {
	friend class DeveloperManager;
	friend class ResetThings;
private:
	static void finalizeScores(); //ResetThings calls this
public:
	static PowerInteractionBoolHolder determineWinner(Tank*, Bullet*); //note: modifies the tank's value of "dead"
	static PowerInteractionBoolHolder determineWinner(Tank*, Tank*); //note: modifies the tanks' values of "dead"
	static PowerInteractionBoolHolder determineWinner(Bullet*, Bullet*);
	static PowerInteractionBoolHolder determineWinner(Tank*, CircleHazard*); //note: modifies the tank's value of "dead"
	static PowerInteractionBoolHolder determineWinner(Tank*, RectHazard*); //note: modifies the tank's value of "dead"
	static PowerInteractionBoolHolder determineWinner(Bullet*, CircleHazard*);
	static PowerInteractionBoolHolder determineWinner(Bullet*, RectHazard*);
};