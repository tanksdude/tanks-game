#pragma once
#include "../circle-hazard.h"

#include "../constants.h"
#include "../power-square.h"

class CloudHazard : public CircleHazard {
protected:
	ColorValueHolder color;
	std::vector<std::vector<std::string>> powerChoices;
	double initialSpeed;
	double acceleration;
	double tickCount;
	double tickCycle;

	std::vector<Game_ID> powerupsToMove;
	std::vector<SimpleVector2D> powerupsVelocity;

protected:
	virtual PowerSquare* makePowerup(int powerupIndex) const;
	void pushPowerup();

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual CircleHazardCollisionType getCollisionType() const override { return CircleHazardCollisionType::under; } //if anything, "over"

	virtual bool actuallyCollided(const Tank*) const override { return false; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) override { return; }

	virtual bool actuallyCollided(const Bullet*) const override { return false; }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) override { return; }

protected:
	virtual float getDefaultOffense() const override { return 0; }
	virtual float getDefaultDefense() const override { return HIGH_TIER; }

public:
	//virtual bool validLocation() const override { return true; } //maybe change
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "cloud"; }

	virtual void tick() override;
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

protected:
	static SimpleVector2D body_vertices[Circle::NumOfSides+1];
	static unsigned int body_indices[Circle::NumOfSides*3];
	static bool initialized_vertices;

	static bool initializeVertices();

public:
	CloudHazard(double xpos, double ypos, double radius); //only speed power
	CloudHazard(double xpos, double ypos, double radius, double initialSpeed, double acceleration); //only speed power
	CloudHazard(double xpos, double ypos, double radius, double initialSpeed, double acceleration, const std::vector<std::vector<std::string>>& powerList);
	~CloudHazard();
	static CircleHazard* factory(const GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	virtual int getFactoryArgumentCount() const override { return 3; }
	virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::constructionIsTooComplex; }
	virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
