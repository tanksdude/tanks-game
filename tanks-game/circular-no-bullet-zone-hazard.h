#pragma once
#include "circle-hazard.h"
#include "generalized-no-bullet-zone.h"
#include "constants.h"

#include "vertex-array.h"
#include "vertex-buffer.h"
#include "index-buffer.h"

class CircularNoBulletZoneHazard : public CircleHazard, public GeneralizedNoBulletZone {
private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static bool initialized_GPU;

	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		//it's not really an old hazard (no bullet zone was rectangular only)
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual CircleHazardCollisionType getCollisionType() const override { return CircleHazardCollisionType::under; } //not too sure

	virtual bool actuallyCollided(const Tank*) const override { return false; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) override { return; }

	virtual bool actuallyCollided(const Bullet*) const override { return true; }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) override { return; } //TODO: should the bullet be destroyed here?

protected:
	virtual double getDefaultOffense() const override { return HIGH_TIER; }
	virtual double getDefaultDefense() const override { return HIGH_TIER; }

public:
	//virtual bool validLocation() const override { return true; }
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "no_bullet_zone"; }

	virtual void tick() override { GeneralizedNoBulletZone::tick(); }
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

	CircularNoBulletZoneHazard(double xpos, double ypos, double radius);
	~CircularNoBulletZoneHazard();
	static CircleHazard* factory(int, std::string*);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() const override { return 3; }
	virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::radiusRequired; }
	virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
