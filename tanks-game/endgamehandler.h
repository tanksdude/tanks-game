#pragma once
#include "interactionboolholder.h"
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
	static InteractionBoolHolder determineWinner(Tank*, Bullet*); //note: modifies the tank's value of "dead"
	static InteractionBoolHolder determineWinner(Tank*, Tank*); //note: modifies the tanks' values of "dead"
	static InteractionBoolHolder determineWinner(Bullet*, Bullet*);
	static InteractionBoolHolder determineWinner(Tank*, CircleHazard*); //note: modifies the tank's value of "dead"
	static InteractionBoolHolder determineWinner(Tank*, RectHazard*); //note: modifies the tank's value of "dead"
	static InteractionBoolHolder determineWinner(Bullet*, CircleHazard*);
	static InteractionBoolHolder determineWinner(Bullet*, RectHazard*);
};
