#pragma once
#include "hazard.h"
#include "rect.h"
#include <string>

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class RectHazard : public Hazard, public Rect {
protected: //this may be wrong to include, but I'll figure that out soon
	//static VertexArray* va;
	//static VertexBuffer* vb;
	//static IndexBuffer* ib;
public:
	//virtual void initializeGPU() = 0;
	virtual void draw() = 0;
	virtual std::string getName() = 0;
	static std::string getClassName();

	static RectHazard* factory(int argc, std::string* argv); //strings so any data type can be used (theoretically; structs can't, ya know)
};