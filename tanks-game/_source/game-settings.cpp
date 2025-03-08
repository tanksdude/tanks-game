#include "game-settings.h"

#include "constants.h"

void GameSettings::Initialize(const BasicINIParser::BasicINIData& ini_data) {
	//[DEBUG]
	PerformanceGraphEnable = ini_data.exists("DEBUG", "PerformanceGraphEnable") && std::stoi(ini_data.get("DEBUG", "PerformanceGraphEnable"));
	EnableDebugDrawing = ini_data.exists("DEBUG", "EnableDebugDrawing") && std::stoi(ini_data.get("DEBUG", "EnableDebugDrawing"));

	//[GRAPHICS_SETTINGS]
	Bullet_PerformanceMode = ini_data.exists("GRAPHICS_SETTINGS", "Bullet.PerformanceMode") && std::stoi(ini_data.get("GRAPHICS_SETTINGS", "Bullet.PerformanceMode"));

	//[GAME_OPTIONS]
	GameLevelPlaylist = ini_data.exists("GAME_OPTIONS", "GameLevelPlaylist")
		? ini_data.get("GAME_OPTIONS", "GameLevelPlaylist") : "random-vanilla";
	if (ini_data.exists("GAME_OPTIONS", "GameForceSameLevel") && ini_data.length("GAME_OPTIONS", "GameForceSameLevel") >= 2) {
		GameForceSameLevel = true;
		GameForceSameLevel_identifier = { ini_data.get("GAME_OPTIONS", "GameForceSameLevel", 0), ini_data.get("GAME_OPTIONS", "GameForceSameLevel", 1) };
	} else {
		GameForceSameLevel = false;
		GameForceSameLevel_identifier = { "", "" };
	
	}
	ReportCurrentLevel = ini_data.exists("GAME_OPTIONS", "ReportCurrentLevel") && std::stoi(ini_data.get("GAME_OPTIONS", "ReportCurrentLevel"));

	LimitBullets = ini_data.exists("GAME_OPTIONS", "LimitBullets") && std::stoi(ini_data.get("GAME_OPTIONS", "LimitBullets"));
	MaxBullets = ini_data.exists("GAME_OPTIONS", "MaxBullets")
		? std::stoi(ini_data.get("GAME_OPTIONS", "MaxBullets")) : -1; //-1 does not mean infinite
	FewerExtraShootingBullets = ini_data.exists("GAME_OPTIONS", "FewerExtraShootingBullets") && std::stoi(ini_data.get("GAME_OPTIONS", "FewerExtraShootingBullets"));
	ShootingCooldown = ini_data.exists("GAME_OPTIONS", "ShootingCooldown")
		? std::stod(ini_data.get("GAME_OPTIONS", "ShootingCooldown")) : SHOOT_COOLDOWN;
	PowerupDurationBaseTime = ini_data.exists("GAME_OPTIONS", "PowerupDurationBaseTime")
		? std::stod(ini_data.get("GAME_OPTIONS", "PowerupDurationBaseTime")) : POWER_COOLDOWN;

	RestrictTankTurning = ini_data.exists("GAME_OPTIONS", "RestrictTankTurning") && std::stoi(ini_data.get("GAME_OPTIONS", "RestrictTankTurning"));
	NoTankAcceleration = ini_data.exists("GAME_OPTIONS", "NoTankAcceleration") && std::stoi(ini_data.get("GAME_OPTIONS", "NoTankAcceleration"));

	//[SPECIFIC_ADJUSTMENTS]
	//BananaPower_ExplodeCount;
}

GameSettings::GameSettings() {
	//nothing?
}
