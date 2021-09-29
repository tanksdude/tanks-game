#pragma once
#include "recthazard.h"
#include "generalizedlava.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class RectangularLava : public RectHazard, public GeneralizedLava {
protected:
	virtual void pushNewBubble(double radius) override;

private:
	static VertexArray* background_va;
	static VertexBuffer* background_vb;
	static IndexBuffer* background_ib;
	static VertexArray* bubble_va;
	static VertexBuffer* bubble_vb;
	static IndexBuffer* bubble_ib;
	static bool initialized_GPU;
	
	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	virtual double getDefaultOffense() override { return .5; }
	virtual double getDefaultDefense() override { return 999; }

	//virtual bool actuallyCollided(Tank*) override { return true; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) override { return; }
	
	virtual bool actuallyCollided(Bullet* b) override { return (b->velocity.getMagnitude() == 0); }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) override { return; }

	//virtual bool validLocation() override { return true; }
	virtual bool reasonableLocation() override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "lava"; }

	virtual void tick() { GeneralizedLava::tick(); }
	virtual void draw() const override;
	virtual void draw(double xpos, double ypos) const override;
	virtual void poseDraw() const override;

	RectangularLava(double xpos, double ypos, double width, double height);
	~RectangularLava();
	static RectHazard* factory(int, std::string*);
	static RectHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() const override { return 4; }
	virtual RectHazardConstructionTypes getConstructionType() const override { return RectHazardConstructionTypes::standardConstruction; }
	virtual RectFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
