#pragma once
#include "../circle-hazard.h"
#include "../generalized-no-bullet-zone.h"

#include "../constants.h"

class CircularNoBulletZoneHazard : public CircleHazard, public GeneralizedNoBulletZone {
public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		//it's not really an old hazard (no bullet zone was rectangular only)
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual CircleHazardCollisionType getCollisionType() const override { return CircleHazardCollisionType::under; } //not too sure

	virtual bool actuallyCollided(const Tank*) const override { return false; }
	virtual InteractionUpdateHolder<TankUpdateStruct, CircleHazardUpdateStruct> modifiedTankCollision(const Tank*) const override { return { false, false, nullptr, nullptr }; }

	virtual bool actuallyCollided(const Bullet*) const override { return true; }
	virtual InteractionUpdateHolder<BulletUpdateStruct, CircleHazardUpdateStruct> modifiedBulletCollision(const Bullet*) const override { return { false, false, nullptr, nullptr }; } //TODO: should the bullet be destroyed here?

	virtual float getOffenseTier() const override { return HIGH_TIER; }
	virtual float getDefenseTier() const override { return HIGH_TIER; }

public:
	//virtual bool validLocation() const override { return true; }
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "no_bullet_zone"; }

	virtual void tick() override { return; }
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

protected:
	static SimpleVector2D body_vertices[Circle::NumOfSides+1];
	static unsigned int body_indices[Circle::NumOfSides*3];
	static SimpleVector2D* redX_vertices;
	static unsigned int* redX_indices; //these don't get deleted but it's fine
	static int redX_vertices_count;
	static int redX_indices_count;
	static bool initialized_vertices;

	static bool initializeVertices();

public:
	CircularNoBulletZoneHazard(double xpos, double ypos, double radius);
	~CircularNoBulletZoneHazard();
	static CircleHazard* factory(const GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	virtual int getFactoryArgumentCount() const override { return 3; }
	virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::radiusRequired; }
	virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
