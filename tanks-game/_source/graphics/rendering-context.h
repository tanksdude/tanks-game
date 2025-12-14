#pragma once

class RenderingContext {
	friend class Renderer;

public:
	virtual void Initialize() = 0;
	virtual void Clear() const = 0;
	virtual void Flush() const = 0;
	virtual void PrintRendererInfo() const = 0;
	virtual void PrintRenderingError() const = 0;

	//names aren't great...
	virtual void DrawWithoutIndices(int vertexCount) const = 0;
	virtual void DrawUsingIndices(int indexCount) const = 0;
	virtual void DrawInstanced(int indexCount, int drawCount) const = 0;

	virtual ~RenderingContext() {}
};
