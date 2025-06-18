#pragma once
#include <string>
#include <utility>
#include <vector>

#include "basic-ini-parser.h"

class GameSettings {
public:
	void Initialize(const BasicINIParser::BasicINIData& ini_data);

public: //safe to be public because a const reference is used, and I don't want to write getters
	//[DEBUG]
	bool PerformanceGraphEnable;
	bool EnableDebugDrawing;

	//[GRAPHICS_SETTINGS]
	bool Bullet_PerformanceMode;

	//[GAME_OPTIONS]
	std::string GameLevelPlaylist;
	bool GameForceSameLevel;
	std::pair<std::string, std::string> GameForceSameLevel_identifier;
	std::vector<std::pair<std::pair<std::string, std::string>, float>> CustomLevelPlaylist;
	bool ReportCurrentLevel;

	bool LimitBullets;
	int MaxBullets;
	bool FewerExtraShootingBullets;
	double ShootingCooldown;
	double PowerupDurationBaseTime;

	bool RestrictTankTurning;
	bool NoTankAcceleration;
	bool AlwaysShootingMode;

	//[SPECIFIC_ADJUSTMENTS]
	//int BananaPower_ExplodeCount;

public:
	GameSettings();
};
