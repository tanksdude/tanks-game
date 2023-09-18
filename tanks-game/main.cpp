//STL stuff
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <stdexcept>

//needed for callbacks and stuff:
#include "renderer.h"
#include "rng.h"
#include "color-value-holder.h"
#include "reset-things.h"
#include "developer-manager.h"
#include "keypress-manager.h"
//other:
#include "diagnostics.h"
#include "basic-ini-parser.h"
#include "custom-level-interpreter.h"

//managers:
#include "game-scene-manager.h"
#include "game-manager.h"
#include "tank-manager.h"
#include "bullet-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "level-manager.h"
//other:
#include "powerup-data-governor.h"
#include "hazard-data-governor.h"
#include "level-data-governor.h"

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
#include "tight-patrolling-corridor-level.h"
#include "dangerous-center-level.h"
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
#include "dev-no-walls-level-1.h"
#include "timed-reward-level.h" //added to vanilla-extra

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
#include "dev-backwards-movement-power.h" //self-explanatory
#include "wall-sparks-power.h" //create some extra bullets when hitting a wall
#include "dev-weird-extra-cannons-power.h" //adds some extra cannons at weird angles

#include "game-main-loop.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

const std::string GameWindowName = "PowerTanks Battle v0.2.5 NOT FINAL"; //this is not guaranteed to be correct every commit but likely will be



int main(int argc, char** argv) {
	GameManager::initializeINI("../tanks.ini");
	const BasicINIParser::BasicINIData& ini_data = GameManager::get_INI();

	if (ini_data.exists("UNIVERSAL", "RNGSeed")) {
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
		std::string name = GameWindowName;
		if (ini_data.exists("GRAPHICS_SETTINGS", "Position.StartX") && ini_data.exists("GRAPHICS_SETTINGS", "Position.StartY")) {
			int startX = std::stoi(ini_data.get("GRAPHICS_SETTINGS", "Position.StartX"));
			int startY = std::stoi(ini_data.get("GRAPHICS_SETTINGS", "Position.StartY"));
			if (ini_data.exists("GRAPHICS_SETTINGS", "Position.SizeX") && ini_data.exists("GRAPHICS_SETTINGS", "Position.SizeY")) {
				int sizeX = std::stoi(ini_data.get("GRAPHICS_SETTINGS", "Position.SizeX"));
				int sizeY = std::stoi(ini_data.get("GRAPHICS_SETTINGS", "Position.SizeY"));
				Renderer::PreInitialize(&argc, argv, name, startX, startY, sizeX, sizeY);
			} else {
				Renderer::PreInitialize(&argc, argv, name, startX, startY);
			}
		} else {
			Renderer::PreInitialize(&argc, argv, name);
		}
	}
	catch (const std::exception& e) {
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

	//prepare for incoming data:
	PowerupDataGovernor::initialize();
	HazardDataGovernor::initialize();
	LevelDataGovernor::initialize();

	//powers
	//vanilla (some are also "old"):
	PowerupDataGovernor::addPowerFactory(SpeedPower::factory);
	PowerupDataGovernor::addPowerFactory(WallhackPower::factory);
	PowerupDataGovernor::addPowerFactory(BouncePower::factory);
	PowerupDataGovernor::addPowerFactory(MultishotPower::factory);
	PowerupDataGovernor::addPowerFactory(TripleNamedPower::factory);
	PowerupDataGovernor::addPowerFactory(HomingPower::factory);
	PowerupDataGovernor::addPowerFactory(InvincibleNamedPower::factory);
	PowerupDataGovernor::addPowerFactory(BigNamedPower::factory);
	PowerupDataGovernor::addPowerFactory(MegaDeathPower::factory);
	PowerupDataGovernor::addPowerFactory(GrenadePower::factory);
	PowerupDataGovernor::addPowerFactory(FireNamedPower::factory);
	PowerupDataGovernor::addPowerFactory(BlastPower::factory);
	PowerupDataGovernor::addPowerFactory(BananaPower::factory);
	PowerupDataGovernor::addPowerFactory(GodmodePower::factory);
	//"extra":
	PowerupDataGovernor::addPowerFactory(MinesPower::factory);
	PowerupDataGovernor::addPowerFactory(ShotgunPower::factory);
	PowerupDataGovernor::addPowerFactory(TrackingPower::factory);
	PowerupDataGovernor::addPowerFactory(BarrierPower::factory);
	PowerupDataGovernor::addPowerFactory(ShieldPower::factory);

	//old:
	PowerupDataGovernor::addPowerFactory(OldBouncePower::factory);
	PowerupDataGovernor::addPowerFactory(OldBigNamedPower::factory);
	PowerupDataGovernor::addPowerFactory(OldMinesPower::factory);

	//dev:
	PowerupDataGovernor::addPowerFactory(DevLongInvincibleNamedPower::factory);
	PowerupDataGovernor::addPowerFactory(InversionPower::factory);
	PowerupDataGovernor::addPowerFactory(DevAnnoyingPower::factory);
	PowerupDataGovernor::addPowerFactory(UltraBouncePower::factory);
	PowerupDataGovernor::addPowerFactory(DevColorChangingPower::factory);
	PowerupDataGovernor::addPowerFactory(DevOtherStuffIsPoisonPower::factory);
	PowerupDataGovernor::addPowerFactory(DevBackwardsMovementPower::factory);
	PowerupDataGovernor::addPowerFactory(WallSparksPower::factory);
	PowerupDataGovernor::addPowerFactory(DevWeirdExtraCannonsPower::factory);

	//hazards
	//vanilla (some are also "old"):
	HazardDataGovernor::addCircleHazardFactory(StationaryTurretHazard::factory, StationaryTurretHazard::randomizingFactory);
	HazardDataGovernor::addCircleHazardFactory(TargetingTurretHazard::factory, TargetingTurretHazard::randomizingFactory);
	HazardDataGovernor::addCircleHazardFactory(PatrollingTurretHazard::factory, PatrollingTurretHazard::randomizingFactory);
	HazardDataGovernor::addCircleHazardFactory(MotherTurretHazard::factory, MotherTurretHazard::randomizingFactory);
	HazardDataGovernor::addCircleHazardFactory(GinormousTurretHazard::factory, GinormousTurretHazard::randomizingFactory);
	HazardDataGovernor::addRectHazardFactory(RectangularLightningHazard::factory, RectangularLightningHazard::randomizingFactory);
	HazardDataGovernor::addRectHazardFactory(HorizontalLightningHazard::factory, HorizontalLightningHazard::randomizingFactory);
	HazardDataGovernor::addRectHazardFactory(VerticalLightningHazard::factory, VerticalLightningHazard::randomizingFactory);
	HazardDataGovernor::addCircleHazardFactory(CircularLightningHazard::factory, CircularLightningHazard::randomizingFactory);
	HazardDataGovernor::addRectHazardFactory(RectangularLavaHazard::factory, RectangularLavaHazard::randomizingFactory);
	HazardDataGovernor::addCircleHazardFactory(CircularLavaHazard::factory, CircularLavaHazard::randomizingFactory);
	HazardDataGovernor::addRectHazardFactory(RectangularNoBulletZoneHazard::factory, RectangularNoBulletZoneHazard::randomizingFactory);
	HazardDataGovernor::addCircleHazardFactory(CircularNoBulletZoneHazard::factory, CircularNoBulletZoneHazard::randomizingFactory);
	//dev:
	HazardDataGovernor::addRectHazardFactory(DevWallHazard::factory, DevWallHazard::randomizingFactory);

	//level effects
	//vanilla (some are also "old"):
	LevelDataGovernor::addLevelEffectFactory(WindLevelEffect::factory);
	LevelDataGovernor::addLevelEffectFactory(RespawningPowerupsLevelEffect::factory);
	LevelDataGovernor::addLevelEffectFactory(MinefieldLevelEffect::factory);
	LevelDataGovernor::addLevelEffectFactory(IceLevelEffect::factory);
	LevelDataGovernor::addLevelEffectFactory(InvisibleWallsLevelEffect::factory);

	//dev:
	LevelDataGovernor::addLevelEffectFactory(DevSymmetricTanksLevelEffect::factory);

	//levels
	//vanilla (some are also "old"):
	LevelDataGovernor::addLevelFactory(DefaultRandomLevel::factory);
	LevelDataGovernor::addLevelFactory(EmptyLevel::factory);
	LevelDataGovernor::addLevelFactory(CorridorLevel::factory);
	LevelDataGovernor::addLevelFactory(BigFunLevel::factory);
	LevelDataGovernor::addLevelFactory(FewObstaclesLevel::factory);
	LevelDataGovernor::addLevelFactory(ConcealedPowerupsLevel::factory);
	LevelDataGovernor::addLevelFactory(ManyHazardsLevel::factory);
	//"extra":
	LevelDataGovernor::addLevelFactory(HidingPlacesLevel::factory);
	LevelDataGovernor::addLevelFactory(SneakyRewardLevel::factory);
	LevelDataGovernor::addLevelFactory(LightningCornersLevel::factory);
	LevelDataGovernor::addLevelFactory(LoneTurretLevel::factory);

	//special:
	LevelDataGovernor::addLevelFactory(TrickyManeuveringLevel::factory);
	LevelDataGovernor::addLevelFactory(MineHeavenLevel::factory);
	LevelDataGovernor::addLevelFactory(WinningPathLevel::factory);

	//old:
	LevelDataGovernor::addLevelFactory(OldEmptyLevel::factory);
	//TODO: add old versions of levels when (if) traps are made (but make sure they're disabled since no one likes them)

	//dev:
	LevelDataGovernor::addLevelFactory(DeveloperLevel0::factory);
	LevelDataGovernor::addLevelFactory(DeveloperLevel1::factory);
	LevelDataGovernor::addLevelFactory(DeveloperLevel2::factory);
	LevelDataGovernor::addLevelFactory(DeveloperLevel3::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel1::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel2::factory);
	LevelDataGovernor::addLevelFactory(DevNoWallsLevel1::factory);
	LevelDataGovernor::addLevelFactory(TimedRewardLevel::factory);
	LevelDataGovernor::addLevelFactory(TightPatrollingCorridorLevel::factory);
	LevelDataGovernor::addLevelFactory(DangerousCenterLevel::factory);

	//initialize managers and stuff:
	GameManager::Initialize();
	KeypressManager::Initialize();
	TankManager::initialize();
	BulletManager::initialize();
	PowerupManager::initialize();
	WallManager::initialize();
	LevelManager::initialize();
	HazardManager::initialize();
	Diagnostics::Initialize();
	GameSceneManager::Initialize();
	Renderer::Initialize();

	CustomLevelInterpreter::ProcessCustomLevels();

	Diagnostics::declareGraph("tick", ColorValueHolder(1.0f, 0.0f, 0.0f));
	Diagnostics::declareGraph("upload", ColorValueHolder(0.0f, 1.0f, 0.0f));
	Diagnostics::declareGraph("draw", ColorValueHolder(0.0f, 0.0f, 1.0f));

#if _DEBUG
	Diagnostics::setGraphYOffset(0);
	//Diagnostics::setGraphYOffset(GAME_HEIGHT);
#else
	//Diagnostics::setGraphYOffset(0);
	//Diagnostics::setGraphYOffset(GAME_HEIGHT);

	if (ini_data.exists("DEBUG", "PerformanceGraphOffset")) {
		Diagnostics::setGraphYOffset(GAME_HEIGHT * std::stod(ini_data.get("DEBUG", "PerformanceGraphOffset")));
	} else {
		Diagnostics::setGraphYOffset(GAME_HEIGHT);
	}
#endif

	//game mode:
	GameMainLoop* game;
	if (ini_data.exists("UNIVERAL", "GameMode")) {
		int mode = std::stoi(ini_data.get("UNIVERSAL", "GameMode"));
		switch (mode) {
			default:
				std::cerr << "Unknown GameMode \"" << mode << "\"!" << std::endl;
			case 0:
				//normal
				game = new GameMainLoop();
			case 1:
				//superfast shooting
				game = new GameMainLoop();
			case 2:
				//infinite world
				game = new GameMainLoop();
		}
	} else {
		game = new GameMainLoop();
	}
	GameSceneManager::pushScene(game);

	//main game code initialization stuff:
	ResetThings::firstGameInitialize("\"WASD 4 Life\"", "\"Arrow Keys R WINZ\"", *game);
	//they're good team names, deal with it
#if _DEBUG
	//LevelManager::pushLevel("dev", "dev3");
	//LevelManager::pushLevel("vanilla", "default_random");
	//LevelManager::pushLevel("random-vanilla", "sneaky_reward");
	//LevelManager::pushLevel("vanilla-extra", "tricky_maneuvering");
	//LevelManager::pushLevel("random-vanilla", "tight_patrolling_corridor");
	//LevelManager::pushLevel("dev", "unnamed3");
	ResetThings::firstLevelPush();
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

	Renderer::Uninitialize();

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
