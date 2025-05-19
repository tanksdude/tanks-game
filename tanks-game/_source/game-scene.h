#pragma once

class GameScene {
	friend class GameSceneManager; //needed?

protected:
	//Scene_ID sceneID;

public:
	//TODO
	virtual void Tick() = 0;
	virtual void Draw() const = 0;

	virtual ~GameScene() {}

protected:
	GameScene();

private:
	GameScene(const GameScene&) = delete;
};
