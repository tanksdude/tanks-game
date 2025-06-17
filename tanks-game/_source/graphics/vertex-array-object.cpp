#include "vertex-array-object.h"

#include <stdexcept>

#include "../renderer.h"
#include "opengl-vertex-array-object.h"

VertexArrayObject* VertexArrayObject::MakeVertexArrayObject() {
	AvailableRenderingContexts API = Renderer::GetContext();
	switch (API) {
		case AvailableRenderingContexts::OpenGL:
			return new OpenGLVertexArrayObject();
		case AvailableRenderingContexts::software:
			//return new SoftwareVertexArrayObject();
		case AvailableRenderingContexts::null_rendering:
			//return new NullVertexArrayObject();
		default:
			throw std::invalid_argument("ERROR: Unknown rendering API!");
	}
}
