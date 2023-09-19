#pragma once

enum class DrawingLayers {
	under,
	normal,
	effects,
	top, //tank death
	debug
};

class DrawableThing {
	friend class Renderer;

public:
	virtual void draw() const = 0; //draw normally
	virtual void draw(DrawingLayers) const = 0;
	virtual void poseDraw() const = 0; //"pose for the camera"
	virtual void poseDraw(DrawingLayers) const = 0;
	virtual void ghostDraw(float alpha) const = 0; //draw something not quite ready for this world
	virtual void ghostDraw(DrawingLayers, float alpha) const = 0;
};
