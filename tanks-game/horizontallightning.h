#pragma once
#include "rectangularlightning.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class HorizontalLightning : public RectangularLightning {
	//just called Lightning in JS Tanks
protected:
	//bool flexible; //worry about later

	Circle* getLeftPoint(); //for checks when a bullet/tank collides
	Circle* getRightPoint();

	//unsigned int maxBolts; // = 2;
	virtual void refreshBolt(int num) override; //uses RectangularLightning::refreshBolt
	virtual void simpleRefreshBolt(int num); //fast path for refreshing a bolt that goes from beginning to end
	//virtual int getDefaultNumBoltPoints(double horzDist);
	virtual void pushBolt(LightningBolt* l) override { pushBolt(l, false); }
	virtual void pushBolt(LightningBolt*, bool simpleRefresh);
	virtual void pushDefaultBolt(int num, bool randomize) override;

private:
	static VertexArray* background_va;
	static VertexBuffer* background_vb;
	static IndexBuffer* background_ib;
	VertexArray* bolt_va;
	VertexBuffer* bolt_vb;
	//the bolt is just lines so only the length is needed when drawing (meaning no IndexBuffer needed)
	int bolt_vb_length;
	static bool initialized_GPU;
	void local_reinitializeGPU(int length);
	void streamBoltVertices(unsigned int boltNum) const;

	static bool initializeGPU();
	void local_initializeGPU();
	static bool uninitializeGPU();
	void local_uninitializeGPU();

protected:
	virtual void specialEffectCircleCollision(Circle*) override;

public:
	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "horizontal_lightning"; }

	virtual bool validLocation() override;
	virtual bool reasonableLocation() override;

	//virtual void tick() override;
	virtual void draw() const override;
	virtual void draw(double xpos, double ypos) const override;
	virtual void poseDraw() const override;

	HorizontalLightning(double xpos, double ypos, double width, double height);
	//HorizontalLightning(double xpos, double ypos, double width, double height, bool flexible);
	~HorizontalLightning();
	static RectHazard* factory(int, std::string*);
	static RectHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() const override { return 4; }
	virtual RectHazardConstructionTypes getConstructionType() const override { return RectHazardConstructionTypes::standardConstruction; }
	virtual RectFactoryInformation getFactoryInformation() const override { return { true, true, false, false, true }; }
};
