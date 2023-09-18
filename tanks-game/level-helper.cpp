#include "level-helper.h"

#include "rng.h"
#include "mylib.h" //isInArray, weightedSelect

#include "powerup-data-governor.h"
#include "wall-manager.h" //only used by pushClassicWalls()

Wall* LevelHelper::makeNewRandomWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, const ColorValueHolder& c, double minW, double minH, double maxW, double maxH) {
	double w = RNG::randFunc() * (maxW - minW) + minW;
	double h = RNG::randFunc() * (maxH - minH) + minH;

	return new Wall(x_beginning + RNG::randFunc() * (width_ofArea - w), y_beginning + RNG::randFunc() * (height_ofArea - h), w, h, c);
}

void LevelHelper::pushClassicWalls(const ColorValueHolder& c) {
	for (int i = 0; i < 4; i++) {
		//classic JS walls
		PositionHolder pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, c));
	}

	//compiler warning C4506: can't inline a function if it's not defined right away, unless extern is used
	//that's why this function wasn't declared as inline
}

PositionHolder* LevelHelper::getClassicWallPositions() {
	PositionHolder* wallArray = new PositionHolder[4];
	for (int i = 0; i < 4; i++) {
		//classic JS walls
		wallArray[i] = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
	}
	return wallArray;
}

std::string* LevelHelper::getRandomPowers(int count, std::string type) {
	//get list of powers
	int powerNum = PowerupDataGovernor::getNumPowerTypes(type);
	std::string* possiblePowers = new std::string[powerNum];
	for (int i = 0; i < powerNum; i++) {
		possiblePowers[i] = PowerupDataGovernor::getPowerName(type, i);
	}

	//get stacking status and weight of the powers
	bool* canStack = new bool[powerNum];
	float* powerWeights = new float[powerNum];
	for (int i = 0; i < powerNum; i++) {
		Power* p = PowerupDataGovernor::getPowerFactory(type, possiblePowers[i])();
		std::vector<std::string> attributes = p->getPowerAttributes();
		canStack[i] = (std::find(attributes.begin(), attributes.end(), "stack") != attributes.end());
		powerWeights[i] = p->getWeights()[type];
		delete p;
	}

	//get the randomized powers
	std::string* n = LevelHelper::getRandomPowers(count, canStack, possiblePowers, powerWeights, powerNum);

	delete[] possiblePowers;
	delete[] canStack;
	delete[] powerWeights;

	return n;
}

//this can be condensed with the previous function but whatever
std::string* LevelHelper::getRandomPowers(int count, std::string type, std::string* names, int nameCount) {
	//get stacking status and weight of the powers
	bool* canStack = new bool[nameCount];
	float* powerWeights = new float[nameCount];
	for (int i = 0; i < nameCount; i++) {
		Power* p = PowerupDataGovernor::getPowerFactory(type, names[i])();
		std::vector<std::string> attributes = p->getPowerAttributes();
		canStack[i] = (std::find(attributes.begin(), attributes.end(), "stack") != attributes.end());
		powerWeights[i] = p->getWeights()[type];
		delete p;
	}

	//get the randomized powers
	std::string* n = LevelHelper::getRandomPowers(count, canStack, names, powerWeights, nameCount);

	delete[] canStack;
	delete[] powerWeights;

	return n;
}

std::string* LevelHelper::getRandomPowersOld(int count, bool replacement, std::string* names, int nameCount) {
	std::string* n = new std::string[count];
	int* used = new int[count];

	for (int i = 0; i < count; i++) {
		if (!replacement) {
			//first verify if a new name can be chosen:
			bool found = true;
			for (int j = 0; j < i; j++) {
				if (!isInArray<int>(j, used, i+1)) {
					found = false;
					break;
				}
			}
			//then add name based on result
			if (!found) {
				//there are values that can get chosen
				int index;
				do {
					index = RNG::randFunc() * nameCount;
				} while (isInArray<int>(index, used, i+1));
				used[i] = index;
			} else {
				//there aren't values that can get chosen
				replacement = true; //instead of getting stuck or throwing an error, just put duplicates in (change eventually)
			}
		}
		if (replacement) {
			int index = RNG::randFunc() * nameCount;
			used[i] = index;
		}
	}

	for (int i = 0; i < count; i++) {
		n[i] = names[used[i]];
	}

	delete[] used;
	return n;
}

std::string* LevelHelper::getRandomPowers(int count, bool* powersCanStack, std::string* names, int nameCount) {
	std::string* n = new std::string[count];
	int* used = new int[count];

	for (int i = 0; i < count; i++) {
		//first verify if a new name can be chosen:
		bool leftoverPowersCanStack = true;
		for (int j = 0; j < i; j++) {
			if (!powersCanStack[used[j]]) {
				leftoverPowersCanStack = false;
				break;
			}
		}
		//then add name based on result
		int index;
		if (!leftoverPowersCanStack) {
			//there are values that can get chosen
			do {
				index = RNG::randFunc() * nameCount;
			} while (!powersCanStack[index] && isInArray<int>(index, used, i+1));
		} else {
			//there aren't values that can get chosen or no stacking problems yet
			index = RNG::randFunc() * nameCount;
		}
		//not sure if the comments are right, but the results are (empirically determined)
		used[i] = index;
	}

	for (int i = 0; i < count; i++) {
		n[i] = names[used[i]];
	}

	delete[] used;
	return n;
}

//this function could become an endless loop if the only stacking power left has a weight of 0
std::string* LevelHelper::getRandomPowers(int count, bool* powersCanStack, std::string* names, float* weights, int powerCount) {
	std::string* n = new std::string[count];
	int* used = new int[count];

	for (int i = 0; i < count; i++) {
		//first verify if a new name can be chosen:
		bool leftoverPowersCanStack = true;
		for (int j = 0; j < i; j++) {
			if (!powersCanStack[used[j]]) {
				leftoverPowersCanStack = false;
				break;
			}
		}
		//then add name based on result
		int index;
		if (!leftoverPowersCanStack) {
			//there are values that can get chosen
			do {
				index = weightedSelect<float>(weights, powerCount);
			} while (!powersCanStack[index] && isInArray<int>(index, used, i+1));
		} else {
			//there aren't values that can get chosen or no stacking problems yet
			index = weightedSelect<float>(weights, powerCount);
		}
		//not sure if the comments are right, but the results are (empirically determined)
		used[i] = index;
	}

	for (int i = 0; i < count; i++) {
		n[i] = names[used[i]];
	}

	delete[] used;
	return n;
}

std::string LevelHelper::powerAlternate(int position, int rand, std::string p1, std::string p2) {
	return (((int)ceil( float(position%3) / 3) + rand) % 2 == 0 ? p1 : p2);
	/*
	{0,   1,   2,  3} // start
	{0,   1,   2,  0} // %3, then cast to float
	{0,  .3,  .6,  0} // /3, then add rand
	{0,  .3,  .6,  0} (rand=0) -> ceil -> {0, 1, 1, 0}
	{1, 1.3, 1.6,  1} (rand=1) -> ceil -> {1, 2, 2, 1}
	{0, 1, 1, 0} OR	{1, 0, 0, 1} // %2
	*/
}

std::string LevelHelper::simplePowerAlternate(int position, int rand, std::string p1, std::string p2) {
	return ((position + rand) % 2 == 0 ? p1 : p2);
}

PositionHolder LevelHelper::getSymmetricPowerupPositions_LR(int position, double x_center, double y_center, double x_offset) {
	return { x_center + ((position%2)*2-1) * x_offset, y_center };
}

PositionHolder LevelHelper::getSymmetricPowerupPositions_UD(int position, double x_center, double y_center, double y_offset) {
	return { x_center, y_center + ((position%2)*2-1) * y_offset };
}

PositionHolder LevelHelper::getSymmetricPowerupPositions_Corners(int position, double x_center, double y_center, double x_offset, double y_offset) {
	//return { x_center - (position%2*2-1) * x_offset, y_center - ((position/2)*2-1) * y_offset };
	return { x_center + (position%2*2-1) * x_offset, y_center + ((position/2)*2-1) * y_offset };
	//the above returns give the same results but in opposite orders; the order doesn't matter, so I'll arbitrarily choose the second
	//LD, RD, LU, RU
	/*
	switch (position) {
		case 0: return { x_center - x_offset, y_center - y_offset };
		case 1: return { x_center + x_offset, y_center - y_offset };
		case 2: return { x_center - x_offset, y_center + y_offset };
		case 3: return { x_center + x_offset, y_center + y_offset };
		default: return {0,0};
	}
	*/
}

PositionHolder LevelHelper::getSymmetricPowerupPositions_DiagForwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset) {
	return { x_center + ((position%2)*2-1) * x_offset, y_center + ((position%2)*2-1) * y_offset };
	//note: has not been tested
}

PositionHolder LevelHelper::getSymmetricPowerupPositions_DiagBackwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset) {
	return { x_center + ((position%2)*2-1) * x_offset, y_center - ((position%2)*2-1) * y_offset };
	//note: has not been tested
}

PositionHolder LevelHelper::getSymmetricWallPositions_LR(int position, double x_center, double y_center, double x_offset, double wallWidth, double wallHeight) {
	return { x_center + ((position%2)*2-1) * x_offset - ((position+1)%2) * wallWidth, y_center - wallHeight/2 };
}

PositionHolder LevelHelper::getSymmetricWallPositions_UD(int position, double x_center, double y_center, double y_offset, double wallWidth, double wallHeight) {
	return { x_center - wallWidth/2, y_center + ((position%2)*2-1) * y_offset - ((position+1)%2) * wallHeight };
}

PositionHolder LevelHelper::getSymmetricWallPositions_Corners(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight) {
	return { x_center + ((position%2)*2-1) * x_offset - ((position+1)%2) * wallWidth, y_center + ((position/2)*2-1) * y_offset - ((3-position)/2) * wallHeight };
	/*
	//LD, RD, LU, RU
	switch (position) {
		case 0: return { x_center - x_offset - wallWidth, y_center - y_offset - wallHeight };
		case 1: return { x_center + x_offset, y_center - y_offset - wallHeight };
		case 2: return { x_center - x_offset - wallWidth, y_center + y_offset };
		case 3: return { x_center + x_offset, y_center + y_offset };
		default: return {0,0};
	}
	*/
}

PositionHolder LevelHelper::getSymmetricWallPositions_DiagForwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight) {
	return { x_center + ((position%2)*2-1) * x_offset - ((position+1)%2) * wallWidth, y_center + ((position%2)*2-1) * y_offset - ((position+1)%2) * wallHeight };
	//note: has not been tested
}

PositionHolder LevelHelper::getSymmetricWallPositions_DiagBackwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight) {
	return { x_center + ((position%2)*2-1) * x_offset - ((position+1)%2) * wallWidth, y_center - ((position%2)*2-1) * y_offset - (position%2) * wallHeight };
	//note: has not been tested
}
