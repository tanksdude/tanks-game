#pragma once
#include "rect-hazard.h"
#include "generalized-lava.h"
#include "constants.h"

#include "vertex-array.h"
#include "vertex-buffer.h"
#include "index-buffer.h"

class RectangularLavaHazard : public RectHazard, public GeneralizedLava {
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
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual RectHazardCollisionType getCollisionType() const override { return RectHazardCollisionType::under; }

	//virtual bool actuallyCollided(const Tank*) const override { return true; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) override { return; }

	virtual bool actuallyCollided(const Bullet* b) const override { return (b->velocity.getMagnitude() == 0); }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) override { return; }

protected:
	virtual double getDefaultOffense() const override { return .5; }
	virtual double getDefaultDefense() const override { return HIGH_TIER; }

public:
	//virtual bool validLocation() const override { return true; }
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "lava"; }

	virtual void tick() override { GeneralizedLava::tick(); }
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

private:
	inline void drawBackground(bool pose, float alpha = 1.0f) const;
	inline void drawBubbles(bool pose, float alpha = 1.0f) const;

public:
	RectangularLavaHazard(double xpos, double ypos, double width, double height);
	~RectangularLavaHazard();
	static RectHazard* factory(int, std::string*);
	static RectHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() const override { return 4; }
	virtual RectHazardConstructionTypes getConstructionType() const override { return RectHazardConstructionTypes::standardConstruction; }
	virtual RectFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};