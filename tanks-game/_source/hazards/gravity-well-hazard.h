#pragma once
#include "../circle-hazard.h"

#include "../constants.h"

class GravityWellHazard : public CircleHazard {
protected:
	ColorValueHolder color;
	double gravityRange;
	double minGravityStrength;
	double maxGravityStrength;
	double tickCount;
	double tickCycle; //only for the "gravity circle"

protected:
	inline double getGravityStrength(double dist) const;
	virtual inline double getInnerGravityCircleRadius() const;
	[[nodiscard]] inline Circle* getGravityRangeCircle() const;

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual CircleHazardCollisionType getCollisionType() const override { return CircleHazardCollisionType::solid; }

protected:
	virtual float getDefaultOffense() const override { return 0; }
	virtual float getDefaultDefense() const override { return DESTRUCTION_TIER; }

public:
	//virtual bool validLocation() const override { return true; } //maybe change
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "gravity_well"; }

	virtual void tick() override;
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

protected:
	virtual inline void drawBody(float alpha = 1.0f) const;
	virtual inline void drawOutline(float alpha = 1.0f) const;
	virtual inline void drawGravityCircle(float alpha = 1.0f) const;
	virtual inline void drawGravityArrows(float alpha = 1.0f) const;

protected:
	static SimpleVector2D body_vertices[Circle::numOfSides+1];
	static unsigned int body_indices[Circle::numOfSides*3];
	static unsigned int outline_indices[Circle::numOfSides*2*3];
	static SimpleVector2D vertices_arrow[7];
	static unsigned int indices_arrow[3*3]; //from WindLevelEffect
	static bool initialized_vertices;

	static bool initializeVertices();

public:
	GravityWellHazard(double xpos, double ypos, double radius);
	GravityWellHazard(double xpos, double ypos, double radius, double gravityRange);
	GravityWellHazard(double xpos, double ypos, double radius, double gravityRange, double minGravity, double maxGravity);
	~GravityWellHazard();
	static CircleHazard* factory(const GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	virtual int getFactoryArgumentCount() const override { return 3; }
	virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::constructionIsTooComplex; }
	virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
