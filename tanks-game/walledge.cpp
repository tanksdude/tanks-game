#pragma once
#include "walledge.h"

WallEdge::WallEdge(double x_, double y_, double w_, double h_) {
	this->x = x_;
	this->y = y_;
	this->w = w_;
	this->h = h_;
}

void WallEdge::draw(){return;}
