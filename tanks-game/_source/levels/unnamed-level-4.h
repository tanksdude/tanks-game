#pragma once
#include "../level.h"

class UnnamedLevel4 : public Level {
public:
	virtual std::string getName() const override { return "unnamed4"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	UnnamedLevel4();
	virtual ~UnnamedLevel4() { return; }
	static Level* factory();
};
