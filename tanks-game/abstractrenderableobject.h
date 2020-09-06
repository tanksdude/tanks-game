#pragma once

class AbstractRenderableObject {
	friend class RenderingContext;
public:
	AbstractRenderableObject(const void* positions, unsigned int size, RenderingObjectDrawingHints hint, const unsigned int* indices, unsigned int count);
	virtual ~AbstractRenderableObject();
};
