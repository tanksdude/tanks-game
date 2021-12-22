#pragma once
#include "level.h"

class DevNoWallsLevel1 : public Level {
	//not level 0 since it's closer to a real level than just testing
public:
	virtual std::string getName() const override { return "nowalls1"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	DevNoWallsLevel1();
	virtual ~DevNoWallsLevel1() { return; }
	static Level* factory();
};
