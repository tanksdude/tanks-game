#pragma once
#include <string>
#include "abstractrenderableobject.h"

enum class RenderingObjectDrawTypes {
	triangles, //main
	line_loop,
	line_strip,
	lines,
	points //unused
};

enum class RenderingObjectDrawingHints {
	static_draw, //useless until I figure out framebuffers
	dynamic_draw, //default
	stream_draw
};

class RenderingObject {
	//this is the one thing that maybe should be "final", but it doesn't really need to be
	friend class Renderer;

private:
	AbstractRenderableObject* thingToBeDrawn;
	RenderingObjectDrawTypes drawingType;

public:
	RenderingObject(const void* positions, unsigned int size, RenderingObjectDrawingHints hint, const unsigned int* indices, unsigned int count);
	~RenderingObject();
};
