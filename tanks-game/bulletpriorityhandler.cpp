#pragma once
#include "constants.h"
#include "bulletpriorityhandler.h"
#include "bullet.h"

char BulletPriorityHandler::determinePriority(Bullet* a, Bullet* b) {
	return -1; //for now, all die

	//actually this is definitely not what I want anymore but I'll replace it soon:

	//balance between what I can implement, what I wanted, and code complexity:
	/*
	double a_highestOffenseTier = a->getHighestOffenseTier();
	double b_highestOffenseTier = b->getHighestOffenseTier();
	double a_highestDefenseTier = a->getHighestBulletDefenseValue();
	double b_highestDefenseTier = b->getHighestBulletDefenseValue();
	double a_offenseTierValue = a->getOffenseValue(a_highestOffenseTier);
	double b_offenseTierValue = b->getOffenseValue(b_highestOffenseTier);
	double a_defenseTierValue = a->getDefenseValue(a_highestDefenseTier);
	double b_defenseTierValue = b->getDefenseValue(b_highestDefenseTier);

	bool a_shouldDie = false;
	bool b_shouldDie = false;

	//figure out whether a should die:
	if (a_highestDefenseTier < b_highestOffenseTier) {
		a_shouldDie = true;
	}
	else if (a_highestDefenseTier == b_highestOffenseTier) {
		if (a_defenseTierValue <= b_offenseTierValue) {
			a_shouldDie = true;
		}
	}
	//else shouldn't die

	//figure out whether b should die:
	if (b_highestDefenseTier < a_highestOffenseTier) {
		b_shouldDie = true;
	}
	else if (b_highestDefenseTier == a_highestOffenseTier) {
		if (b_defenseTierValue <= a_offenseTierValue) {
			b_shouldDie = true;
		}
	}
	//else shouldn't die

	//return result:
	if (a_shouldDie && b_shouldDie) {
		return nullptr;
	}
	if (a_shouldDie) {
		return a;
	}
	if (b_shouldDie) {
		return b;
	}
	return junkBullet;
	*/

	//this is the updated version of the code below this code block (unfinished)
	/*
	//purpose: find which has the higher offense and see if it can defeat the other's defense
	//does not matter if one has 1 inv and 1 big vs a 1 big; they both lose (jeez, that simplifies so much!)

	//double highest_tier = HIGHEST_REASONABLE_TIER;
	double a_highestOffenseTier = a->getHighestOffenseTier();
	double b_highestOffenseTier = b->getHighestOffenseTier();
	double a_highestDefenseTier = a->getHighestBulletDefenseValue();
	double b_highestDefenseTier = b->getHighestBulletDefenseValue();
	double a_offenseTierValue = a->getOffenseValue(a_highestOffenseTier);
	double b_offenseTierValue = b->getOffenseValue(b_highestOffenseTier);
	double a_defenseTierValue = a->getDefenseValue(a_highestDefenseTier);
	double b_defenseTierValue = b->getDefenseValue(b_highestDefenseTier);

	bool a_shouldDie = false;
	bool b_shouldDie = false;

	if (a_highestOffenseTier > b_highestOffenseTier) {
		if (a_highestOffenseTier > b_highestDefenseTier) {
			return b;
		}
		if (a_highestOffenseTier == b_highestDefenseTier) {
			return nullptr; //this right? might want to take into account the tier values
		}
		return junkBullet;
	}
	if (b_highestOffenseTier > a_highestOffenseTier) {
		if (b_highestOffenseTier > a_highestDefenseTier) {
			return a;
		}
		if (b_highestOffenseTier == a_highestDefenseTier) {
			return nullptr; //think about
		}
		return junkBullet;
	}

	//offense tiers are equivalent
	if (a_offenseTierValue > b_offenseTierValue) {
		if (a_highestOffenseTier > b_highestDefenseTier) {
			return b;
		}
		if (a_highestOffenseTier < b_highestDefenseTier) {
			return 
		}
		return b;
	}
	if (b_offenseTierValue > a_offenseTierValue) {
		return a;
	}

	if (a_shouldDie && b_shouldDie) {
		return nullptr;
	}
	if (a_shouldDie) {
		return a;
	}
	if (b_shouldDie) {
		return b;
	}
	return junkBullet;
	*/

	//could be worked on, but it's good enough
	/*
	double highest_tier = HIGHEST_REASONABLE_TIER;
	double value_a = a->getTierValue(highest_tier);
	double value_b = b->getTierValue(highest_tier);

	if (value_a > value_b) {
		return b;
	}
	if (value_a < value_b) {
		return a;
	}

	while (highest_tier > 0) {
		double highest_a = BulletPriorityHandler::getNextHighestTier(a, highest_tier);
		double highest_b = BulletPriorityHandler::getNextHighestTier(b, highest_tier);

		if (highest_a > highest_b) {
			return b;
		}
		if (highest_a < highest_b) { 
			return b;
		}

		value_a = a->getTierValue(highest_a);
		value_b = b->getTierValue(highest_b);

		if (value_a > value_b) {
			return b;
		}
		if (value_a < value_b) {
			return a;
		}

		highest_tier = highest_a;
	}
	return nullptr;
	*/
	
	
	//old:

	//TODO: tier-based system: 1=shield, 2=invincible, 3=big, 4=super/mega-death/other (4 overpowers invincibility on tank, meaning other powers will have state what they protect up to)
	//IMPORTANT: each power has protection tier and offense tier
	//bullet needs to be able to give other bullet some state changes
	/*
	//because it's which SHOULD win, not also deal with result, this code won't be used
	if (a->getBigValue() == 0 && b->getBigValue() == 0) {
		if (a->getInvValue() == 0 && b->getInvValue() == 0) {
			if (a->getShieldValue() == 0 && b->getShieldValue() == 0) {
				//a->loseShield();
				//b->loseShield();
			}
			else if (a->getShieldValue() == 0) {
				//a->loseShield();
			}
			else if (b->getShieldValue() == 0) {
				//b->loseShield();
			}
			else {
				//a->loseShield();
				//b->loseShield();
			}
		}
		else if (a->getInvValue() == 0) {
			//a->loseInv();
		}
		else if (b->getInvValue() == 0) {
			//b->loseInv();
		}
		else {
			//a->loseInv();
			//b->loseInv();
		}
	}
	else if (a->getBigValue() == 0) {
		//a->loseBig();
	}
	else if (b->getBigValue() == 0) {
		//b->loseBig();
	}
	else {
		//a->loseBig();
		//b->loseBig();
	}
	//technically just supposed to return which should win, right?
	//should have while loops
	//probably just do everything in one tick: 3shield vs 2shield would result in 1shield and dead, 1shield vs 1inv with 1shield would result in dead and 1inv with 1shield
	//if that's not done, bullets need to be able to stop in place
	*/
}

//short BulletPriorityHandler::getNextHighestTier(Bullet* z, short current_tier) {
	//return 0;
	
	/*
	short highest_tier = 0;
	for (int i = 0; i < z->currentPowers.size(); i++) {
		short t = z->currentPowers[i]->getOffenseTier();
		if (t > highest_tier && t < current_tier) {
			highets_tier = t;
		}
	}
	return highest_tier;
	*/
//}