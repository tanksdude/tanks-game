#pragma once
class Wall;

#include "game-thing.h"
#include "rect.h"
#include "drawable-thing.h"
#include "color-value-holder.h"
#include "wall-update-struct.h"

#include "vertex-array.h"
#include "vertex-buffer.h"
#include "index-buffer.h"

class Wall : public GameThing, public Rect, public DrawableThing {
	friend class ResetThings; //not really needed
	friend class PowerFunctionHelper;

public: //TODO: shouldn't be public
	ColorValueHolder color;

private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static bool initialized_GPU;

	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	void draw() const override;
	void draw(DrawingLayers) const override;
	void poseDraw() const override;
	void poseDraw(DrawingLayers) const override;
	void ghostDraw(float alpha) const override;
	void ghostDraw(DrawingLayers, float alpha) const override;
	//void drawCPU() const;

	void update(const WallUpdateStruct*);

	Wall(double x, double y, double w, double h, ColorValueHolder);
	//Wall(double x, double y, double w, double h, ColorValueHolder c, Team_ID teamID);
	~Wall();
};
