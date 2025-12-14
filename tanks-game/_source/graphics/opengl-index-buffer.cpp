#include "opengl-index-buffer.h"

#include <stdexcept>

#include <glad/gl.h>

OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int* data, unsigned int count, RenderingHints hint) {
	this->count = count;

	glGenBuffers(1, &rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	switch (hint) {
		case RenderingHints::stream_draw:
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STREAM_DRAW);
			break;
		case RenderingHints::static_draw:
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
			break;
		case RenderingHints::dynamic_draw:
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW);
			break;
		default:
			throw std::invalid_argument("ERROR: Unknown rendering hint!");
	}
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
	glDeleteBuffers(1, &rendererID);
}

void OpenGLIndexBuffer::modifyData(const void* data, int offset, unsigned int size) {
	this->count = size / sizeof(unsigned int);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

void OpenGLIndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void OpenGLIndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
