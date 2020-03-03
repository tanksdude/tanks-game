#pragma once
#include "hazard.h"
#include "circle.h"
#include <string>

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class CircleHazard : public Hazard, public Circle {
protected:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
public:
	static void initializeGPU();
	virtual void draw() = 0;
	virtual std::string getName() = 0;
	static std::string getClassName();

	static CircleHazard* factory(int argc, std::string* argv); //strings so any data type can be used (theoretically; structs can't, ya know)
};