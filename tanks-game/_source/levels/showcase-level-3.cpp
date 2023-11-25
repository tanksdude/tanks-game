#include "showcase-level-3.h"

#include "../constants.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

ColorValueHolder ShowcaseLevel3::getDefaultColor() const {
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> ShowcaseLevel3::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.25f });
	weights.insert({ "random-dev", 0.25f });
	return weights;
}

void ShowcaseLevel3::initialize() {
	ResetThings::tankPositionReset(ResetThings::default_tankToEdgeDist, GAME_HEIGHT/2);

	ColorValueHolder color = getDefaultColor();
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;

	posArr = new double[3]{ GAME_WIDTH/2 - 160, GAME_HEIGHT/2, PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "stationary_turret", constructionData);

	posArr = new double[3]{ GAME_WIDTH/2 - 120, GAME_HEIGHT/2, PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "targeting_turret", constructionData);

	int* patrolCount = new int[1]{ 2 };
	double* patrolRoute = new double[2*2]{
		GAME_WIDTH/2 - 80, GAME_HEIGHT/2 - 20,
		GAME_WIDTH/2 - 80, GAME_HEIGHT/2 + 20
	};

	posArr = new double[3]{ patrolRoute[0], patrolRoute[1], -PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(2*2, patrolRoute);
	HazardManager::pushCircleHazard("vanilla", "patrolling_turret", constructionData);

	posArr = new double[3]{ GAME_WIDTH/2 + 0, GAME_HEIGHT/2, PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "mother_turret", constructionData);

	posArr = new double[3]{ GAME_WIDTH/2 + 160, GAME_HEIGHT/2, PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "ginormous_turret", constructionData);
}

Level* ShowcaseLevel3::factory() {
	return new ShowcaseLevel3();
}

ShowcaseLevel3::ShowcaseLevel3() { return; }
