#pragma once
#include "../level.h"

//does not have the portal level effect
class OldPortalLevel : public Level {
public:
	virtual std::string getName() const override { return "old_portal"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0xAA/255.0, 0x22/255.0, 1.0f); }
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old" };
		//no random-old
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	OldPortalLevel();
	virtual ~OldPortalLevel() { return; }
	static Level* factory();
};
