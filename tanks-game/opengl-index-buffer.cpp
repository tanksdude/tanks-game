#include "opengl-index-buffer.h"
#include <GL/glew.h>
#include <stdexcept>

OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int* data, unsigned int count) {
	this->count = count;

	glGenBuffers(1, &rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STREAM_DRAW);
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
