#pragma once

class IndexBuffer {
private:
	unsigned int rendererID;
	unsigned int count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	int getCount() const { return count; }
	unsigned int getRendererID() const { return rendererID; }
};
