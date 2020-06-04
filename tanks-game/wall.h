#pragma once
class Wall;

#include "gamething.h"
#include "rect.h"
#include "colorvalueholder.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class Wall : public Rect, public GameThing {
	friend class ResetThings; //not really needed
	friend class PowerFunctionHelper;
protected:
	ColorValueHolder color;

private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static bool initialized_GPU;
public:
	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	Wall(double x, double y, double w, double h, ColorValueHolder);
	Wall(double x, double y, double w, double h, ColorValueHolder c, char teamID);

	void draw();
	void drawCPU();

	~Wall();
};
