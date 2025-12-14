#include "index-buffer.h"

#include <stdexcept>

#include "../renderer.h"
#include "opengl-index-buffer.h"

IndexBuffer* IndexBuffer::MakeIndexBuffer(const unsigned int* data, unsigned int count, RenderingHints hint) {
	AvailableRenderingContexts API = Renderer::GetContext();
	switch (API) {
		case AvailableRenderingContexts::OpenGL:
			return new OpenGLIndexBuffer(data, count, hint);
		case AvailableRenderingContexts::software:
			//return new SoftwareIndexBuffer(data, count, hint);
		case AvailableRenderingContexts::null_rendering:
			//return new NullIndexBuffer(data, count, hint);
		default:
			throw std::invalid_argument("ERROR: Unknown rendering API!");
	}
}
