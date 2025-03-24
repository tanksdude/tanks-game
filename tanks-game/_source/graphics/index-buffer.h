#pragma once

class IndexBuffer {
public:
	virtual void modifyData(const void* data, int offset, unsigned int size) = 0;
	inline void modifyData(const void* data, unsigned int size) { modifyData(data, 0, size); }

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual unsigned int getCount() const = 0;
	virtual unsigned int getRendererID() const = 0;

	static IndexBuffer* MakeIndexBuffer(const unsigned int* data, unsigned int count);
	virtual ~IndexBuffer() { return; }
};
