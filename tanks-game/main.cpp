#pragma once
//main stuff
#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <unordered_map>
#include <stdexcept>

//GPU rendering:
#include "vertex-array.h"
#include "vertex-buffer.h"
#include "index-buffer.h"
#include "shader.h"
#include "renderer.h"
#include <glm.hpp> //GLM is overkill but that's okay
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

//important stuff:
#include "rng.h"
#include "color-value-holder.h"
#include "background-rect.h"
#include "tank.h"
#include "wall.h"
#include "bullet.h"
#include "power-square.h"
#include "level.h"
#include "circle-hazard.h"
#include "rect-hazard.h"

//managers:
#include "game-scene-manager.h"
#include "developer-manager.h"
#include "game-manager.h"
#include "keypress-manager.h"
#include "tank-manager.h"
#include "bullet-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "level-manager.h"
#include "hazard-manager.h"
#include "diagnostics.h"
#include "basic-ini-parser.h"

//classes with important handling functions:
#include "collision-handler.h"
#include "reset-things.h"
#include "priority-handler.h"
#include "color-mixer.h"
#include "power-function-helper.h"
#include "end-game-handler.h"
#include "physics-handler.h"

//levels:
#include "default-random-level.h"
#include "empty-level.h"
#include "corridor-level.h"
#include "big-fun-level.h"
#include "few-obstacles-level.h"
#include "concealed-powerups-level.h"
#include "many-hazards-level.h"
//"extra":
#include "hiding-places-level.h"
#include "sneaky-reward-level.h"
#include "lightning-corners-level.h"
#include "lone-turret-level.h"
//special levels:
#include "tricky-maneuvering-level.h"
#include "mine-heaven-level.h"
#include "winning-path-level.h"
//old levels:
#include "old-empty-level.h"
//dev levels:
#include "developer-level-0.h"
#include "developer-level-1.h"
#include "developer-level-2.h"
#include "developer-level-3.h"
#include "unnamed-level-1.h"
#include "unnamed-level-2.h"
#include "unnamed-level-3.h"
#include "dev-no-walls-level-1.h"
#include "timed-reward-level.h"
#include "tight-patrolling-corridor-level.h"

//level effects:
#include "wind-level-effect.h"
#include "respawning-powerups-level-effect.h"
#include "minefield-level-effect.h"
#include "ice-level-effect.h"
#include "invisible-walls-level-effect.h"
//dev level effects:
#include "dev-symmetric-tanks-level-effect.h"

//hazards:
#include "stationary-turret-hazard.h"
#include "targeting-turret-hazard.h"
#include "patrolling-turret-hazard.h"
#include "mother-turret-hazard.h"
#include "ginormous-turret-hazard.h" //currently not used
#include "rectangular-lightning-hazard.h"
#include "horizontal-lightning-hazard.h"
#include "vertical-lightning-hazard.h"
#include "circular-lightning-hazard.h"
#include "rectangular-lava-hazard.h"
#include "circular-lava-hazard.h"
#include "rectangular-no-bullet-zone-hazard.h"
#include "circular-no-bullet-zone-hazard.h"
//dev hazards:
#include "dev-wall-hazard.h"

//powers:
#include "speed-power.h"
#include "wallhack-power.h"
#include "multishot-power.h"
#include "bounce-power.h"
#include "triple-named-power.h"
#include "homing-power.h"
#include "invincible-named-power.h"
#include "big-named-power.h"
#include "mega-death-power.h"
#include "grenade-power.h"
#include "fire-named-power.h"
#include "blast-power.h"
#include "banana-power.h"
#include "god-mode-power.h"
//"extra":
#include "mines-power.h"
#include "shotgun-power.h"
#include "tracking-power.h"
#include "barrier-power.h"
#include "shield-power.h"
//old powers:
#include "old-bounce-power.h" //identical to bounce except it makes the tank smaller
#include "old-big-named-power.h" //big but moves at half speed (not quarter) and the speed stacks
#include "old-mines-power.h" //firing rate stacks
//dev powers:
#include "dev-long-invincible-named-power.h" //invincible but lasts a long time
#include "inversion-power.h" //flips left and right turning
#include "dev-annoying-power.h" //bullet doesn't hurt tank, merely pushes away
#include "ultra-bounce-power.h" //pushes wall away when it bounces
#include "dev-color-changing-power.h" //color changes based on tank proximity
#include "dev-other-stuff-is-poison-power.h" //kills tank when touching normally-safe stuff

//a lot of the includes aren't needed anymore thanks to GameMainLoop, but may as well keep them
#include "game-main-loop.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

//tank team rules:
//0  = no team or default team
//-1 = hazard team (hazards can be split up into multiple teams, but by default there's only one)
//1  = default teamID of tank #1 ("WASD")
//2  = default teamID of tank #2 ("Arrow Keys")
//tank teams always > 0



int main(int argc, char** argv) {
	GameManager::initializeINI("../tanks.ini");
	const BasicINIParser::BasicINIData& ini_data = GameManager::get_INI();

	if (ini_data.exists("UNIVERSAL", "RNGSeed") && (ini_data.get("UNIVERSAL", "RNGSeed") != "")) {
		RNG::Initialize(std::stoll(ini_data.get("UNIVERSAL", "RNGSeed")));
	} else {
		RNG::Initialize(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	}
	if (ini_data.exists("UNIVERSAL", "GraphicsContext")) {
		Renderer::SetContext(ini_data.get("UNIVERSAL", "GraphicsContext"));
	} else {
		Renderer::SetContext("OpenGL");
	}
	//std::cout << "RNGSeed: " << ini_data.get("", "RNGSeed") << std::endl;
	//std::cout << "GraphisContext: " << ini_data.get("", "GraphicsContext") << std::endl;

	try {
		std::string name = "PowerTanks Battle v0.2.5 NOT FINAL"; //this is not guaranteed to be correct every commit but likely will be
		if (ini_data.exists("GRAPHICS_SETTINGS", "Position.StartX") && ini_data.exists("GRAPHICS_SETTINGS", "Position.StartY")) {
			int startX = std::stoi(ini_data.get("GRAPHICS_SETTINGS", "Position.StartX"));
			int startY = std::stoi(ini_data.get("GRAPHICS_SETTINGS", "Position.StartY"));
			if (ini_data.exists("GRAPHICS_SETTINGS", "Position.SizeX") && ini_data.exists("GRAPHICS_SETTINGS", "Position.SizeY")) {
				std::string sizeX = ini_data.get("GRAPHICS_SETTINGS", "Position.SizeX");
				std::string sizeY = ini_data.get("GRAPHICS_SETTINGS", "Position.SizeY");
				if ((sizeX == "") || (sizeY == "")) {
					Renderer::PreInitialize(&argc, argv, name, startX, startY);
				} else {
					Renderer::PreInitialize(&argc, argv, name, startX, startY, std::stoi(sizeX), std::stoi(sizeY));
				}
			} else {
				Renderer::PreInitialize(&argc, argv, name, startX, startY);
			}
		} else {
			Renderer::PreInitialize(&argc, argv, name);
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	// Set callback for drawing the scene
	glutDisplayFunc(GameSceneManager::DrawScenes);
	//glutDisplayFunc(GameMainLoop::drawMain);
	//glutDisplayFunc(GameMainLoop::drawAllLayers);

	// Set callback for resizing the window
	glutReshapeFunc(Renderer::windowResizeFunc);

	//mouse clicking
	glutMouseFunc(DeveloperManager::mouseClickFunc);

	// Set callback to handle mouse dragging
	glutMotionFunc(DeveloperManager::mouseDragFunc);

	// Set callback to handle keyboard events
	glutKeyboardFunc(KeypressManager::setNormalKey);

	//callback for keyboard up events
	glutKeyboardUpFunc(KeypressManager::unsetNormalKey);

	//special keyboard down
	glutSpecialFunc(KeypressManager::setSpecialKey);

	//special keyboard up
	glutSpecialUpFunc(KeypressManager::unsetSpecialKey);

	//mousewheel
	glutMouseWheelFunc(DeveloperManager::mouseWheelFunc);

	// Set callback for the idle function
	//glutIdleFunc(draw);

	//powers:
	//vanilla (some are also "old"):
	PowerupManager::addPowerFactory(SpeedPower::factory);
	PowerupManager::addPowerFactory(WallhackPower::factory);
	PowerupManager::addPowerFactory(BouncePower::factory);
	PowerupManager::addPowerFactory(MultishotPower::factory);
	PowerupManager::addPowerFactory(TripleNamedPower::factory);
	PowerupManager::addPowerFactory(HomingPower::factory);
	PowerupManager::addPowerFactory(InvincibleNamedPower::factory);
	PowerupManager::addPowerFactory(BigNamedPower::factory);
	PowerupManager::addPowerFactory(MegaDeathPower::factory);
	PowerupManager::addPowerFactory(GrenadePower::factory);
	PowerupManager::addPowerFactory(FireNamedPower::factory);
	PowerupManager::addPowerFactory(BlastPower::factory);
	PowerupManager::addPowerFactory(BananaPower::factory);
	PowerupManager::addPowerFactory(GodmodePower::factory);
	//"extra":
	PowerupManager::addPowerFactory(MinesPower::factory);
	PowerupManager::addPowerFactory(ShotgunPower::factory);
	PowerupManager::addPowerFactory(TrackingPower::factory);
	PowerupManager::addPowerFactory(BarrierPower::factory);
	PowerupManager::addPowerFactory(ShieldPower::factory);

	//old:
	PowerupManager::addPowerFactory(OldBouncePower::factory);
	PowerupManager::addPowerFactory(OldBigNamedPower::factory);
	PowerupManager::addPowerFactory(OldMinesPower::factory);

	//dev:
	PowerupManager::addPowerFactory(DevLongInvincibleNamedPower::factory);
	PowerupManager::addPowerFactory(InversionPower::factory);
	PowerupManager::addPowerFactory(DevAnnoyingPower::factory);
	PowerupManager::addPowerFactory(UltraBouncePower::factory);
	PowerupManager::addPowerFactory(DevColorChangingPower::factory);
	PowerupManager::addPowerFactory(DevOtherStuffIsPoisonPower::factory);

	//hazards
	//vanilla (some are also "old"):
	HazardManager::addCircleHazardFactory(StationaryTurretHazard::factory, StationaryTurretHazard::randomizingFactory);
	HazardManager::addCircleHazardFactory(TargetingTurretHazard::factory, TargetingTurretHazard::randomizingFactory);
	HazardManager::addCircleHazardFactory(PatrollingTurretHazard::factory, PatrollingTurretHazard::randomizingFactory);
	HazardManager::addCircleHazardFactory(MotherTurretHazard::factory, MotherTurretHazard::randomizingFactory);
	HazardManager::addCircleHazardFactory(GinormousTurretHazard::factory, GinormousTurretHazard::randomizingFactory);
	HazardManager::addRectHazardFactory(RectangularLightningHazard::factory, RectangularLightningHazard::randomizingFactory);
	HazardManager::addRectHazardFactory(HorizontalLightningHazard::factory, HorizontalLightningHazard::randomizingFactory);
	HazardManager::addRectHazardFactory(VerticalLightningHazard::factory, VerticalLightningHazard::randomizingFactory);
	HazardManager::addCircleHazardFactory(CircularLightningHazard::factory, CircularLightningHazard::randomizingFactory);
	HazardManager::addRectHazardFactory(RectangularLavaHazard::factory, RectangularLavaHazard::randomizingFactory);
	HazardManager::addCircleHazardFactory(CircularLavaHazard::factory, CircularLavaHazard::randomizingFactory);
	HazardManager::addRectHazardFactory(RectangularNoBulletZoneHazard::factory, RectangularNoBulletZoneHazard::randomizingFactory);
	HazardManager::addCircleHazardFactory(CircularNoBulletZoneHazard::factory, CircularNoBulletZoneHazard::randomizingFactory);
	//dev:
	HazardManager::addRectHazardFactory(DevWallHazard::factory, DevWallHazard::randomizingFactory);

	//level effects
	//vanilla (some are also "old"):
	LevelManager::addLevelEffectFactory(WindLevelEffect::factory);
	LevelManager::addLevelEffectFactory(RespawningPowerupsLevelEffect::factory);
	LevelManager::addLevelEffectFactory(MinefieldLevelEffect::factory);
	LevelManager::addLevelEffectFactory(IceLevelEffect::factory);
	LevelManager::addLevelEffectFactory(InvisibleWallsLevelEffect::factory);

	//dev:
	LevelManager::addLevelEffectFactory(DevSymmetricTanksLevelEffect::factory);

	//levels
	//vanilla (some are also "old"):
	LevelManager::addLevelFactory(DefaultRandomLevel::factory);
	LevelManager::addLevelFactory(EmptyLevel::factory);
	LevelManager::addLevelFactory(CorridorLevel::factory);
	LevelManager::addLevelFactory(BigFunLevel::factory);
	LevelManager::addLevelFactory(FewObstaclesLevel::factory);
	LevelManager::addLevelFactory(ConcealedPowerupsLevel::factory);
	LevelManager::addLevelFactory(ManyHazardsLevel::factory);
	//"extra":
	LevelManager::addLevelFactory(HidingPlacesLevel::factory);
	LevelManager::addLevelFactory(SneakyRewardLevel::factory);
	LevelManager::addLevelFactory(LightningCornersLevel::factory);
	LevelManager::addLevelFactory(LoneTurretLevel::factory);

	//special:
	LevelManager::addLevelFactory(TrickyManeuveringLevel::factory);
	LevelManager::addLevelFactory(MineHeavenLevel::factory);
	LevelManager::addLevelFactory(WinningPathLevel::factory);

	//old:
	LevelManager::addLevelFactory(OldEmptyLevel::factory);
	//TODO: add old versions of levels when (if) traps are made (but make sure they're disabled since no one likes them)

	//dev:
	LevelManager::addLevelFactory(DeveloperLevel0::factory);
	LevelManager::addLevelFactory(DeveloperLevel1::factory);
	LevelManager::addLevelFactory(DeveloperLevel2::factory);
	LevelManager::addLevelFactory(DeveloperLevel3::factory);
	LevelManager::addLevelFactory(UnnamedLevel1::factory);
	LevelManager::addLevelFactory(UnnamedLevel2::factory);
	LevelManager::addLevelFactory(UnnamedLevel3::factory);
	LevelManager::addLevelFactory(DevNoWallsLevel1::factory);
	LevelManager::addLevelFactory(TimedRewardLevel::factory);
	LevelManager::addLevelFactory(TightPatrollingCorridorLevel::factory);

	//initialize managers and stuff:
	GameManager::Initialize();
	KeypressManager::Initialize();
	TankManager::initialize();
	BulletManager::initialize();
	PowerupManager::initialize();
	WallManager::initialize();
	LevelManager::initialize();
	HazardManager::initialize();
	Renderer::Initialize(); //static VAO, VBO, and IBO has better performance
	BackgroundRect::initializeGPU();
	Diagnostics::Initialize();
	GameSceneManager::Initialize();

	Diagnostics::declareGraph("tick", ColorValueHolder(1.0f, 0.0f, 0.0f));
	Diagnostics::declareGraph("draw", ColorValueHolder(0.0f, 0.0f, 1.0f));

#if _DEBUG
	Diagnostics::setGraphYOffset(0);
	//Diagnostics::setGraphYOffset(GAME_HEIGHT);
#else
	//TODO: INI setting?
	//Diagnostics::setGraphYOffset(0);
	Diagnostics::setGraphYOffset(GAME_HEIGHT);
#endif

	//game mode:
	if (ini_data.exists("UNIVERAL", "GameMode")) {
		int mode = std::stoi(ini_data.get("UNIVERSAL", "GameMode"));
		switch (mode) {
			default:
				std::cerr << "Unknown GameMode \"" << mode << "\"!" << std::endl;
			case 0:
				//normal
				GameSceneManager::pushScene(new GameMainLoop());
			case 1:
				//superfast shooting
				GameSceneManager::pushScene(new GameMainLoop());
			case 2:
				//infinite world
				GameSceneManager::pushScene(new GameMainLoop());
		}
	} else {
		GameSceneManager::pushScene(new GameMainLoop());
	}

	//main game code initialization stuff:
	ResetThings::firstGameInitialize("\"WASD 4 Life\"", "\"Arrow Keys R WINZ\"");
	//they're good team names, deal with it
#if _DEBUG
	LevelManager::pushLevel("dev", "dev3");
	//LevelManager::pushLevel("vanilla", "default_random");
	//LevelManager::pushLevel("random-vanilla", "sneaky_reward");
	//LevelManager::pushLevel("vanilla-extra", "tricky_maneuvering");
	//LevelManager::pushLevel("random-vanilla", "tight_patrolling_corridor");
	//LevelManager::pushLevel("dev", "unnamed3");
#else
	ResetThings::firstLevelPush();
#endif
	ResetThings::firstReset();

	std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl << std::endl;

	//framelimiter
	//glutTimerFunc(1000/physicsRate, tick, physicsRate); //see GameMainLoop
	//GameMainLoop::Tick();
	GameSceneManager::TickScenes(100);

	// Start the main loop
	glutMainLoop();

	return 0; //does this even do anything?
}

/*
 * estimated total completion:
 * 100% required GPU drawing stuff!
 * 20% theoretical GPU stuff (may not attempt)
 * * add a gradient shader
 * * gotta learn how to do batching
 * * make things more efficient (way easier said than done, I suppose)
 * * drawing layers
 * 97.5?% theoretical foundation: no hazard powers
 * * hazard powers: ~50%
 * * levels: ~95%? not too sure
 * 90% actual foundation: not every "modification function" actually does something
 * * almost everything needed is somewhere, so copy-and-paste will be the solution (unless I can figure out member function pointers)
 * ~50% game code:
 * * I don't know what will be final beyond the ideas located in power.cpp, hazard.cpp, level.cpp, and elsewhere
 * * it's just an estimate
 * * 100% probably won't be "finished" on this scale (it could jump from 60% to 100%)
 */
