#include "index-buffer.h"
#include "renderer.h"
#include <stdexcept>
#include "opengl-index-buffer.h"

IndexBuffer* IndexBuffer::MakeIndexBuffer(const unsigned int* data, unsigned int count) {
	AvailableRenderingContexts API = Renderer::GetContext();
	switch (API) {
		case AvailableRenderingContexts::OpenGL:
			return new OpenGLIndexBuffer(data, count);
		case AvailableRenderingContexts::software:
			//return new SoftwareIndexBuffer(data, count);
		case AvailableRenderingContexts::null_rendering:
			//return new NullIndexBuffer(data, count);
		default:
			throw std::invalid_argument("ERROR: Unknown rendering API!");
	}
}
