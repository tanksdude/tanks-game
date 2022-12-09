#pragma once
#include "game-scene.h"
#include "drawable-thing.h"

class GameMainLoop : public GameScene {
	friend class DeveloperManager;
	friend class GameManager; //needed?

protected:
	//bool currentlyDrawing; //look into std::mutex
	//long frameCount; //doesn't need a long for how it's interpreted...
	//long ticksUntilFrame; //whatever again
	int physicsRate; //(in Hz)
	int waitCount;
	int maxWaitCount;

public:
	GameMainLoop();
	void Tick() { Tick(physicsRate); }
	void Tick(int UPS) override;
	void Draw() const override { drawMain(); }

	//tick stuff:
	void levelTick();
	void moveTanks();
	void tankToPowerup();
	void tickHazards();
	void moveBullets();
	void tankShoot();
	void tankPowerCalculate();
	void bulletPowerCalculate();
	void tankToWall();
	void tankToHazard();
	void tankToTank();
	void tankToEdge();
	void bulletToEdge();
	void bulletToWall();
	void bulletToHazard();
	void bulletToBullet();
	void bulletToTank();

	void drawMain() const; //doesn't draw all layers (since not everything uses everything)
	void drawAllLayers() const;
	void drawLayer(DrawingLayers) const;

private:
	GameMainLoop(const GameMainLoop&) {}
};
