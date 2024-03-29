#include "opengl-vertex-buffer.h"

#include <stdexcept>

#include <GL/glew.h>

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, unsigned int size) : OpenGLVertexBuffer(data, size, RenderingHints::stream_draw) {
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
	//layout = VertexBufferLayout({ { ShaderDataType::None, "" } });
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

GLenum OpenGLVertexBuffer::ShaderDataTypeToGLenum(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float:  return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat4:   return GL_FLOAT;
		case ShaderDataType::Int:    return GL_INT;
		case ShaderDataType::Int2:   return GL_INT;
		case ShaderDataType::Int3:   return GL_INT;
		case ShaderDataType::Int4:   return GL_INT;
	}
	return 0;
}
