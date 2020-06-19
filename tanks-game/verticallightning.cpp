#include "verticallightning.h"
#include "gamemanager.h"
#include "renderer.h"
#include "backgroundrect.h"
#include "colormixer.h"
#include "mylib.h"
#include "constants.h"
#include <math.h>
#include <stdexcept>
#include <algorithm> //std::copy
#include "point.h"
#include "collisionhandler.h"
#include "wallmanager.h"
#include <iostream>

VertexArray* VerticalLightning::background_va;
VertexBuffer* VerticalLightning::background_vb;
IndexBuffer* VerticalLightning::background_ib;
bool VerticalLightning::initialized_GPU = false;

VerticalLightning::VerticalLightning(double xpos, double ypos, double width, double height) : RectangularLightning(xpos,ypos,width,height,true) {
	//flexible = false;
	
	maxBolts = 2;
	//lengthOfBolt = 4;
	bolts.reserve(maxBolts);
	pushDefaultBolt(maxBolts, true);

	//canAcceptPowers = false;

	//modifiesTankCollision = true;
	//hasSpecialEffectTankCollision = true;
	//modifiesBulletCollision = true;
	//hasSpecialEffectBulletCollision = true;

	local_initializeGPU();
	initializeGPU();
}

Circle* VerticalLightning::getTopPoint() {
	return new Point(x + w/2, y + h);
}

Circle* VerticalLightning::getBottomPoint() {
	return new Point(x + w/2, y);
}

VerticalLightning::~VerticalLightning() {
	//calls ~RectangularLightning(), so this doesn't need to do anything extra
	//clearBolts();

	local_uninitializeGPU(); //I don't know if this is okay, but there isn't an error...
	//uninitializeGPU();
}

bool VerticalLightning::initializeGPU() {
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

//requires a bolt to initialize:
void VerticalLightning::local_initializeGPU() {
	float* positions = new float[bolts[0]->length*2];
	for (int i = 0; i < bolts[0]->length; i++) {
		positions[i*2]   = bolts[0]->positions[i*2];
		positions[i*2+1] = bolts[0]->positions[i*2+1];
	}
	bolt_vb_length = bolts[0]->length;
	
	bolt_vb = new VertexBuffer(positions, bolts[0]->length*2 * sizeof(float), GL_STREAM_DRAW);
	VertexBufferLayout layout(2);
	bolt_va = new VertexArray(*bolt_vb, layout);

	delete[] positions;
}

void VerticalLightning::local_reinitializeGPU(int length) { //does not seed the VertexBuffer with values
	delete bolt_va;
	delete bolt_vb;

	float* positions = new float[length*2];
	bolt_vb_length = length;
	
	bolt_vb = new VertexBuffer(positions, length*2 * sizeof(float), GL_STREAM_DRAW);
	VertexBufferLayout layout(2);
	bolt_va = new VertexArray(*bolt_vb, layout);

	delete[] positions;
}

bool VerticalLightning::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;

	initialized_GPU = false;
	return true;
}

void VerticalLightning::local_uninitializeGPU() {
	delete bolt_va;
	delete bolt_vb;
}

void VerticalLightning::streamBoltVertices(unsigned int boltNum) {
	bolt_vb->modifyData(bolts[boltNum]->positions.data(), bolts[boltNum]->length*2 * sizeof(float));
}

RectHazard* VerticalLightning::factory(int argc, std::string* argv) {
	if (argc >= 4) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double w = std::stod(argv[2]);
		double h = std::stod(argv[3]);
		return new VerticalLightning(x, y, w, h);
	}
	return new VerticalLightning(0, 0, 0, 0);
}

void VerticalLightning::specialEffectCircleCollision(Circle* c) {
	/* Lightning zone/region layout:
	 *
	 *    6LU   |    5U   |   6RU
	 *          |         |
	 *----------+----0----+----------      <- y+h
	 *          |         |
	 *    3LU   |    4U   |   3RU
	 *----------+---------+----------      <- y+h - c->r
	 *          |         |
	 *          |         |
	 *          |         |
	 *          |         |
	 *    2L    |    1    |   2R
	 *          |         |
	 *          |         |
	 *          |         |
	 *          |         |
	 *----------+---------+----------      <- y + c->r
	 *    3LD   |    4D   |   3RD
	 *          |         |
	 *----------+----0----+----------      <- y
	 *          |         |
	 *    6LD   |    5D   |   6RD
	 *
	 *
	 *          ^         ^
	 *          x         x+w
	 *
	 * 0 = topPoint/bottomPoint
	 * zone/region 7: circle radius c->r around 0
	 * determined by circle center
	 */

	//TODO: confirm everything is good
	Circle* bottomPoint = getBottomPoint();
	Circle* topPoint = getTopPoint();
	double intersectionXD, intersectionXU, intersectionYD, intersectionYU;
	int boltPointsD = -1, boltPointsU = -1;
	//determine region the circle is in
	if (c->y-c->r >= y && c->y+c->r <= y+h) {
		if (c->x >= x && c->x <= x+w) {
			intersectionXD = c->x;
			intersectionXU = c->x;
			intersectionYD = c->y - c->r;
			intersectionYU = c->y + c->r;
			//std::cout << "case 1" << std::endl;
		} else {
			if (c->x > x+w) {
				intersectionXD = x + w;
				intersectionXU = x + w;
				//std::cout << "case 2R" << std::endl;
			} else { //c->x < x
				intersectionXD = x;
				intersectionXU = x;
				//std::cout << "case 2L" << std::endl;
			}
			intersectionYD = c->y - sqrt(pow(c->r, 2) - pow(intersectionXD - c->x, 2));
			//intersectionYD = (intersectionYD < y ? y : intersectionYD);
			intersectionYU = c->y + sqrt(pow(c->r, 2) - pow(intersectionXU - c->x, 2));
			//intersectionYU = (intersectionYU > y+h ? y+h : intersectionYU);
		}
	} else {
		//I think case 4 and 5 are the same
		if (c->y-c->r < y) {
			//bottom

			if (CollisionHandler::fullyCollided(bottomPoint, c)) {
				intersectionXD = c->x;
				intersectionYD = c->y;
				boltPointsD = 2;
				if (c->x > x+w) {
					intersectionXU = x + w;
					intersectionYU = c->y + sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
				} else if (c->x < x) {
					intersectionXU = x;
					intersectionYU = c->y + sqrt(pow(c->r, 2) - pow(c->x - x, 2));
				} else {
					intersectionXU = c->x;
					intersectionYU = c->y + c->r;
				}
				//std::cout << "case 7D_D" << std::endl;
			} else {
				//check whether the circle is really far down, or just barely out of range of bottomPoint
				//difference between 6RD and 3RD: intersectionXD
				if (c->y <= y) {
					if (c->x > x+w) {
						intersectionXD = c->x - sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						intersectionYD = y;
						intersectionXU = x + w;
						intersectionYU = c->y + sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						//std::cout << "case 6RD ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					} else if (c->x < x) {
						intersectionXD = c->x + sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						intersectionYD = y;
						intersectionXU = x;
						intersectionYU = c->y + sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						//std::cout << "case 6LD ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					} else {
						intersectionYD = y;
						if (c->x > x + w/2) {
							intersectionXD = c->x - sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						} else {
							intersectionXD = c->x + sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						}
						intersectionYU = c->y + c->r;
						intersectionXU = c->x;
						//std::cout << "case 5D ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					}
				} else {
					if (c->x > x+w) {
						intersectionXD = c->x - sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						intersectionXD = (intersectionXD > x+w ? x+w : intersectionXD);
						intersectionYD = c->y - sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						intersectionYD = (intersectionYD < y ? y : intersectionYD);
						intersectionXU = x + w;
						intersectionYU = c->y + sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						//std::cout << "case 3RD ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					} else if (c->x < x) {
						intersectionXD = c->x + sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						intersectionXD = (intersectionXD < x ? x : intersectionXD);
						intersectionYD = c->y - sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						intersectionYD = (intersectionYD < y ? y : intersectionYD);
						intersectionXU = x;
						intersectionYU = c->y + sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						//std::cout << "case 3LD ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					} else {
						intersectionYD = y;
						if (c->x > x + w/2) {
							intersectionXD = c->x - sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						} else {
							intersectionXD = c->x + sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						}
						intersectionXU = c->x;
						intersectionYU = c->y + c->r;
						//std::cout << "case 4D ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					}
				}
			}

			if (CollisionHandler::fullyCollided(topPoint, c)) { //this should be very rare (only happens for small lightnings)
				intersectionXU = c->x;
				intersectionYU = c->y;
				boltPointsU = 2;
				//std::cout << "case 7U_D" << std::endl;
			}
		} else { //c->y+c->r > y+h
			//top

			if (CollisionHandler::fullyCollided(topPoint, c)) {
				intersectionXU = c->x;
				intersectionYU = c->y;
				boltPointsU = 2;
				if (c->x > x+w) {
					intersectionXD = x + w;
					intersectionYD = c->y - sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
				} else if (c->x < x) {
					intersectionXD = x;
					intersectionYD = c->y - sqrt(pow(c->r, 2) - pow(c->x - x, 2));
				} else {
					intersectionXD = c->x;
					intersectionYD = c->y - c->r;
				}
				//std::cout << "case 7U_U" << std::endl;
			} else {
				if (c->y >= y+h) {
					if (c->x > x+w) {
						intersectionXD = x + w;
						intersectionYD = c->y - sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						intersectionXU = c->x - sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						intersectionYU = y + h;
						//std::cout << "case 6RU ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					} else if (c->x < x) {
						intersectionXD = x;
						intersectionYD = c->y - sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						intersectionXU = c->x + sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						intersectionYU = y + h;
						//std::cout << "case 6LU ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					} else {
						intersectionXD = c->x;
						intersectionYD = c->y - c->r;
						intersectionYU = y + h;
						if (c->x > x + w/2) {
							intersectionXU = c->x - sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						} else {
							intersectionXU = c->x + sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						}
						//std::cout << "case 5U ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					}
				} else {
					if (c->x > x+w) {
						intersectionXD = x + w;
						intersectionYD = c->y - sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						intersectionXU = c->x - sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						intersectionXU = (intersectionXU > x+w ? x+w : intersectionXU);
						intersectionYU = c->y + sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						intersectionYU = (intersectionYU > y+h ? y+h : intersectionYU);
						//std::cout << "case 3RU ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					} else if (c->x < x) {
						intersectionXD = x;
						intersectionYD = c->y - sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						intersectionXU = c->x + sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						intersectionXU = (intersectionXU < x ? x : intersectionXU);
						intersectionYU = c->y + sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						intersectionYU = (intersectionYU > y+h ? y+h : intersectionYU);
						//std::cout << "case 3LU ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					} else {
						intersectionXD = c->x;
						intersectionYD = c->y - c->r;
						intersectionYU = y + h;
						if (c->x > x + w/2) {
							intersectionXU = c->x - sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						} else {
							intersectionXU = c->x + sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						}
						//std::cout << "case 4U ";
						//std::cout << "xD:" << (intersectionXD - x) << " xU:" << (intersectionXU - x) << " yD:" << (intersectionYD - y) << " yU:" << (intersectionYU - y) << std::endl;
					}
				}
			}

			if (CollisionHandler::fullyCollided(bottomPoint, c)) { //this should be very rare (only happens for small lightnings)
				intersectionXD = c->x;
				intersectionYD = c->y;
				boltPointsD = 2;
				//std::cout << "case 7D_U" << std::endl;
			}
		}
	}
	double distL = sqrt(pow(intersectionXD - (x + w/2), 2) + pow(intersectionYD - y, 2));
	double distR = sqrt(pow((x + w/2) - intersectionXU, 2) + pow((y + h) - intersectionYU, 2));
	boltPointsD = (boltPointsD < 2 ? getDefaultNumBoltPoints(distL) : boltPointsD);
	boltPointsU = (boltPointsU < 2 ? getDefaultNumBoltPoints(distR) : boltPointsU);
	pushBolt(new LightningBolt(w/2, 0, intersectionXD-x, intersectionYD-y, boltPointsD), false);
	pushBolt(new LightningBolt(intersectionXD-x, intersectionYD-y, intersectionXU-x, intersectionYU-y, 2), false);
	pushBolt(new LightningBolt(intersectionXU-x, intersectionYU-y, w/2, h, boltPointsU), false);
	delete topPoint, bottomPoint;

	//TODO: refactor
}

void VerticalLightning::pushBolt(LightningBolt* l, bool simpleRefresh) {
	if (l->length > bolt_vb_length) {
		local_reinitializeGPU(l->length);
	}
	bolts.push_back(l);
	if (simpleRefresh) {
		simpleRefreshBolt(bolts.size() - 1);
	} else {
		refreshBolt(bolts.size() - 1);
	}
}

void VerticalLightning::pushDefaultBolt(int num, bool randomize) {
	for (int i = 0; i < num; i++) {
		LightningBolt* l = new LightningBolt(w/2, 0, w/2, h, getDefaultNumBoltPoints(h));
		if (randomize) {
			pushBolt(l, true);
		} else {
			if (l->length > bolt_vb_length) {
				local_reinitializeGPU(l->length);
			}
			bolts.push_back(l);
		}
	}
}

bool VerticalLightning::validLocation() {
	bool wallOnTop = false, wallOnBottom = false, wallInMiddle = false;
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		//I'm not sure numerical comparisons are "costly" to the point where short circuiting is needed
		if (!(wallOnTop && wallOnBottom) && (wa->x <= x) && (wa->x + wa->w >= x + w)) {
			if (!wallOnTop && (y == wa->y + wa->h)) {
				wallOnTop = true;
			} else if (!wallOnBottom && (y + h == wa->y)) {
				wallOnBottom = true;
			}
		}
		if (CollisionHandler::partiallyCollidedIgnoreEdge(wa, this)) {
			wallInMiddle=true;
			break;
		}
	}
	if (!wallOnTop && (y + h == GAME_HEIGHT)) {
		wallOnTop = true;
	}
	if (!wallOnBottom && (y == 0)) {
		wallOnBottom = true;
	}
	return (wallOnTop && wallOnBottom && !wallInMiddle);
}

bool VerticalLightning::reasonableLocation() {
	bool wallOnLeft = false, wallOnRight = false;
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		if ((wa->y <= y) && (wa->y + wa->y >= y + h)) {
			if (!wallOnLeft && (x == wa->x + wa->w)) {
				wallOnLeft = true;
			} else if (!wallOnRight && (x + w == wa->x)) {
				wallOnRight = true;
			}
		}
		if (wallOnLeft && wallOnRight) {
			break;
		}
	}
	if (!wallOnLeft && (x <= 0)) { //should this be ==? //also, should this function return false if the lightning is out of bounds?
		wallOnLeft = true;
	}
	if (!wallOnRight && (x + w >= GAME_WIDTH)) {
		wallOnRight = true;
	}
	return (!(wallOnLeft && wallOnRight) && validLocation());
}

void VerticalLightning::simpleRefreshBolt(int num) {
	double maxVariance = w/4;
	/* lightning bolts are allowed to be in an area that looks like this:
	 * (see HorizontalLightning for a better diagram, then just mentally rotate it)
	 * +-------- /-\ --------+        <- h
	 *         /     \        
	 *      /           \     
	 *   /                 \  
	 * |                     |
	 * |                     |
	 * |                     |
	 *   \                 /
	 *      \           /
	 *         \     /
	 * +-------- \-/ --------+        <- 0
	 * 
	 * ^                     ^
	 * 0                     w
	 *
	 * the region is (from bottom to top) 1/4 triangle, 1/2 rectangle, then 1/4 triangle
	 */

	double deltaY = (bolts[num]->positions[bolts[num]->length*2-1] - bolts[num]->positions[1]) / (bolts[num]->length - 1);
	for (int j = 1; j < bolts[num]->length-1; j++) {
		double xRangeLower = bolts[num]->positions[j*2 - 2] - maxVariance;
		double xRangeUpper = bolts[num]->positions[j*2 - 2] + maxVariance;
		double xMin, xMax;
		if (j < bolts[num]->length/4) { //bottom quarter
			//instead of y=ax+b, use x=(y-b)/a
			xMin = ((deltaY * j) - h/4) / (-.5*h/w);
			xMax = ((deltaY * j) + h/4) / ( .5*h/w);
		} else if (j < bolts[num]->length * 3.0/4.0) { //middle half
			xMin = 0;
			xMax = w;
		} else { //top quarter
			xMin = ((deltaY * j) - 3.0/4.0*h) / ( .5*h/w);
			xMax = ((deltaY * j) - 5.0/4.0*h) / (-.5*h/w);
			//alternatively:
			//xMin = ((deltaY * (j - bolts[num]->length*3.0/4.0)) -   0) / ( .5*h/w);
			//xMax = ((deltaY * (j - bolts[num]->length*3.0/4.0)) - h/2) / (-.5*h/w);
		}
		xRangeLower = (xRangeLower < xMin ? xMin : xRangeLower);
		xRangeUpper = (xRangeUpper > xMax ? xMax : xRangeUpper);
		bolts[num]->positions[j*2+0] = xRangeLower + (xRangeUpper - xRangeLower) * randFunc2();
	}
}

void VerticalLightning::refreshBolt(int num) {
	RectangularLightning::refreshBolt(num, w, h);
}

void VerticalLightning::draw() {
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
	MVPM = Renderer::GenerateMatrix(1, 1, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	for (int i = 0; i < bolts.size(); i++) {
		//I think the VertexBuffer resizing should happen here, but there would probably be less strain if it happens only when a bullet/tank collides
		//TODO: that ^ should be the preferred way, since only draw() (and initializeGPU()) should do GPU stuff
		/*
		if (bolts[i]->length > bolt_vb_length) {
			local_reinitializeGPU(bolts[i]->length);
		}
		*/
		streamBoltVertices(i);
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, bolts[i]->length);
	}
}

void VerticalLightning::drawCPU() {
	//background:

	//bolts:

}
