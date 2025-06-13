#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>

enum class ShaderDataType {
	None,
	Float, Float2, Float3, Float4,
	Mat4,
	Int, Int2, Int3, Int4
};

struct VertexBufferElement {
	std::string name;
	ShaderDataType type;
	unsigned int size;
	unsigned int offset;
	bool normalized;
	bool instanced;

	static unsigned int getSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:  return sizeof(float);
			case ShaderDataType::Float2: return sizeof(float) * 2;
			case ShaderDataType::Float3: return sizeof(float) * 3;
			case ShaderDataType::Float4: return sizeof(float) * 4;
			case ShaderDataType::Mat4:   return sizeof(float) * 4*4;
			case ShaderDataType::Int:    return sizeof(int);
			case ShaderDataType::Int2:   return sizeof(int)   * 2;
			case ShaderDataType::Int3:   return sizeof(int)   * 3;
			case ShaderDataType::Int4:   return sizeof(int)   * 4;
		}
		return 0;
	}

	unsigned int getComponentCount() const {
		switch (this->type) {
			case ShaderDataType::Float:  return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Mat4:   return 4; //4 vec4
			case ShaderDataType::Int:    return 1;
			case ShaderDataType::Int2:   return 2;
			case ShaderDataType::Int3:   return 3;
			case ShaderDataType::Int4:   return 4;
		}
		return 0;
	}

	VertexBufferElement(ShaderDataType type, std::string name, bool normalized = false, bool instanced = false) {
		this->name = name;
		this->type = type;
		this->size = getSize(type);
		this->offset = 0;
		this->normalized = normalized;
		this->instanced = instanced;
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:
	VertexBufferLayout(const std::initializer_list<VertexBufferElement>& e) {
		elements = std::vector<VertexBufferElement>(e);
		stride = 0;

		int cumulativeOffset = 0;
		for (int i = 0; i < elements.size(); i++) {
			elements[i].offset = cumulativeOffset;
			cumulativeOffset += elements[i].size;
			stride += elements[i].size;
		}
	}
	VertexBufferLayout() {
		stride = 0;
	}

	inline const std::vector<VertexBufferElement>& getElements() const { return elements; }
	inline unsigned int getStride() const { return stride; }
};
