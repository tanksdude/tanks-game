#include "wall.h"
#include "game-manager.h"
#include "constants.h"
#include "color-mixer.h"
#include "mylib.h"
#include "background-rect.h"
#include "renderer.h"
#include <iostream>

Wall::Wall(double x_, double y_, double w_, double h_, ColorValueHolder c) : GameThing(DEFAULT_TEAM) {
	this->x = x_;
	this->y = y_;
	this->w = w_;
	this->h = h_;
	color = c;
}

//Wall::Wall(double x_, double y_, double w_, double h_, ColorValueHolder c, Team_ID teamID_) : Wall(x_, y_, w_, h_, c) {
//	this->teamID = teamID_;
//}

Wall::~Wall() {
	//nothing
}

void Wall::update(const WallUpdateStruct* up) {
	this->x += up->x;
	this->y += up->y;
	this->w += up->w;
	this->h += up->h;
}

void Wall::draw() const {
	ghostDraw(1.0f);
}

void Wall::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Wall::draw!" << std::endl;
		case DrawingLayers::normal:
			draw();
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void Wall::poseDraw() const {
	draw();
}

void Wall::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Wall::poseDraw!" << std::endl;
		case DrawingLayers::normal:
			poseDraw();
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void Wall::ghostDraw(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder c = this->color;
	c = ColorMixer::mix(BackgroundRect::getBackColor(), c, alpha);

	float coordsAndColor[] = {
		x,   y,     c.getRf(), c.getGf(), c.getBf(), c.getAf(),
		x+w, y,     c.getRf(), c.getGf(), c.getBf(), c.getAf(),
		x+w, y+h,   c.getRf(), c.getGf(), c.getBf(), c.getAf(),
		x,   y+h,   c.getRf(), c.getGf(), c.getBf(), c.getAf()
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	Renderer::SubmitBatchedDraw(coordsAndColor, 4 * (2+4), indices, 2 * 3);
}

void Wall::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for Wall::ghostDraw!" << std::endl;
		case DrawingLayers::normal:
			ghostDraw(alpha);
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

WallUpdateStruct::WallUpdateStruct(double x, double y, double w, double h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void WallUpdateStruct::add(const WallUpdateStruct& other) {
	this->x += other.x;
	this->y += other.y;
	this->w += other.w;
	this->h += other.h;
}
