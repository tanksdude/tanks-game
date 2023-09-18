#include "vertex-array.h"

#include <stdexcept>

#include "renderer.h"
#include "opengl-vertex-array.h"

VertexArray* VertexArray::MakeVertexArray() {
	AvailableRenderingContexts API = Renderer::GetContext();
	switch (API) {
		case AvailableRenderingContexts::OpenGL:
			return new OpenGLVertexArray();
		case AvailableRenderingContexts::software:
			//return new SoftwareVertexArray();
		case AvailableRenderingContexts::null_rendering:
			//return new NullVertexArray();
		default:
			throw std::invalid_argument("ERROR: Unknown rendering API!");
	}
}
