//STL stuff
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread> //sleep (std::this_thread::sleep_for(), std::this_thread::yield())
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
#include "game-settings.h"
#include "mod-processor.h"
#include "statistics-handler.h"

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
#include "levels/default-random-level.h"
#include "levels/empty-level.h"
#include "levels/evened-corridors-level.h"
#include "levels/big-fun-level.h"
#include "levels/few-obstacles-level.h"
#include "levels/many-hazards-level.h"
#include "levels/dangerous-center-level.h"
#include "levels/untraditional-divider-level.h"
//"extra":
#include "levels/hiding-places-level.h"
#include "levels/tricky-maneuvering-level.h"
#include "levels/mine-heaven-level.h"
#include "levels/winning-path-level.h"
#include "levels/sneaky-reward-level.h"
#include "levels/tight-patrolling-corridor-level.h"
//old levels:
#include "levels/concealed-powerups-level.h"
#include "levels/lightning-corners-level.h"
#include "levels/lone-turret-level.h"
#include "levels/old-empty-level.h"
#include "levels/old-portal-level.h" //does not have the portal level effect
#include "levels/old-evened-corridors-level.h"
#include "levels/old-few-obstacles-level.h"
#include "levels/old-hiding-places-level.h"
//dev levels:
#include "levels/developer-level-0.h"
#include "levels/developer-level-1.h"
#include "levels/developer-level-2.h"
#include "levels/developer-level-3.h"
#include "levels/developer-level-4.h"
#include "levels/unnamed-level-1.h"
#include "levels/unnamed-level-2.h"
#include "levels/unnamed-level-3.h"
#include "levels/unnamed-level-4.h"
#include "levels/unnamed-level-6.h"
#include "levels/unnamed-level-9.h"
#include "levels/unnamed-level-10.h"
#include "levels/unnamed-level-11.h"
#include "levels/unnamed-level-12.h"
#include "levels/unnamed-level-13.h"
#include "levels/showcase-level-1.h"
#include "levels/showcase-level-2.h"
#include "levels/showcase-level-3.h"
#include "levels/dev-no-walls-level-1.h"
#include "levels/timed-reward-level.h" //added to vanilla-extra
#include "levels/terrifying-chaos-level.h" //added to vanilla-extra

//level effects:
#include "level-effects/respawning-powerups-level-effect.h"
#include "level-effects/magnetism-level-effect.h"
#include "level-effects/wind-level-effect.h"
#include "level-effects/minefield-level-effect.h"
#include "level-effects/ice-level-effect.h"
#include "level-effects/invisible-walls-level-effect.h"
//dev level effects:
#include "level-effects/dev-symmetric-tanks-level-effect.h"
#include "level-effects/dev-tiny-tanks-level-effect.h"

//hazards:
#include "hazards/stationary-turret-hazard.h"
#include "hazards/targeting-turret-hazard.h"
#include "hazards/patrolling-turret-hazard.h"
#include "hazards/mother-turret-hazard.h"
#include "hazards/ginormous-turret-hazard.h"
#include "hazards/rectangular-lightning-hazard.h"
#include "hazards/horizontal-lightning-hazard.h"
#include "hazards/vertical-lightning-hazard.h"
#include "hazards/circular-lightning-hazard.h"
#include "hazards/rectangular-lava-hazard.h"
#include "hazards/circular-lava-hazard.h"
#include "hazards/rectangular-no-bullet-zone-hazard.h"
#include "hazards/circular-no-bullet-zone-hazard.h"
#include "hazards/spiral-lava-hazard.h"
#include "hazards/gravity-well-hazard.h"
//dev hazards:
#include "hazards/dev-wall-hazard.h"
#include "hazards/reflecktor-hazard.h"
#include "hazards/cloud-hazard.h"

//powers:
#include "powers/speed-power.h"
#include "powers/wallhack-power.h"
#include "powers/multishot-power.h"
#include "powers/bounce-power.h"
#include "powers/triple-named-power.h"
#include "powers/homing-power.h"
#include "powers/invincible-named-power.h"
#include "powers/big-named-power.h"
#include "powers/mega-death-power.h"
#include "powers/grenade-power.h"
#include "powers/fire-named-power.h"
#include "powers/blast-power.h"
#include "powers/banana-power.h"
#include "powers/god-mode-power.h"
//"extra":
#include "powers/mines-power.h"
#include "powers/shotgun-power.h"
#include "powers/tracking-power.h"
#include "powers/barrier-power.h"
#include "powers/shield-power.h"
#include "powers/swarm-power.h"
//old powers:
#include "powers/old-bounce-power.h" //identical to bounce except it makes the tank smaller
#include "powers/old-big-named-power.h" //big but moves at half speed (not quarter) and the speed stacks
#include "powers/old-mines-power.h" //firing rate stacks
//dev powers:
#include "powers/dev-long-invincible-named-power.h" //invincible but lasts a long time
#include "powers/dev-colorless-long-invincible-named-power.h" //same but now it's not colored
#include "powers/the-super-named-power.h" //the strongest power
#include "powers/inversion-power.h" //flips left and right turning
#include "powers/annoying-power.h" //bullet doesn't hurt tank, merely pushes away
#include "powers/ultra-bounce-power.h" //pushes wall away when it bounces
#include "powers/dev-other-stuff-is-poison-power.h" //kills tank when touching normally-safe stuff
#include "powers/dev-backwards-movement-power.h" //self-explanatory
#include "powers/wall-sparks-power.h" //create some extra bullets when hitting a wall
#include "powers/edge-sparks-power.h" //create some extra bullets when hitting an edge
#include "powers/dev-weird-extra-cannons-power.h" //adds some extra cannons at weird angles
#include "powers/triple-spread-named-power.h" //triple but bullets spread out instead of being next to each other (the only power that does so)
#include "powers/ring-shooter-power.h" //multishot if it shot straight ahead
#include "powers/ancient-mega-death-power.h" //accelerating tracking bullet that goes through walls
#include "powers/banana-split-power.h" //banana but with a fixed angle range
#include "powers/trickster-circle-power.h" //moves back and forth in a circle motion
#include "powers/trickster-snake-power.h" //moves back and forth by moving up and down
#include "powers/dev-pusher-power.h" //lets the tank push walls and hazards
#include "powers/dev-movement-restarter-power.h" //makes the bullet start moving again once it stops moving (only happens once)
#include "powers/mine-layer-power.h" //lays down a bullet every so often

#include "game-main-loop.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <rpmalloc.h> //rest of rpmalloc stuff is in aaa_first.cpp

const std::string GameWindowName = "PowerTanks Battle post-v0.2.5.1 DEV"; //this is not guaranteed to be correct every commit but likely will be
const std::string INIFilePath = "tanks.ini";



int main(int argc, char** argv) {
	//rpmalloc initialization has to happen before main, so it's not called here

	GameManager::initializeINI(INIFilePath);
	GameManager::initializeSettings();
	const BasicINIParser::BasicINIData& ini_data = GameManager::get_INI();

	if (ini_data.exists("UNIVERSAL", "RNGSeed")) {
		long long seed = std::stoll(ini_data.get("UNIVERSAL", "RNGSeed"));
		GameRNG::Initialize(seed);
		VisualRNG::Initialize(seed);
		LevelRNG::Initialize(seed);
	} else {
		long long currTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		GameRNG::Initialize(currTime);
		VisualRNG::Initialize(currTime);
		LevelRNG::Initialize(currTime);
	}
	if (ini_data.exists("UNIVERSAL", "GraphicsContext")) {
		Renderer::SetContext(ini_data.get("UNIVERSAL", "GraphicsContext"));
	} else {
		Renderer::SetContext("OpenGL");
	}

	try {
		std::string name = GameWindowName;
		if (ini_data.exists("GRAPHICS_SETTINGS", "Position.StartX") && ini_data.exists("GRAPHICS_SETTINGS", "Position.StartY")) {
			int startX = std::stoi(ini_data.get("GRAPHICS_SETTINGS", "Position.StartX"));
			int startY = std::stoi(ini_data.get("GRAPHICS_SETTINGS", "Position.StartY"));
			if (ini_data.exists("GRAPHICS_SETTINGS", "Position.SizeMultiplier")) {
				double sizeMultiplier = std::stod(ini_data.get("GRAPHICS_SETTINGS", "Position.SizeMultiplier"));
				Renderer::PreInitialize(&argc, argv, name, startX, startY, sizeMultiplier);
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

	if (ini_data.exists("DEBUG", "DeveloperInsertMenuPowerAdditions")) {
		for (int i = 0; i < ini_data.length("DEBUG", "DeveloperInsertMenuPowerAdditions") / 3; i++) {
			DeveloperManager::addInsertPower(ini_data.get("DEBUG", "DeveloperInsertMenuPowerAdditions", i*3),
			                                 ini_data.get("DEBUG", "DeveloperInsertMenuPowerAdditions", i*3+1),
			                                 ini_data.get("DEBUG", "DeveloperInsertMenuPowerAdditions", i*3+2));
		}
	}

	//callbacks
	glfwSetFramebufferSizeCallback(Renderer::glfw_window, Renderer::windowResizeFunc);
	glfwSetKeyCallback(Renderer::glfw_window, KeypressManager::keyCallbackFunc);
	glfwSetMouseButtonCallback(Renderer::glfw_window, DeveloperManager::mouseButtonCallbackFunc);
	glfwSetCursorPosCallback(Renderer::glfw_window, DeveloperManager::mouseCursorPosCallbackFunc);
	glfwSetScrollCallback(Renderer::glfw_window, DeveloperManager::mouseScrollCallbackFunc);
	glfwSetWindowCloseCallback(Renderer::glfw_window, StatisticsHandler::DumpData);

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
	PowerupDataGovernor::addPowerFactory(SwarmPower::factory);

	//old:
	PowerupDataGovernor::addPowerFactory(OldBouncePower::factory);
	PowerupDataGovernor::addPowerFactory(OldBigNamedPower::factory);
	PowerupDataGovernor::addPowerFactory(OldMinesPower::factory);

	//dev:
	PowerupDataGovernor::addPowerFactory(DevLongInvincibleNamedPower::factory);
	PowerupDataGovernor::addPowerFactory(DevColorlessLongInvincibleNamedPower::factory);
	PowerupDataGovernor::addPowerFactory(TheSuperNamedPower::factory);
	PowerupDataGovernor::addPowerFactory(InversionPower::factory);
	PowerupDataGovernor::addPowerFactory(AnnoyingPower::factory);
	PowerupDataGovernor::addPowerFactory(UltraBouncePower::factory);
	PowerupDataGovernor::addPowerFactory(DevOtherStuffIsPoisonPower::factory);
	PowerupDataGovernor::addPowerFactory(DevBackwardsMovementPower::factory);
	PowerupDataGovernor::addPowerFactory(WallSparksPower::factory);
	PowerupDataGovernor::addPowerFactory(EdgeSparksPower::factory);
	PowerupDataGovernor::addPowerFactory(DevWeirdExtraCannonsPower::factory);
	PowerupDataGovernor::addPowerFactory(TripleSpreadNamedPower::factory);
	PowerupDataGovernor::addPowerFactory(RingShooterPower::factory);
	PowerupDataGovernor::addPowerFactory(AncientMegaDeathPower::factory);
	PowerupDataGovernor::addPowerFactory(BananaSplitPower::factory);
	PowerupDataGovernor::addPowerFactory(TricksterCirclePower::factory);
	PowerupDataGovernor::addPowerFactory(TricksterSnakePower::factory);
	PowerupDataGovernor::addPowerFactory(DevPusherPower::factory);
	PowerupDataGovernor::addPowerFactory(DevMovementRestarterPower::factory);
	PowerupDataGovernor::addPowerFactory(MineLayerPower::factory);

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
	HazardDataGovernor::addRectHazardFactory(SpiralLavaHazard::factory, SpiralLavaHazard::randomizingFactory);
	HazardDataGovernor::addCircleHazardFactory(GravityWellHazard::factory, GravityWellHazard::randomizingFactory);
	//dev:
	HazardDataGovernor::addRectHazardFactory(DevWallHazard::factory, DevWallHazard::randomizingFactory);
	HazardDataGovernor::addRectHazardFactory(ReflecktorHazard::factory, ReflecktorHazard::randomizingFactory);
	HazardDataGovernor::addCircleHazardFactory(CloudHazard::factory, CloudHazard::randomizingFactory);

	//level effects
	//vanilla (some are also "old"):
	LevelDataGovernor::addLevelEffectFactory(RespawningPowerupsLevelEffect::factory);
	LevelDataGovernor::addLevelEffectFactory(MagnetismLevelEffect::factory);
	LevelDataGovernor::addLevelEffectFactory(WindLevelEffect::factory);
	LevelDataGovernor::addLevelEffectFactory(MinefieldLevelEffect::factory);
	LevelDataGovernor::addLevelEffectFactory(IceLevelEffect::factory);
	LevelDataGovernor::addLevelEffectFactory(InvisibleWallsLevelEffect::factory);

	//dev:
	LevelDataGovernor::addLevelEffectFactory(DevSymmetricTanksLevelEffect::factory);
	LevelDataGovernor::addLevelEffectFactory(DevTinyTanksLevelEffect::factory);

	//levels
	//vanilla (some are also "old"):
	LevelDataGovernor::addLevelFactory(DefaultRandomLevel::factory);
	LevelDataGovernor::addLevelFactory(EmptyLevel::factory);
	LevelDataGovernor::addLevelFactory(EvenedCorridorsLevel::factory);
	LevelDataGovernor::addLevelFactory(BigFunLevel::factory);
	LevelDataGovernor::addLevelFactory(FewObstaclesLevel::factory);
	LevelDataGovernor::addLevelFactory(ManyHazardsLevel::factory);
	LevelDataGovernor::addLevelFactory(DangerousCenterLevel::factory);
	LevelDataGovernor::addLevelFactory(UntraditionalDividerLevel::factory);
	//"extra":
	LevelDataGovernor::addLevelFactory(HidingPlacesLevel::factory);
	LevelDataGovernor::addLevelFactory(TrickyManeuveringLevel::factory);
	LevelDataGovernor::addLevelFactory(MineHeavenLevel::factory);
	LevelDataGovernor::addLevelFactory(WinningPathLevel::factory);
	LevelDataGovernor::addLevelFactory(SneakyRewardLevel::factory);
	LevelDataGovernor::addLevelFactory(TightPatrollingCorridorLevel::factory);

	//old:
	LevelDataGovernor::addLevelFactory(ConcealedPowerupsLevel::factory);
	LevelDataGovernor::addLevelFactory(LightningCornersLevel::factory);
	LevelDataGovernor::addLevelFactory(LoneTurretLevel::factory);
	LevelDataGovernor::addLevelFactory(OldEmptyLevel::factory);
	LevelDataGovernor::addLevelFactory(OldPortalLevel::factory);
	LevelDataGovernor::addLevelFactory(OldEvenedCorridorsLevel::factory);
	LevelDataGovernor::addLevelFactory(OldFewObstaclesLevel::factory);
	LevelDataGovernor::addLevelFactory(OldHidingPlacesLevel::factory);
	//TODO: traps don't exist so these "old" levels technically aren't completely identical
	//also power mixing can't be disabled anymore, but screw keeping that option

	//dev:
	LevelDataGovernor::addLevelFactory(DeveloperLevel0::factory);
	LevelDataGovernor::addLevelFactory(DeveloperLevel1::factory);
	LevelDataGovernor::addLevelFactory(DeveloperLevel2::factory);
	LevelDataGovernor::addLevelFactory(DeveloperLevel3::factory);
	LevelDataGovernor::addLevelFactory(DeveloperLevel4::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel1::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel2::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel3::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel4::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel6::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel9::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel10::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel11::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel12::factory);
	LevelDataGovernor::addLevelFactory(UnnamedLevel13::factory);
	LevelDataGovernor::addLevelFactory(ShowcaseLevel1::factory);
	LevelDataGovernor::addLevelFactory(ShowcaseLevel2::factory);
	LevelDataGovernor::addLevelFactory(ShowcaseLevel3::factory);
	LevelDataGovernor::addLevelFactory(DevNoWallsLevel1::factory);
	LevelDataGovernor::addLevelFactory(TimedRewardLevel::factory);
	LevelDataGovernor::addLevelFactory(TerrifyingChaosLevel::factory);

	//initialize managers and stuff:
	GameManager::Initialize();
	KeypressManager::Initialize();
	TankManager::initialize();
	BulletManager::initialize();
	PowerupManager::initialize();
	WallManager::initialize();
	HazardManager::initialize();
	LevelManager::initialize();
	Diagnostics::Initialize();
	GameSceneManager::Initialize();
	Renderer::Initialize();

	ModProcessor::ProcessMods();

	Diagnostics::declareGraph("tick", ColorValueHolder(1.0f, 0.0f, 0.0f));
	Diagnostics::declareGraph("upload", ColorValueHolder(0.0f, 1.0f, 0.0f));
	Diagnostics::declareGraph("draw", ColorValueHolder(0.0f, 0.0f, 1.0f));
	Diagnostics::declareGraph("all", ColorValueHolder(1.0f, 1.0f, 1.0f));

	if (ini_data.exists("DEBUG", "PerformanceGraphOffset")) {
		Diagnostics::setGraphYOffset(GAME_HEIGHT * std::stod(ini_data.get("DEBUG", "PerformanceGraphOffset")));
	} else {
		Diagnostics::setGraphYOffset(GAME_HEIGHT);
	}

	//game mode:
	GameMainLoop* game;
	if (ini_data.exists("UNIVERAL", "GameMode")) {
		int mode = std::stoi(ini_data.get("UNIVERSAL", "GameMode"));
		switch (mode) {
			default:
				std::cerr << "Unknown GameMode \"" << mode << "\"!" << std::endl;
				[[fallthrough]];
			case 0:
				//normal
				game = new GameMainLoop();
				break;
			case 1:
				//superfast shooting
				game = new GameMainLoop();
				break;
			case 2:
				//infinite world
				game = new GameMainLoop();
				break;
		}
	} else {
		game = new GameMainLoop();
	}
	GameSceneManager::pushScene(game);

	//main game code initialization stuff:
	ResetThings::firstGameInitialize("\"WASD 4 Life\"", "\"Arrow Keys R WINZ\"", *game);
	//they're good team names, deal with it
	ResetThings::firstLevelPush();
	ResetThings::firstReset();

	std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl << std::endl;

	//main loop:
	//double pastTime = glfwGetTime();
	while (!glfwWindowShouldClose(Renderer::glfw_window)) {
		double currTime = glfwGetTime(); //TODO: set this outside the loop, +=10ms if <10ms frame
		//std::cout << "time passed: " << (currTime - pastTime)*1000 << "ms";

		glfwPollEvents();
		GameSceneManager::TickScenes(100);
		double endTime = glfwGetTime();
		//pastTime = endTime;

		const double timeDiffMS = (endTime - currTime) * 1000;
		const double sleepTime = (10 - timeDiffMS) / 1000;
		//std::cout << " | time took: " << timeDiffMS << "ms | sleep time : " << sleepTime*1000 << "ms" << std::endl;
		/*
		if (timeDiffMS < 10) {
			std::this_thread::sleep_until(std::chrono::steady_clock::now() + std::chrono::duration<double>(sleepTime));
		}
		*/
		//sleeping is not very reliable, so just spin instead (the poor CPU cycles wasted)
		double temp = glfwGetTime();
		while (temp - currTime < 10/1000.0) {
			std::this_thread::yield(); //better than raw spinning...
			temp = glfwGetTime();
		}
		//TODO: find a better way for frame delaying
	}

	Renderer::Uninitialize();

	rpmalloc_finalize();
	return 0;
}
