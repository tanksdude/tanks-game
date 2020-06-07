#include "randomlevel.h"
#include "mylib.h"

Wall* RandomLevel::makeNewWall(double x_beginning, double y_beginning, double width_ofArea, double height_ofArea, ColorValueHolder c, double minW, double minH, double maxW, double maxH) {
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
		if(replacement) {
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
	return (((int)ceil( float((position*2)%3) / 3) + rand) % 2 == 0 ? p1 : p2);
	/*
	{0,   1,   2,  3} // start
	{0,   2,   4,  6} // *2
	{0,   2,   1,  0} // %3, then cast to float
	{0,  .6,  .3,  0} // /3, then add rand
	{0,  .6,  .3,  0} (rand=0) -> ceil -> {0, 1, 1, 0}
	{1, 1.6, 1.3,  1} (rand=1) -> ceil -> {1, 2, 2, 1}
	{0, 1, 1, 0} OR	{1, 0, 0, 1} // %2
	*/
}
