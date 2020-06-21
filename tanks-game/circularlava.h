#pragma once
#include "circlehazard.h"
#include "generalizedlava.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class CircularLava : public CircleHazard, public GeneralizedLava {
protected:
	virtual void pushNewBubble(double radius);

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
	
	virtual bool actuallyCollided(Bullet* b) override { return (b->velocity == 0); }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) override { return; }

	//virtual bool validLocation() override { return true; }
	virtual bool reasonableLocation() override;

	virtual std::string getName() { return getClassName(); }
	static std::string getClassName() { return "circular lava"; }

	virtual void tick();
	virtual void draw();
	virtual void drawCPU();

	CircularLava(double xpos, double ypos, double radius);
	~CircularLava();
	static CircleHazard* factory(int, std::string*);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() override { return 3; }
	virtual CircleHazardConstructionTypes getConstructionType() override { return CircleHazardConstructionTypes::radiusRequired; }
	virtual CircleFactoryInformation getFactoryInformation() override { return { false, false, false, false, false }; }
};
