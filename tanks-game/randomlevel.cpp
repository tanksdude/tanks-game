#include "randomlevel.h"
#include "mylib.h"

Wall* RandomLevel::makeNewRandomWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, ColorValueHolder c, double minW, double minH, double maxW, double maxH) {
	double w = randFunc2() * (maxW - minW) + minW;
	double h = randFunc2() * (maxH - minH) + minH;

	return new Wall(x_beginning + randFunc2() * (width_ofArea - w), y_beginning + randFunc2() * (height_ofArea - h), w, h, c);
}

std::string* RandomLevel::getRandomPowers(int count, bool replacement, int nameCount, std::string* names) {
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
					index = randFunc() * nameCount;
				} while (isInArray<int>(index, used, i+1));
				used[i] = index;
			} else {
				//there aren't values that can get chosen
				replacement = true; //instead of getting stuck or throwing an error, just put duplicates in (change eventually)
			}
		}
		if (replacement) {
			int index = randFunc() * nameCount;
			used[i] = index;
		}
	}

	for (int i = 0; i < count; i++) {
		n[i] = names[used[i]];
	}

	delete[] used;
	return n;
}

std::string RandomLevel::powerAlternate(int position, int rand, std::string p1, std::string p2) {
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

std::string RandomLevel::simplePowerAlternate(int position, int rand, std::string p1, std::string p2) {
	return ((position + rand) % 2 == 0 ? p1 : p2);
}

PositionHolder RandomLevel::getSymmetricPowerupPositions_LR(int position, double x_center, double y_center, double x_offset) {
	return { x_center + ((position%2)*2-1) * x_offset, y_center };
}

PositionHolder RandomLevel::getSymmetricPowerupPositions_UD(int position, double x_center, double y_center, double y_offset) {
	return { x_center, y_center + ((position%2)*2-1) * y_offset };
}

PositionHolder RandomLevel::getSymmetricPowerupPositions_Corners(int position, double x_center, double y_center, double x_offset, double y_offset) {
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

PositionHolder RandomLevel::getSymmetricPowerupPositions_DiagForwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset) {
	return { x_center + ((position%2)*2-1) * x_offset, y_center + ((position%2)*2-1) * y_offset };
	//note: has not been tested
}

PositionHolder RandomLevel::getSymmetricPowerupPositions_DiagBackwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset) {
	return { x_center + ((position%2)*2-1) * x_offset, y_center - ((position%2)*2-1) * y_offset };
	//note: has not been tested
}

PositionHolder RandomLevel::getSymmetricWallPositions_LR(int position, double x_center, double y_center, double x_offset, double wallWidth, double wallHeight) {
	return { x_center + ((position%2)*2-1) * x_offset - ((position+1)%2) * wallWidth, y_center - wallHeight/2 };
}

PositionHolder RandomLevel::getSymmetricWallPositions_UD(int position, double x_center, double y_center, double y_offset, double wallWidth, double wallHeight) {
	return { x_center - wallWidth/2, y_center + ((position%2)*2-1) * y_offset - (position%2) * wallHeight };
}

PositionHolder RandomLevel::getSymmetricWallPositions_Corners(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight) {
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

PositionHolder RandomLevel::getSymmetricWallPositions_DiagForwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight) {
	return { x_center + ((position%2)*2-1) * x_offset - ((position+1)%2) * wallWidth, y_center + ((position%2)*2-1) * y_offset - ((position+1)%2) * wallHeight };
	//note: has not been tested
}

PositionHolder RandomLevel::getSymmetricWallPositions_DiagBackwardSlash(int position, double x_center, double y_center, double x_offset, double y_offset, double wallWidth, double wallHeight) {
	return { x_center + ((position%2)*2-1) * x_offset - ((position+1)%2) * wallWidth, y_center - ((position%2)*2-1) * y_offset - (position%2) * wallHeight };
	//note: has not been tested
}
