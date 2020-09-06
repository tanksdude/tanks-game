#include "vertexbuffer.h"
#include "renderer.h"
#include <stdexcept>
#include "openglvertexbuffer.h"

VertexBuffer* VertexBuffer::MakeVertexBuffer(const void* data, unsigned int size) {
	return MakeVertexBuffer(data, size, RenderingHints::dynamic_draw);
}

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
