#include "renderingobject.h"

RenderingObject::RenderingObject(const void* positions, unsigned int size, RenderingObjectDrawingHints hint,
                                 const unsigned int* indices, unsigned int count) {
    thingToBeDrawn = new AbstractRenderableObject(positions, size, hint, indices, count);
}

RenderingObject::~RenderingObject() {
    delete thingToBeDrawn;
}
