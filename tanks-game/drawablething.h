#pragma once

class DrawableThing {
	friend class Renderer;

public:
	virtual void draw() const = 0;
	virtual void draw(double xpos, double ypos) const = 0;

	//do some sort of "pose for the camera" draw that doesn't depend on internal values (except size stuff); might need it for level thumbnails
	virtual void poseDraw() const = 0;
};
