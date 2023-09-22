#include "game-scene-manager.h"

#include "renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

std::vector<std::pair<GameScene*, Scene_ID>> GameSceneManager::scenes;
int GameSceneManager::nextSceneID = 0;

void GameSceneManager::Initialize() {
	//nothing
}

void GameSceneManager::TickScenes(int UPS) {
	for (int i = 0; i < scenes.size(); i++) {
		scenes[i].first->Tick(UPS);
	}
	DrawScenes();
}

void GameSceneManager::DrawScenes() {
	Renderer::BeginningStuff();
	Renderer::Clear();
	for (int i = 0; i < scenes.size(); i++) {
		scenes[i].first->Draw();
	}
	Renderer::Flush();
}

GameScene* GameSceneManager::getScene(unsigned int index) {
	return scenes[index].first;
}

GameScene* GameSceneManager::getSceneByID(Scene_ID sceneID) {
	for (int i = 0; i < scenes.size(); i++) {
		if (scenes[i].second == sceneID) {
			return scenes[i].first;
		}
	}
	return nullptr;
}

void GameSceneManager::pushScene(GameScene* gs) {
	scenes.push_back({ gs, ++nextSceneID });
}

void GameSceneManager::deleteScene(unsigned int index) {
	delete scenes[index].first;
	scenes.erase(scenes.begin() + index);
}

void GameSceneManager::deleteSceneByID(Scene_ID sceneID) {
	for (int i = 0; i < scenes.size(); i++) {
		if (scenes[i].second == sceneID) {
			deleteScene(i);
			break;
		}
	}
}

/*
Scene_ID GameSceneManager::getNextID() {
	nextSceneID++;
	return nextSceneID;
}
*/
