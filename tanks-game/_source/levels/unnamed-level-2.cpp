#include "unnamed-level-2.h"

#include "../constants.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"
#include "../level-manager.h"

ColorValueHolder UnnamedLevel2::getDefaultColor() const {
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> UnnamedLevel2::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.25f });
	weights.insert({ "random-dev", 0.25f });
	return weights;
}

void UnnamedLevel2::initialize() {
	ResetThings::tankPositionReset();

	ColorValueHolder color = getDefaultColor();
	//PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;

	//1. generate squares, then only use the diagonal edges
	//(maybe the diagonal can be randomized (either forward slash or backslash direction)?)
	//2. add thickness to the lines
	//3. adjust corners to be full
	//4. adjust rectangles to not overlap (extend vertical edges, shrink horizontal)

	const int EDGE_COUNT = 5; //count of horizontal edges
	const double EDGE_WIDTH = GAME_HEIGHT / (EDGE_COUNT+1); //since GAME_HEIGHT < GAME_WIDTH
	const double EDGE_THICKNESS = 20.0;

	//color = ColorValueHolder(0, 0, 0);
	if (EDGE_WIDTH - EDGE_THICKNESS > 0) { //walls will have positive widths
		//occurs when EDGE_COUNT < 15 given EDGE_THICKNESS = 20.0
		for (int i = 1; i <= EDGE_COUNT; i++) { //horizontal edges
			//line
			double xpos = GAME_WIDTH/2 - EDGE_WIDTH * double(EDGE_COUNT)/2 + EDGE_WIDTH * (i-1);
			double ypos = i * EDGE_WIDTH;
			double wall_width = EDGE_WIDTH;
			double wall_height = 0;

			//add thickness
			ypos -= EDGE_THICKNESS/2;
			wall_height = EDGE_THICKNESS;
			xpos -= EDGE_THICKNESS/2;
			wall_width += EDGE_THICKNESS;

			//adjust to not overlap (shrink width)
			xpos += EDGE_THICKNESS;
			wall_width -= 2*EDGE_THICKNESS;

			WallManager::pushWall(new Wall(xpos, ypos, wall_width, wall_height, color));
		}
	}

	//color = getDefaultColor();
	for (int i = 0; i <= EDGE_COUNT; i++) { //vertical edges
		//line
		double xpos = GAME_WIDTH/2 - EDGE_WIDTH * double(EDGE_COUNT)/2 + EDGE_WIDTH * (i-0);
		double ypos = i * EDGE_WIDTH;
		double wall_width = 0;
		double wall_height = EDGE_WIDTH;

		//add thickness
		xpos -= EDGE_THICKNESS/2;
		wall_width = EDGE_THICKNESS;
		ypos -= EDGE_THICKNESS/2;
		wall_height += EDGE_THICKNESS;

		//adjust to not overlap (extend height)
		//... do nothing, handled by horizontal edges

		//extra: make sure the walls are not out of bounds
		if (i == 0) {
			ypos += EDGE_THICKNESS/2;
			wall_height -= EDGE_THICKNESS/2;
		}
		if (i == EDGE_COUNT) {
			//not else for when EDGE_COUNT = 0
			wall_height -= EDGE_THICKNESS/2;
		}

		WallManager::pushWall(new Wall(xpos, ypos, wall_width, wall_height, color));
	}

	//only to avoid softlocks:
	//LevelHelper::pushSymmetricPowerups_DiagForwardSlash(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - 20, GAME_HEIGHT/2 - 20, "vanilla", "wallhack");
	PowerupManager::pushPowerup(new PowerSquare(20, 20, "vanilla", "wallhack"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH - 20, GAME_HEIGHT - 20, "vanilla", "wallhack"));
}

Level* UnnamedLevel2::factory() {
	return new UnnamedLevel2();
}

UnnamedLevel2::UnnamedLevel2() {
	GenericFactoryConstructionData constructionData;
	effects.push_back(LevelManager::makeLevelEffect("vanilla", "respawning_powerups", constructionData));
	//bool* temp = new bool[1]{ true };
	//constructionData = GenericFactoryConstructionData(1, temp);
	//effects.push_back(LevelManager::makeLevelEffect("dev", "symmetric_tanks", constructionData));
}
