#pragma once
//main stuff
#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <unordered_map>
#include <stdexcept>

//GPU rendering:
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "shader.h"
#include "renderer.h"
#include <glm.hpp> //GLM is overkill but that's okay
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

//important stuff:
#include "rng.h"
#include "colorvalueholder.h"
#include "backgroundrect.h"
#include "tank.h"
#include "wall.h"
#include "bullet.h"
#include "powersquare.h"
#include "level.h"
#include "circlehazard.h"
#include "recthazard.h"

//managers:
#include "developermanager.h"
#include "gamemanager.h"
#include "keypressmanager.h"
#include "tankmanager.h"
#include "bulletmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "levelmanager.h"
#include "hazardmanager.h"

//classes with important handling functions:
#include "collisionhandler.h"
#include "resetthings.h"
#include "priorityhandler.h"
#include "colormixer.h"
#include "powerfunctionhelper.h"
#include "endgamehandler.h"

//levels:
#include "defaultrandomlevel.h"
#include "emptylevel.h"
#include "corridorlevel.h"
#include "bigfunlevel.h"
#include "fewobstacleslevel.h"
#include "concealedpowerupslevel.h"
#include "manyhazardslevel.h"
//"extra" (is there even a reason for this?):
#include "hidingplaceslevel.h"
#include "sneakyrewardlevel.h"
#include "lightningcornerslevel.h"
#include "loneturretlevel.h"
//"extra"... special
#include "trickymaneuveringlevel.h"
#include "mineheavenlevel.h"
//old levels:
#include "oldemptylevel.h"
//dev levels:
#include "developerlevel0.h"
#include "developerlevel1.h"
#include "developerlevel2.h"

//level effects:
#include "windleveleffect.h"
#include "respawningpowerupsleveleffect.h"
#include "minefieldleveleffect.h"
#include "iceleveleffect.h"

//hazards:
#include "stationaryturret.h"
#include "targetingturret.h"
#include "rectangularlightning.h"
#include "horizontallightning.h"
#include "verticallightning.h"
#include "circularlightning.h"
#include "rectangularlava.h"
#include "circularlava.h"
#include "rectangularnobulletzone.h"
#include "circularnobulletzone.h"

//powers:
#include "speedpower.h"
#include "wallhackpower.h"
#include "multishotpower.h"
#include "bouncepower.h"
#include "triplepower.h"
#include "homingpower.h"
#include "invinciblenamedpower.h"
#include "bignamedpower.h"
#include "megadeathpower.h"
#include "grenadepower.h"
#include "firenamedpower.h"
#include "blastpower.h"
#include "bananapower.h"
#include "godmodepower.h"
//"extra":
#include "minespower.h"
#include "shotgunpower.h"
#include "trackingpower.h"
#include "barrierpower.h"
#include "shieldpower.h"
//old powers:
#include "oldbouncepower.h" //identical to bounce except it makes the tank smaller
#include "oldbignamedpower.h" //big but moves at half speed (not quarter) and the speed stacks
#include "oldminespower.h" //firing rate stacks
//dev powers:
#include "devlonginvinciblenamedpower.h" //invincible but lasts a long time
#include "inversionpower.h" //flips left and right turning
#include "devannoyingpower.h" //bullet doesn't hurt tank, merely pushes away

//a lot of the includes aren't needed anymore thanks to GameMainLoop, but may as well keep them
#include "gamemainloop.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

//#include "diagnostics.h"

//tank team rules:
//0  = no team or default team
//-1 = hazard team (hazards can be split up into multiple teams, but by default there's only one)
//1  = default teamID of tank #1 ("WASD")
//2  = default teamID of tank #2 ("Arrow Keys")
//tank teams always > 0



int main(int argc, char** argv) {
	RNG::Initialize(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	Renderer::SetContext("OpenGL");
	try {
		Renderer::PreInitialize(&argc, argv, "PowerTanks Battle v0.2.4 NOT FINAL"); //this is not guaranteed to be correct every commit but likely will be
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	// Set callback for drawing the scene
	glutDisplayFunc(GameMainLoop::drawEverything);

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

	//vanilla (should some be "old"?):
	PowerupManager::addPowerFactory(SpeedPower::factory);
	PowerupManager::addPowerFactory(WallhackPower::factory);
	PowerupManager::addPowerFactory(BouncePower::factory);
	PowerupManager::addPowerFactory(MultishotPower::factory);
	PowerupManager::addPowerFactory(TriplePower::factory);
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

	//vanilla (some are also "old"):
	LevelManager::addLevelEffectFactory(WindLevelEffect::factory);
	LevelManager::addLevelEffectFactory(RespawningPowerupsLevelEffect::factory);
	LevelManager::addLevelEffectFactory(MinefieldLevelEffect::factory);
	LevelManager::addLevelEffectFactory(IceLevelEffect::factory);

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
	//"extra"... special:
	LevelManager::addLevelFactory(TrickyManeuveringLevel::factory);
	LevelManager::addLevelFactory(MineHeavenLevel::factory);

	//old:
	LevelManager::addLevelFactory(OldEmptyLevel::factory);

	//dev:
	LevelManager::addLevelFactory(DeveloperLevel0::factory);
	LevelManager::addLevelFactory(DeveloperLevel1::factory);
	LevelManager::addLevelFactory(DeveloperLevel2::factory);

	//vanilla:
	HazardManager::addCircleHazardFactory(StationaryTurret::factory, StationaryTurret::randomizingFactory);
	HazardManager::addCircleHazardFactory(TargetingTurret::factory, TargetingTurret::randomizingFactory);
	HazardManager::addRectHazardFactory(RectangularLightning::factory, RectangularLightning::randomizingFactory);
	HazardManager::addRectHazardFactory(HorizontalLightning::factory, HorizontalLightning::randomizingFactory);
	HazardManager::addRectHazardFactory(VerticalLightning::factory, VerticalLightning::randomizingFactory);
	HazardManager::addCircleHazardFactory(CircularLightning::factory, CircularLightning::randomizingFactory);
	HazardManager::addRectHazardFactory(RectangularLava::factory, RectangularLava::randomizingFactory);
	HazardManager::addCircleHazardFactory(CircularLava::factory, CircularLava::randomizingFactory);
	HazardManager::addRectHazardFactory(RectangularNoBulletZone::factory, RectangularNoBulletZone::randomizingFactory);
	HazardManager::addCircleHazardFactory(CircularNoBulletZone::factory, CircularNoBulletZone::randomizingFactory);

	//initialize managers and stuff:
	GameManager::Initialize();
	KeypressManager::Initialize();
	BulletManager::initialize();
	PowerupManager::initialize();
	WallManager::initialize();
	LevelManager::initialize();
	HazardManager::initialize();
	Renderer::Initialize(); //static VAO, VBO, and IBO has better performance
	BackgroundRect::initializeGPU();

	//main game code initialization stuff:
	TankManager::pushTank(new Tank(20, GAME_HEIGHT/2, 0, 1, "WASD", { false, 'w' }, { false, 'a' }, { false, 'd' }, { false, 's' }, { false, 'e' }));
	TankManager::pushTank(new Tank(GAME_WIDTH-20, GAME_HEIGHT/2, PI, 2, "Arrow Keys", { true, GLUT_KEY_UP }, { true, GLUT_KEY_LEFT }, { true, GLUT_KEY_RIGHT }, { true, GLUT_KEY_DOWN }, { false, '/' }));
	EndGameHandler::addTeamToWatch(1, "\"WASD 4 Life\"");
	EndGameHandler::addTeamToWatch(2, "\"Arrow Keys R WINZ\"");
	//these are just the kind of names that will make me cringe in a month
	EndGameHandler::addTeamToWatch(HAZARD_TEAM, "HAZARDS");
#if _DEBUG
	LevelManager::pushLevel("dev", "dev2");
#else
	LevelManager::pushLevel("vanilla", "default_random");
#endif
	ResetThings::firstReset();

	std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl << std::endl;

	//framelimiter
	//glutTimerFunc(1000/physicsRate, tick, physicsRate); //see GameMainLoop
	GameMainLoop::Tick();

	// Start the main loop
	glutMainLoop();
}

/*
 * estimated total completion:
 * 100% required GPU drawing stuff!
 * 20% theoretical GPU stuff (may not attempt)
 * * add a gradient shader
 * * gotta learn how to do batching
 * * make things more efficient (way easier said than done, I suppose)
 * 92.5?% theoretical foundation: no hazard powers, no level... anything
 * * hazard powers: ~50%
 * * levels: ~75%; no level effects
 * 90% actual foundation: not every "modification function" actually does something
 * * almost everything needed is somewhere, so copy-and-paste will be the solution (unless I can figure out member function pointers)
 * ~40% game code:
 * * I don't know what will be final beyond the ideas located in power.cpp, hazard.cpp, level.cpp, and elsewhere
 * * it's just an estimate
 * * 100% probably won't be "finished" on this scale (it could jump from 60% to 100%)
 */
