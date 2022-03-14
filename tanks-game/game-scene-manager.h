#pragma once
#include "game-scene.h"
#include <vector>
#include <utility>

typedef int Scene_ID;

class GameSceneManager {
private:
	static std::vector<std::pair<GameScene*, Scene_ID>> scenes;
	//static void clearScenes();
	static Scene_ID nextSceneID;

public:
	static void Initialize();
	//TODO
	static GameScene* getScene(int index);
	static GameScene* getSceneByID(Scene_ID);
	static void pushScene(GameScene*);
	static int getNumScenes() { return scenes.size(); }
	static void deleteScene(int index);
	static void deleteSceneByID(Scene_ID);

	static void TickScenes(int UPS);
	static void DrawScenes(int UPS) { DrawScenes(); }
	static void DrawScenes();

private:
	GameSceneManager() {}
	GameSceneManager(const GameSceneManager&) {}
};
