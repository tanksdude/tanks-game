#pragma once
class Wall;

#include "rect.h"
#include "colorvalueholder.h"
#include "resetthings.h"
#include "powerfunctionhelper.h"
#include "renderer.h"

class Wall : public Rect{
	friend class ResetThings; //not really needed
	friend class PowerFunctionHelper;
	ColorValueHolder color;
	short id = -1;

	//walls don't all have the same shape, so they all store different GPU stuff
	//technically they could have the same vertices then just scale it but that can happen later
	//TODO: that ^
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
	void CPUdraw();
};