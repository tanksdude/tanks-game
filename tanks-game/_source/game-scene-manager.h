#pragma once
#include <utility>
#include <vector>

#include "game-scene.h"

typedef int Scene_ID;

class GameSceneManager {
private:
	static std::vector<std::pair<GameScene*, Scene_ID>> scenes;
	//static void clearScenes();
	static Scene_ID nextSceneID;

public:
	//TODO
	static void Initialize();
	static GameScene* getScene(unsigned int index);
	static GameScene* getSceneByID(Scene_ID);
	static void pushScene(GameScene*);
	static unsigned int getNumScenes() { return scenes.size(); }
	static void deleteScene(unsigned int index);
	static void deleteSceneByID(Scene_ID);

	static void TickScenes(int UPS);
	static void DrawScenes(int UPS) { DrawScenes(); }
	static void DrawScenes();

private:
	GameSceneManager() = delete;
	GameSceneManager(const GameSceneManager&) = delete;
};
