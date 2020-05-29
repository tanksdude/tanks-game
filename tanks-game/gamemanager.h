#pragma once

class GameManager {
	friend class DeveloperManager;
private:
	static long nextID;
public:
	static long getNextID();
	static void initialize();
};
