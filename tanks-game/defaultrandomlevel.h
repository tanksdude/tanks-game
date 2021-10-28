#pragma once
#include "level.h"

class DefaultRandomLevel : public Level {
protected:
	ColorValueHolder currentColor;

public:
	virtual std::string getName() const override { return "default_random"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual void initialize() override;

	DefaultRandomLevel();
	virtual ~DefaultRandomLevel() { return; }
	static Level* factory();
};
