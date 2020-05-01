#pragma once
#include "horizontallightning.h"
#include "renderer.h"
#include "backgroundrect.h"
#include "colormixer.h"
#include "mylib.h"
#include <math.h>
#include <exception>
#include <stdexcept>
#include <iostream>

VertexArray* HorizontalLightning::background_va;
VertexBuffer* HorizontalLightning::background_vb;
IndexBuffer* HorizontalLightning::background_ib;
bool HorizontalLightning::initialized_GPU = false;

HorizontalLightning::HorizontalLightning(double xpos, double ypos, double width, double height) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;

	tickCycle = 100; //100 is JS default (because of power speed)
	stateMultiplier = new double[2] {2, 2};
	currentlyActive = false;
	//flexible = false;
	
	lengthOfBolt = 4; //TODO: figure out logic for constraining this to make it look pretty
	bolts.reserve(maxBolts);
	for (int i = 0; i < maxBolts; i++) {
		int boltPoints = ceil(width / lengthOfBolt); //not floor because the last point is the edge of the lightning area
		boltPoints = (boltPoints < 2 ? 2 : boltPoints);
		bolts.push_back(new LightningBolt(boltPoints));
		for (int j = 0; j < boltPoints; j++) {
			bolts[i]->positions.push_back(float(j)/(boltPoints - 1));
			bolts[i]->positions.push_back(.5);
		}
	}

	canAcceptPowers = false;

	local_initializeGPU();
	initializeGPU();
}

HorizontalLightning::~HorizontalLightning() {
	delete[] stateMultiplier;

	local_uninitializeGPU();
	//uninitializeGPU();
}

bool HorizontalLightning::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	float positions[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	background_vb = new VertexBuffer(positions, 4*2 * sizeof(float), GL_DYNAMIC_DRAW);
	VertexBufferLayout layout(2);
	background_va = new VertexArray(*background_vb, layout);

	background_ib = new IndexBuffer(indices, 6);

	initialized_GPU = true;
	return true;
}

void HorizontalLightning::local_initializeGPU() {
	float* positions = new float[bolts[0]->length*2];
	for (int i = 0; i < bolts[0]->length; i++) {
		positions[i*2]   = bolts[0]->positions[i*2];
		positions[i*2+1] = bolts[0]->positions[i*2+1];
	}
	
	bolt_vb = new VertexBuffer(positions, bolts[0]->length*2 * sizeof(float), GL_STREAM_DRAW);
	VertexBufferLayout layout(2);
	bolt_va = new VertexArray(*bolt_vb, layout);

	delete[] positions;
}

bool HorizontalLightning::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;

	initialized_GPU = false;
	return true;
}

void HorizontalLightning::local_uninitializeGPU() {
	delete bolt_va;
	delete bolt_vb;
}

void HorizontalLightning::streamBoltVertices(unsigned int boltNum) {
	bolt_vb->modifyData(&bolts[boltNum]->positions[0], bolts[boltNum]->length*2 * sizeof(float));
}

RectHazard* HorizontalLightning::factory(int argc, std::string* argv) {
	if (argc >= 4) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double w = std::stod(argv[2]);
		double h = std::stod(argv[3]);
		return new HorizontalLightning(x, y, w, h);
	}
	return new HorizontalLightning(0, 0, 0, 0);
}

void HorizontalLightning::tick() {
	if (!validLocation()) {
		tickCount = 0;
		return;
	}

	tickCount++;
	if (tickCount >= tickCycle * stateMultiplier[currentlyActive]) {
		if (tickCycle * stateMultiplier[currentlyActive] <= 0) {
			tickCount = 0;
			currentlyActive = true;
		} else {
			tickCount -= tickCycle * stateMultiplier[currentlyActive];
			currentlyActive = !currentlyActive;
		}
	}
	
	if (currentlyActive) {
		if (boltTick >= boltCycle) {
			refreshBolts();
			boltTick -= boltCycle;
		}
		boltTick++;
	} else {
		boltTick = 0;
	}
}

bool HorizontalLightning::validLocation() {
	//TODO: finish
	return true;

	/*
	var wallOnLeft=false, wallOnRight=false, wallInMiddle=false;
	for(var i=0; i<walls.length; i++){
		if(!(wallOnLeft && wallOnRight) && walls[i].ypos<=this.ypos && walls[i].ypos+walls[i].height>=this.ypos+this.height){
			if(!wallOnLeft && this.xpos==walls[i].xpos+walls[i].width)
				wallOnLeft=true;
			else if(!wallOnRight && this.xpos+this.width==walls[i].xpos)
				wallOnRight=true;
		}
		if(this.xpos+this.width>walls[i].xpos && this.xpos<walls[i].xpos+walls[i].width && this.ypos+this.height>walls[i].ypos && this.ypos<walls[i].ypos+walls[i].height){
			wallInMiddle=true;
			break;
		}
	}
	return (wallOnLeft && wallOnRight && !wallInMiddle);
	*/
}

void HorizontalLightning::refreshBolts() {
	for (int i = 0; i < bolts.size(); i++) {
		for (int j = 0; j < bolts[i]->length; j++) {
			bolts[i]->positions[j*2]   = float(j)/(bolts[i]->length - 1);
			bolts[i]->positions[j*2+1] = rand()/double(RAND_MAX);
		}
	}
}

ColorValueHolder HorizontalLightning::getBackgroundColor() {
	if (currentlyActive) {
		return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(.75f, .75f, .75f), .25);
	}
	return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(.75f, .75f, .75f), .25*constrain_d(tickCount/(tickCycle*stateMultiplier[currentlyActive]), 0, 1));
}

ColorValueHolder HorizontalLightning::getBoltColor() {
	return ColorValueHolder(0xBB/255.0, 1.0f, 1.0f);
}

void HorizontalLightning::draw() {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	
	//background:
	//TODO: make drawUnder() a thing
	ColorValueHolder color = getBackgroundColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*background_va, *background_ib, *shader);

	//bolts:
	if (!currentlyActive) {
		return;
	}

	glLineWidth(2.0f);
	color = getBoltColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	//shader->setUniformMat4f("u_MVP", MVPM);

	for (int i = 0; i < bolts.size(); i++) {
		streamBoltVertices(i);
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, bolts[i]->length);
	}
}

void HorizontalLightning::drawCPU() {
	//background:

	//bolts:

}