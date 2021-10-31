#pragma once
#include "recthazard.h"
#include "generalizednobulletzone.h"
#include "constants.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class RectangularNoBulletZone : public RectHazard, public GeneralizedNoBulletZone {
	//called NoShotZone in JS Tanks (because bullets were called "shots")
private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static bool initialized_GPU;

	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	virtual bool actuallyCollided(const Tank*) const override { return false; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) override { return; }

	virtual bool actuallyCollided(const Bullet*) const override { return true; }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) override { return; } //TODO: should the bullet be destroyed here?

public:
	//virtual bool validLocation() const override { return true; }
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "no_bullet_zone"; }

	virtual double getDefaultOffense() const override { return HIGH_TIER; }
	virtual double getDefaultDefense() const override { return HIGH_TIER; }
	virtual RectHazardCollisionType getCollisionType() const override { return RectHazardCollisionType::under; } //not too sure

	virtual void tick() override { GeneralizedNoBulletZone::tick(); }
	virtual void draw() const override;
	virtual void draw(double xpos, double ypos) const override;
	virtual void poseDraw() const override;

	RectangularNoBulletZone(double xpos, double ypos, double width, double height);
	~RectangularNoBulletZone();
	static RectHazard* factory(int, std::string*);
	static RectHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() const override { return 4; }
	virtual RectHazardConstructionTypes getConstructionType() const override { return RectHazardConstructionTypes::standardConstruction; }
	virtual RectFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
