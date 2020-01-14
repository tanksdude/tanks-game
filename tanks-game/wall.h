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

private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
public:
	static void initializeGPU();

public:
	Wall(double x, double y, double w, double h, ColorValueHolder);
	Wall(double x, double y, double w, double h, ColorValueHolder c, short id);

	short getID() {
		return id;
	}
	void draw();
	void drawCPU();
};