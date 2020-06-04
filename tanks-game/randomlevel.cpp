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
