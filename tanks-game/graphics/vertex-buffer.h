#pragma once
#include "vertex-buffer-layout.h"

enum class RenderingHints {
	static_draw, //useless until I figure out framebuffers
	dynamic_draw, //default
	stream_draw
};

class VertexBuffer {
public:
	virtual void modifyData(const void* data, int offset, unsigned int size) = 0;
	virtual inline void modifyData(const void* data, unsigned int size) { modifyData(data, 0, size); }

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual const VertexBufferLayout& GetLayout() const = 0;
	virtual void SetLayout(const VertexBufferLayout&) = 0;

	static inline VertexBuffer* MakeVertexBuffer(const void* data, unsigned int size);
	static VertexBuffer* MakeVertexBuffer(const void* data, unsigned int size, RenderingHints hint);
	virtual ~VertexBuffer() { return; }
};