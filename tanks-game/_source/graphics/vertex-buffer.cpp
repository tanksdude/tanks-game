#include "vertex-buffer.h"

#include <stdexcept>

#include "../renderer.h"
#include "opengl-vertex-buffer.h"

VertexBuffer* VertexBuffer::MakeVertexBuffer(const void* data, unsigned int size, RenderingHints hint) {
	AvailableRenderingContexts API = Renderer::GetContext();
	switch (API) {
		case AvailableRenderingContexts::OpenGL:
			return new OpenGLVertexBuffer(data, size);
		case AvailableRenderingContexts::software:
			//return new SoftwareVertexBuffer(data, size);
		case AvailableRenderingContexts::null_rendering:
			//return new NullVertexBuffer(data, size);
		default:
			throw std::invalid_argument("ERROR: Unknown rendering API!");
	}
}
