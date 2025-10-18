#include "opengl-vertex-array-object.h"

#include <stdexcept>

#include <GL/glew.h>

//"static" here means to not allow this function outside of this file
static GLenum ShaderDataTypeToGLenum(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::UByte:  return GL_UNSIGNED_BYTE;
		case ShaderDataType::UByte2: return GL_UNSIGNED_BYTE;
		case ShaderDataType::UByte3: return GL_UNSIGNED_BYTE;
		case ShaderDataType::UByte4: return GL_UNSIGNED_BYTE;
		case ShaderDataType::Int:    return GL_INT;
		case ShaderDataType::Int2:   return GL_INT;
		case ShaderDataType::Int3:   return GL_INT;
		case ShaderDataType::Int4:   return GL_INT;
		case ShaderDataType::Float:  return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat4:   return GL_FLOAT;
	}
	return 0;
}

OpenGLVertexArrayObject::OpenGLVertexArrayObject() {
	glGenVertexArrays(1, &rendererID);
	indexBuffer = nullptr;
	vertexBufferIndex = 0;
}

OpenGLVertexArrayObject::~OpenGLVertexArrayObject() {
	glDeleteVertexArrays(1, &rendererID);
}

void OpenGLVertexArrayObject::Bind() const {
	glBindVertexArray(rendererID);
}

void OpenGLVertexArrayObject::Unbind() const {
	glBindVertexArray(0);
}

void OpenGLVertexArrayObject::AddVertexBuffer(const VertexBuffer* vb) {
	glBindVertexArray(rendererID);
	vb->Bind();
	vertexBuffers.push_back(vb);

	const VertexBufferLayout& layout = vb->GetLayout();
	for (const VertexBufferElement& element : layout.getElements()) {
		switch (element.type) {
			default:
				throw std::domain_error("ERROR: Unknown VB element type!");

			case ShaderDataType::UByte:  [[fallthrough]];
			case ShaderDataType::UByte2: [[fallthrough]];
			case ShaderDataType::UByte3: [[fallthrough]];
			case ShaderDataType::UByte4:
				//assume that normalized values are for a vecn (float) input
				glEnableVertexAttribArray(vertexBufferIndex);
				if (element.normalized) [[likely]] {
					glVertexAttribPointer(vertexBufferIndex,
						element.getComponentCount(),
						ShaderDataTypeToGLenum(element.type),
						GL_TRUE,
						layout.getStride(),
						(const void*) element.offset);
				} else {
					glVertexAttribIPointer(vertexBufferIndex,
						element.getComponentCount(),
						ShaderDataTypeToGLenum(element.type),
						layout.getStride(),
						(const void*) element.offset);
				}
				if (element.instanced) {
					glVertexAttribDivisor(vertexBufferIndex, 1);
				}
				vertexBufferIndex++;
				break;

			case ShaderDataType::Int:  [[fallthrough]];
			case ShaderDataType::Int2: [[fallthrough]];
			case ShaderDataType::Int3: [[fallthrough]];
			case ShaderDataType::Int4:
				//glVertexAttribPointer() and glVertexAttribIPointer() are different functions; thank you, OpenGL, for your intuitive API
				glEnableVertexAttribArray(vertexBufferIndex);
				glVertexAttribIPointer(vertexBufferIndex,
					element.getComponentCount(),
					ShaderDataTypeToGLenum(element.type),
					layout.getStride(),
					(const void*) element.offset);
				if (element.instanced) [[unlikely]] {
					glVertexAttribDivisor(vertexBufferIndex, 1);
				}
				vertexBufferIndex++;
				break;

			case ShaderDataType::Float:  [[fallthrough]];
			case ShaderDataType::Float2: [[fallthrough]];
			case ShaderDataType::Float3: [[fallthrough]];
			case ShaderDataType::Float4:
				glEnableVertexAttribArray(vertexBufferIndex);
				glVertexAttribPointer(vertexBufferIndex,
					element.getComponentCount(),
					ShaderDataTypeToGLenum(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(),
					(const void*) element.offset);
				if (element.instanced) [[unlikely]] {
					glVertexAttribDivisor(vertexBufferIndex, 1);
				}
				vertexBufferIndex++;
				break;

			case ShaderDataType::Mat4: {
				//it's not possible to directly set a mat4 as an attribute, because the limit is a vec4
				unsigned int count = element.getComponentCount();
				for (unsigned int i = 0; i < count; i++) {
					glEnableVertexAttribArray(vertexBufferIndex);
					glVertexAttribPointer(vertexBufferIndex,
						count,
						ShaderDataTypeToGLenum(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.getStride(),
						(const void*) (element.offset + i * count * sizeof(float)));
					if (true) { //matrices should only be instanced
						glVertexAttribDivisor(vertexBufferIndex, 1);
					}
					vertexBufferIndex++;
				}
				} break;
		}
	}
}

void OpenGLVertexArrayObject::SetIndexBuffer(const IndexBuffer* ib) {
	glBindVertexArray(rendererID);
	ib->Bind();
	indexBuffer = ib;
}
