#include "opengl-vertex-buffer.h"
#include <GL/glew.h>
#include <stdexcept>

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, unsigned int size) : OpenGLVertexBuffer(data, size, RenderingHints::dynamic_draw) {
	//nothing
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, unsigned int size, RenderingHints hint) {
	glGenBuffers(1, &rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	switch (hint) {
		case RenderingHints::static_draw:
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			return;
		case RenderingHints::dynamic_draw:
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
			return;
		case RenderingHints::stream_draw:
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
			return;
		default:
			throw std::invalid_argument("ERROR: Unknown rendering hint!");
	}
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
	glDeleteBuffers(1, &rendererID);
}

void OpenGLVertexBuffer::modifyData(const void* data, int offset, unsigned int size) {
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void OpenGLVertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void OpenGLVertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
