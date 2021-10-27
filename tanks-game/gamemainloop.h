#pragma once

class GameMainLoop {
	friend class DeveloperManager;
	friend class GameManager; //needed?
private:
	static bool currentlyDrawing;
	static long frameCount;
	static long ticksUntilFrame;
	static int physicsRate;
	static void Tick(int physicsUPS);
public:
	//tick stuff:
	static void Tick() { Tick(physicsRate); }
	static void levelTick();
	static void moveTanks();
	static void tankToPowerup();
	static void tickHazards();
	static void moveBullets();
	static void tankPowerCalculate();
	static void bulletPowerCalculate();
	static void tankShoot();
	static void tankToWall();
	static void tankToHazard();
	static void tankToTank();
	static void tankToEdge();
	static void bulletToEdge();
	static void bulletToWall();
	static void bulletToHazard();
	static void bulletToBullet();
	static void bulletToTank();

	static void drawEverything();

private:
	GameMainLoop() {}
	GameMainLoop(const GameMainLoop&) {}
};
