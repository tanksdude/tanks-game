#include "opengl-vertex-array.h"

#include <stdexcept>

#include <GL/glew.h>
#include "opengl-vertex-buffer.h"
#include "opengl-index-buffer.h"

OpenGLVertexArray::OpenGLVertexArray() {
	glGenVertexArrays(1, &rendererID);
	indexBuffer = nullptr;
	vertexBufferIndex = 0;
}

OpenGLVertexArray::~OpenGLVertexArray() {
	glDeleteVertexArrays(1, &rendererID);
}

void OpenGLVertexArray::Bind() const {
	glBindVertexArray(rendererID);
}

void OpenGLVertexArray::Unbind() const {
	glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const VertexBuffer* vb) {
	glBindVertexArray(rendererID);
	vb->Bind();
	vertexBuffers.push_back(vb);

	const VertexBufferLayout& layout = vb->GetLayout();
	for (const VertexBufferElement& element : layout.getElements()) {
		switch (element.type) {
			default:
				throw std::domain_error("ERROR: Unknown VBO element type!");

			case ShaderDataType::Float:  [[fallthrough]];
			case ShaderDataType::Float2: [[fallthrough]];
			case ShaderDataType::Float3: [[fallthrough]];
			case ShaderDataType::Float4:
				glEnableVertexAttribArray(vertexBufferIndex);
				glVertexAttribPointer(vertexBufferIndex,
					element.getComponentCount(),
					OpenGLVertexBuffer::ShaderDataTypeToGLenum(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(),
					(const void*) element.offset);
				if (element.instanced) [[unlikely]] {
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
					OpenGLVertexBuffer::ShaderDataTypeToGLenum(element.type),
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
						OpenGLVertexBuffer::ShaderDataTypeToGLenum(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.getStride(),
						(const void*) (element.offset + i * count * sizeof(float)));
					glVertexAttribDivisor(vertexBufferIndex, 1); //matrices should only be instanced
					vertexBufferIndex++;
				}
				} break;
		}
	}
}

void OpenGLVertexArray::SetIndexBuffer(const IndexBuffer* ib) {
	glBindVertexArray(rendererID);
	ib->Bind();
	indexBuffer = ib;
}
