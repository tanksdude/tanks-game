#include "vertex-array.h"
#include "renderer.h"
#include <stdexcept>
#include "opengl-vertex-array.h"

VertexArray* VertexArray::MakeVertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	AvailableRenderingContexts API = Renderer::GetContext();
	switch (API) {
		case AvailableRenderingContexts::OpenGL:
			return new OpenGLVertexArray(vb, layout);
		case AvailableRenderingContexts::software:
			//return new SoftwareVertexArray(vb, layout);
		case AvailableRenderingContexts::null_rendering:
			//return new NullVertexArray(vb, layout);
		default:
			throw std::invalid_argument("ERROR: Unknown rendering API!");
	}
}
