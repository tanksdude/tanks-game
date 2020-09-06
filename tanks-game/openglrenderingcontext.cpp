#include "openglrenderingcontext.h"

class OpenGLRenderableObject : public AbstractRenderableObject {
	OpenGLRenderableObject(const void* positions, unsigned int size, RenderingObjectDrawingHints hint, const unsigned int* indices, unsigned int count);
	~OpenGLRenderableObject();
};

class OpenGLRenderingContext : public RenderingContext {

};

AbstractRenderableObject* asdf = new OpenGLRenderableObject();
OpenGLRenderableObject* fdsa = (OpenGLRenderableObject*)asdf;