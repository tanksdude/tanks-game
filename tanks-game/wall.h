#pragma once
class Wall;

#include "rect.h"
#include "colorvalueholder.h"
#include "resetthings.h"
#include "powerfunctionhelper.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class Wall : public Rect{
	friend class ResetThings; //not really needed
	friend class PowerFunctionHelper;
	ColorValueHolder color;
	short id = -1;

protected:
	//these are for figuring out if the VertexBuffer needs to be updated
	double old_x;
	double old_y;
	double old_w;
	double old_h;

private:
	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib;
public:
	void initializeGPU();

public:
	Wall(double x, double y, double w, double h, ColorValueHolder);
	Wall(double x, double y, double w, double h, ColorValueHolder c, short id);

	short getID() {
		return id;
	}
	void draw();
	void drawCPU();

	~Wall();
};