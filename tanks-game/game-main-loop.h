#pragma once
#include "drawable-thing.h"

class GameMainLoop {
	friend class DeveloperManager;
	friend class GameManager; //needed?

private:
	static bool currentlyDrawing;
	static long frameCount;
	static long ticksUntilFrame;
	static int physicsRate;
	static void Tick(int physicsUPS);
	static void drawLayer(DrawingLayers);

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

	static void drawMain(); //doesn't draw all layers (since not everything uses everything)
	static void drawAllLayers();

private:
	GameMainLoop() {}
	GameMainLoop(const GameMainLoop&) {}
};
