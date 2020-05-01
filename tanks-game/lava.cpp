#pragma once
#include "lava.h"
#include "renderer.h"
#include "backgroundrect.h"
#include "colormixer.h"
#include "constants.h"
#include <math.h>
#include "mylib.h"
#include <iostream>

VertexArray* Lava::background_va;
VertexBuffer* Lava::background_vb;
IndexBuffer* Lava::background_ib;
VertexArray* Lava::bubble_va;
VertexBuffer* Lava::bubble_vb;
IndexBuffer* Lava::bubble_ib;
bool Lava::initialized_GPU = false;

Lava::Lava(double xpos, double ypos, double width, double height) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;

	//tickCount = 0;
	tickCycle = 2400;
	bubbles.reserve(maxBubbles);
	
	canAcceptPowers = false;

	initializeGPU();
}

Lava::~Lava() {
	//uninitializeGPU();
}

bool Lava::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//background:
	float background_positions[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};
	unsigned int background_indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	background_vb = new VertexBuffer(background_positions, 4*2 * sizeof(float), GL_DYNAMIC_DRAW);
	VertexBufferLayout background_layout(2);
	background_va = new VertexArray(*background_vb, background_layout);

	background_ib = new IndexBuffer(background_indices, 6);

	//bubble:
	float bubble_positions[(Circle::numOfSides+1)*2];
	bubble_positions[0] = 0;
	bubble_positions[1] = 0;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		bubble_positions[i*2]   = cos((i-1) * 2*PI / Circle::numOfSides);
		bubble_positions[i*2+1] = sin((i-1) * 2*PI / Circle::numOfSides);
	}

	unsigned int bubble_indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		bubble_indices[i*3]   = 0;
		bubble_indices[i*3+1] = i+1;
		bubble_indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	bubble_vb = new VertexBuffer(bubble_positions, (Circle::numOfSides+1)*2 * sizeof(float), GL_DYNAMIC_DRAW);
	VertexBufferLayout bubble_layout(2);
	bubble_va = new VertexArray(*bubble_vb, bubble_layout);
	
	bubble_ib = new IndexBuffer(bubble_indices, Circle::numOfSides*3);

	initialized_GPU = true;
	return true;
}

bool Lava::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;
	delete bubble_va;
	delete bubble_vb;
	delete bubble_ib;

	initialized_GPU = false;
	return true;
}

RectHazard* Lava::factory(int argc, std::string* argv) {
	if (argc >= 4) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double w = std::stod(argv[2]);
		double h = std::stod(argv[3]);
		return new Lava(x, y, w, h);
	}
	return new Lava(0, 0, 0, 0);
}

void Lava::tick() {
	//tickCount = fmod(tickCount, tickCycle);
	if (++tickCount >= tickCycle) {
		tickCount -= tickCycle;
	}

	if (randFunc() < .125) {
		if (bubbles.size() < maxBubbles) {
			pushNewBubble(4); //possible radius: sqrt(w * h * 2) / 50
		}
	}

	for (int i = bubbles.size()-1; i >= 0; i--) {
		//do stuff
		bubbles[i]->tick();
		if (bubbles[i]->isDead()) {
			delete bubbles[i];
			bubbles.erase(bubbles.begin() + i);
		}
	}
}

void Lava::pushNewBubble(double radius) {
	float x0, y0, x1, y1;
	int attempts = 0;
		
	x0 = randFunc() * (w - radius*2) + radius;
	y0 = randFunc() * (h - radius*2) + radius;
	do {
		x1 = randFunc() * (w - radius*2) + radius;
		y1 = randFunc() * (h - radius*2) + radius;
		attempts++;
	} while((attempts < 8) && (abs(x0-x1) < w/16 || abs(y0-y1) < h/16)); //JS Tanks used w/8 and h/8

	if (attempts < 8) {
		double maxTick = floor(randFunc()*101) + 200;
		bubbles.push_back(new LavaBubble(radius, x0, y0, x1, y1, maxTick));
	}
}

ColorValueHolder Lava::getBackgroundColor() {
	//colors: red (#FF0000) and orange-red (#FFAA00) mixed
	return ColorMixer::mix(ColorValueHolder(1.0f, 0.0f, 0.0f), ColorValueHolder(1.0f, .875f, 0.0f), .625 + sin(2*PI * tickCount/tickCycle)/8 + cos(2*PI * tickCount/tickCycle * 8)/8);
}

ColorValueHolder Lava::getBubbleColor(LavaBubble* bubble) {
	return ColorMixer::mix(ColorValueHolder(1.0f, 1.0f, 1.0f), getBackgroundColor(), 1 - bubble->getAlpha());
	//this is just for sorting the bubbles based on their alpha
}

void Lava::draw() {
	std::cout << bubbles.size() << std::endl;

	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	
	//background:
	//TODO: make drawUnder() a thing
	ColorValueHolder color = getBackgroundColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*background_va, *background_ib, *shader);

	//bubbles:
	if (bubbles.size() == 0) {
		return;
	}

	glLineWidth(2.0f);
	//std::cout << "r: " << bubbles[0]->r << ", X: " << (bubbles[0]->getX()+x) << ", Y: " << (bubbles[0]->getY()+y) << ", A: " << bubbles[0]->getAlpha() << std::endl;
	//first, sort by alpha: lowest to highest
	std::vector<LavaBubble*> sortedBubbles;
	sortedBubbles.reserve(bubbles.size());
	for (int i = 0; i < bubbles.size(); i++) {
		//insertion sort because laziness
		sortedBubbles.push_back(bubbles[i]);
		for (int j = sortedBubbles.size()-1; j >= 1; j--) {
			if (sortedBubbles[j]->getAlpha() < sortedBubbles[j-1]->getAlpha()){
				std::swap(sortedBubbles[j], sortedBubbles[j-1]);
			} else {
				break;
			}
		}
	}

	//second, draw the bubbles (this makes the bubbles less weird-looking when drawn over each other)
	for (int i = 0; i < sortedBubbles.size(); i++) {
		color = getBubbleColor(sortedBubbles[i]);
		MVPM = Renderer::GenerateMatrix(sortedBubbles[i]->r, sortedBubbles[i]->r, 0, sortedBubbles[i]->getX() + x, sortedBubbles[i]->getY() + y);

		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
		shader->setUniformMat4f("u_MVP", MVPM);

		Renderer::Draw(*bubble_va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);
	}
}

void Lava::drawCPU() {
	//background:

	//bubbles:

}