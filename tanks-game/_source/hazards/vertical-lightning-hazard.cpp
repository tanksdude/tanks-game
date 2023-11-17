#include "vertical-lightning-hazard.h"

#include "../constants.h"
#include <cmath>
#include <stdexcept>
#include <algorithm> //std::copy, std::clamp
#include <iostream>
#include "../rng.h"

#include "../renderer.h"
#include "../color-mixer.h"
#include "../background-rect.h"

#include "../collision-handler.h"
#include "../point.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

std::unordered_map<std::string, float> VerticalLightningHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

VerticalLightningHazard::VerticalLightningHazard(double xpos, double ypos, double width, double height) : RectangularLightningHazard(xpos,ypos,width,height) {
	//flexible = false;

	maxBolts = 2;
	//lengthOfBolt = 4;
	bolts.reserve(maxBolts);

	//canAcceptPowers = false;

	//modifiesTankCollision = true;
	//hasSpecialEffectTankCollision = true;
	//modifiesBulletCollision = true;
	//hasSpecialEffectBulletCollision = true;
}

inline Circle* VerticalLightningHazard::getTopPoint() const {
	return new Point(x + w/2, y + h);
}

inline Circle* VerticalLightningHazard::getBottomPoint() const {
	return new Point(x + w/2, y);
}

VerticalLightningHazard::~VerticalLightningHazard() {
	//calls ~RectangularLightningHazard(), so this doesn't need to do anything extra
	//clearBolts();
}

RectHazard* VerticalLightningHazard::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) [[likely]] {
		int count = args.getDataPortionLength(0);

		if (count >= 4) [[likely]] {
			const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
			double x = arr[0];
			double y = arr[1];
			double w = arr[2];
			double h = arr[3];
			return new VerticalLightningHazard(x, y, w, h);
		}
	}

	return new VerticalLightningHazard(0, 0, 0, 0);
}

void VerticalLightningHazard::specialEffectCircleCollision(const Circle* c) {
	//TODO: confirm everything is good
	const Circle* bottomPoint = getBottomPoint();
	const Circle* topPoint = getTopPoint();
	const Circle* circleCenter = new Point(c->x, c->y);
	double intersectionXD, intersectionXU, intersectionYD, intersectionYU;
	int boltPointsD, boltPointsU;

	double xpos, ypos; //circle adjusted x and y

	if (CollisionHandler::fullyCollided(circleCenter, this)) {
		//circle center inside rectangle area
		xpos = c->x;
		ypos = c->y;
	} else {
		//circle center outside rectangle area
		if (c->x < x) {
			xpos = x; //circle too far to the left
		} else if (c->x > x+w) {
			xpos = x+w; //circle too far to the right
		} else {
			xpos = c->x; //circle's x-position is fine
		}
		if (c->y < y) {
			ypos = y; //circle too low
		} else if (c->y > y+h) {
			ypos = y+h; //circle too high
		} else {
			ypos = c->y; //circle's y-position is fine
		}
		//std::cout << "xpos: " << (xpos-c->x) << ", ypos: " << (ypos-c->y) << std::endl;
	}

	if (CollisionHandler::fullyCollided(bottomPoint, c)) {
		intersectionXD = c->x;
		intersectionYD = c->y;
		boltPointsD = 2;
	} else {
		std::pair<PositionHolder, PositionHolder> intersections = CollisionHandler::circleLineIntersection(c, xpos, ypos, bottomPoint->x, bottomPoint->y);
		if (c->x < x + w/2) {
			intersectionXD = std::max(intersections.first.x, intersections.second.x);
		} else {
			intersectionXD = std::min(intersections.first.x, intersections.second.x);
		}
		intersectionYD = std::min(intersections.first.y, intersections.second.y);

		if (intersectionXD < x || intersectionXD > x+w) [[unlikely]] {
			std::cerr << "WARNING: vertical lightning endpoint X (bottom half) out of range!" << std::endl;
			intersectionXD = std::clamp<double>(intersectionXD, x, x+w);
		}
		if (intersectionYD < y || intersectionYD > y+h) [[unlikely]] {
			std::cerr << "WARNING: vertical lightning endpoint Y (bottom half) out of range!" << std::endl;
			intersectionYD = std::clamp<double>(intersectionYD, y, y+h);
		}
		boltPointsD = getDefaultNumBoltPoints(sqrt((intersectionXD - bottomPoint->x)*(intersectionXD - bottomPoint->x) + (intersectionYD - bottomPoint->y)*(intersectionYD - bottomPoint->y)));

		//pushBolt(new LightningBolt(w/2, 0, intersections.x1 - x, intersections.y1 - y, 2)); //debugging
		//pushBolt(new LightningBolt(w/2, 0, intersections.x1 - x, intersections.y2 - y, 2));
		//pushBolt(new LightningBolt(w/2, 0, intersections.x2 - x, intersections.y1 - y, 2));
		//pushBolt(new LightningBolt(w/2, 0, intersections.x2 - x, intersections.y2 - y, 2));
	}

	if (CollisionHandler::fullyCollided(topPoint, c)) {
		intersectionXU = c->x;
		intersectionYU = c->y;
		boltPointsU = 2;
	} else {
		std::pair<PositionHolder, PositionHolder> intersections = CollisionHandler::circleLineIntersection(c, xpos, ypos, topPoint->x, topPoint->y);
		if (c->x < x + w/2) {
			intersectionXU = std::max(intersections.first.x, intersections.second.x);
		} else {
			intersectionXU = std::min(intersections.first.x, intersections.second.x);
		}
		intersectionYU = std::max(intersections.first.y, intersections.second.y);

		if (intersectionXU < x || intersectionXU > x+w) [[unlikely]] {
			std::cerr << "WARNING: vertical lightning endpoint X (top half) out of range!" << std::endl;
			intersectionXU = std::clamp<double>(intersectionXU, x, x+w);
		}
		if (intersectionYU < y || intersectionYU > y+h) [[unlikely]] {
			std::cerr << "WARNING: vertical lightning endpoint Y (top half) out of range!" << std::endl;
			intersectionYU = std::clamp<double>(intersectionYU, y, y+h);
		}
		boltPointsU = getDefaultNumBoltPoints(sqrt((intersectionXU - topPoint->x)*(intersectionXU - topPoint->x) + (intersectionYU - topPoint->y)*(intersectionYU - topPoint->y)));

		//pushBolt(new LightningBolt(intersections.x1 - x, intersections.y1 - y, w/2, h, 2)); //debugging
		//pushBolt(new LightningBolt(intersections.x1 - x, intersections.y2 - y, w/2, h, 2));
		//pushBolt(new LightningBolt(intersections.x2 - x, intersections.y1 - y, w/2, h, 2));
		//pushBolt(new LightningBolt(intersections.x2 - x, intersections.y2 - y, w/2, h, 2));
	}

	//double distL = sqrt(pow(intersectionXD - (x + w/2), 2) + pow(intersectionYD - y, 2));
	//double distR = sqrt(pow((x + w/2) - intersectionXU, 2) + pow((y + h) - intersectionYU, 2));
	//boltPointsD = (boltPointsD < 2 ? getDefaultNumBoltPoints(distL) : boltPointsD);
	//boltPointsU = (boltPointsU < 2 ? getDefaultNumBoltPoints(distR) : boltPointsU);
	pushBolt(new LightningBolt(w/2, 0, intersectionXD-x, intersectionYD-y, boltPointsD), false);
	pushBolt(new LightningBolt(intersectionXD-x, intersectionYD-y, intersectionXU-x, intersectionYU-y, 2), false);
	pushBolt(new LightningBolt(intersectionXU-x, intersectionYU-y, w/2, h, boltPointsU), false);
	delete bottomPoint; delete topPoint; delete circleCenter;

	//TODO: refactor? (it's done, right?)
}

void VerticalLightningHazard::pushBolt(LightningBolt* l, bool simpleRefresh) {
	bolts.push_back(l);
	if (simpleRefresh) {
		simpleRefreshBolt(l);
	} else {
		refreshBolt(l);
	}
}

void VerticalLightningHazard::pushDefaultBolt(int num, bool randomize) {
	for (int i = 0; i < num; i++) {
		LightningBolt* l = new LightningBolt(w/2, 0, w/2, h, getDefaultNumBoltPoints(h));
		if (randomize) {
			pushBolt(l, true);
		} else {
			bolts.push_back(l);
		}
	}
}

bool VerticalLightningHazard::validLocation() const {
	bool wallOnBottom = false, wallOnTop = false; //, wallInMiddle = false;
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		const Wall* wa = WallManager::getWall(i);
		if (CollisionHandler::partiallyCollidedIgnoreEdge(wa, this)) {
			//wallInMiddle = true;
			//break;
			return false;
		}
		//don't short-circuit these comparisons, it's completely unnecessary; though maybe wrap it in (!(wallOnBottom && wallOnTop))
		if ((wa->x <= x) && (wa->x + wa->w >= x + w)) {
			if (y == wa->y + wa->h) {
				wallOnBottom = true;
			} else if (y + h == wa->y) {
				wallOnTop = true;
			}
		}
	}
	if (y == 0) {
		wallOnBottom = true;
	}
	if (y + h == GAME_HEIGHT) {
		wallOnTop = true;
	}
	return (wallOnBottom && wallOnTop); // && !wallInMiddle);
}

bool VerticalLightningHazard::reasonableLocation() const {
	bool wallOnLeft = false, wallOnRight = false;
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		const Wall* wa = WallManager::getWall(i);
		if ((wa->y <= y) && (wa->y + wa->y >= y + h)) {
			if (x == wa->x + wa->w) {
				wallOnLeft = true;
			} else if (x + w == wa->x) {
				wallOnRight = true;
			}
		}
		if (wallOnLeft && wallOnRight) {
			break;
		}
	}
	if (x <= 0) { //should this be ==? //also, should this function return false if the lightning is out of bounds?
		wallOnLeft = true;
	}
	if (x + w >= GAME_WIDTH) {
		wallOnRight = true;
	}

	if (wallOnLeft && wallOnRight) {
		return false;
	}

	//don't check general lightnings
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		if (CollisionHandler::partiallyCollided(this, HazardManager::getCircleHazard(i))) {
			return false;
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		const RectHazard* rh = HazardManager::getRectHazard(i);
		if (rh->getName() != this->getName()) {
			if (CollisionHandler::partiallyCollided(this, rh)) {
				return false;
			}
		}
	}

	return validLocation();
}

void VerticalLightningHazard::simpleRefreshBolt(LightningBolt* l) const {
	const float maxVariance = w/4;
	/* lightning bolts are allowed to be in an area that looks like this:
	 * (see HorizontalLightningHazard for a better diagram, then just mentally rotate it)
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

	float deltaY = (l->positions[l->length*2-1] - l->positions[1]) / (l->length - 1);
	for (int j = 1; j < l->length-1; j++) {
		float xRangeLower = l->positions[j*2 - 2] - maxVariance;
		float xRangeUpper = l->positions[j*2 - 2] + maxVariance;
		float xMin, xMax;
		if (j < l->length/4) { //bottom quarter
			//instead of y=ax+b, use x=(y-b)/a
			xMin = ((deltaY * j) - h/4) / (-.5*(h/w));
			xMax = ((deltaY * j) + h/4) / ( .5*(h/w));
		} else if (j < l->length * (3.0/4.0)) { //middle half
			xMin = 0;
			xMax = w;
		} else { //top quarter
			xMin = ((deltaY * j) - (3.0/4.0)*h) / ( .5*(h/w));
			xMax = ((deltaY * j) - (5.0/4.0)*h) / (-.5*(h/w));
			//alternatively:
			//xMin = ((deltaY * (j - l->length*3.0/4.0)) -   0) / ( .5*h/w);
			//xMax = ((deltaY * (j - l->length*3.0/4.0)) - h/2) / (-.5*h/w);
		}
		xRangeLower = (xRangeLower < xMin ? xMin : xRangeLower);
		xRangeUpper = (xRangeUpper > xMax ? xMax : xRangeUpper);
		l->positions[j*2+0] = RNG::randFloatInRange(xRangeLower, xRangeUpper);
	}
}

void VerticalLightningHazard::refreshBolt(LightningBolt* l) const {
	RectangularLightningHazard::refreshBolt(l, this->w, this->h);
}

inline void VerticalLightningHazard::drawBackground(bool pose, float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	double scale;
	if (pose || currentlyActive) {
		scale = 1.0;
	} else {
		scale = tickCount / (tickCycle * stateMultiplier[currentlyActive]);
	}
	//scale = scale * scale;

	//ColorValueHolder color = (pose ? getBackgroundColor_Pose() : getBackgroundColor());
	ColorValueHolder color = getBackgroundColor_Pose();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

	float coordsAndColor[] = {
		x,   y+(h/2) - (h/2)*scale,   color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		x+w, y+(h/2) - (h/2)*scale,   color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		x+w, y+(h/2) + (h/2)*scale,   color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		x,   y+(h/2) + (h/2)*scale,   color.getRf(), color.getGf(), color.getBf(), color.getAf()
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	Renderer::SubmitBatchedDraw(coordsAndColor, 4 * (2+4), indices, 2 * 3);

	drawBackgroundOutline(alpha);
}

inline void VerticalLightningHazard::drawBackgroundOutline(float alpha) const {
	//alpha set by drawBackground()

	//ColorValueHolder color_outline = ColorValueHolder(0.0f, 0.0f, 0.0f); //black is a bit too strong for a lightning's outline
	ColorValueHolder color_outline = ColorValueHolder(0.5f, 0.5f, 0.5f);
	color_outline = ColorMixer::mix(BackgroundRect::getBackColor(), color_outline, alpha);
	const float lineWidth = 0.5f;
	//using the same color for the background works well, though it's not used because the outline was added to make the lightning's boundary obvious

	float coordsAndColor_outline[] = {
		//outer
		(x)   - lineWidth, (y)   - lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //0
		(x+w) + lineWidth, (y)   - lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //1
		(x+w) + lineWidth, (y+h) + lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //2
		(x)   - lineWidth, (y+h) + lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //3

		//inner
		(x)   + lineWidth, (y)   - lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //4
		(x+w) - lineWidth, (y)   - lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //5
		(x+w) - lineWidth, (y+h) + lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf(), //6
		(x)   + lineWidth, (y+h) + lineWidth,   color_outline.getRf(), color_outline.getGf(), color_outline.getBf(), color_outline.getAf()  //7
	};
	unsigned int indices_outline[] = { //rects
		//right
		1, 2, 6,
		6, 5, 1,

		//left
		4, 7, 3,
		3, 0, 4
	};

	Renderer::SubmitBatchedDraw(coordsAndColor_outline, (4+4) * (2+4), indices_outline, (2*2) * 3);
}

inline void VerticalLightningHazard::drawBolts_Pose(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	//generate bolts
	std::vector<LightningBolt*> poseBolts;
	//from pushDefaultBolt(), mostly
	LightningBolt* l = new LightningBolt(w/2, 0, w/2, h, getDefaultNumBoltPoints(h));
	refreshBolt(l);
	poseBolts.push_back(l);

	//draw
	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 0.75f;

	for (int i = 0; i < poseBolts.size(); i++) {
		//match with drawBolts()
		float* coordsAndColor = new float[(poseBolts[i]->length-1)*4*(2+4)];
		unsigned int* indices = new unsigned int[(poseBolts[i]->length-1)*6];

		for (int j = 0; j < poseBolts[i]->length-1; j++) {
			const int startVertex = (j*4) * 6;
			const int startIndex = j*6;

			SimpleVector2D dist = SimpleVector2D(poseBolts[i]->positions[(j+1)*2] - poseBolts[i]->positions[j*2], poseBolts[i]->positions[(j+1)*2+1] - poseBolts[i]->positions[j*2+1]);
			SimpleVector2D distCW = SimpleVector2D(dist.getAngle() - PI/2, lineWidth, true);

			coordsAndColor[startVertex + 0*6]   = x + poseBolts[i]->positions[j*2]                     + distCW.getXComp();
			coordsAndColor[startVertex + 0*6+1] = y + poseBolts[i]->positions[j*2+1]                   + distCW.getYComp();
			coordsAndColor[startVertex + 1*6]   = x + poseBolts[i]->positions[j*2]   + dist.getXComp() + distCW.getXComp();
			coordsAndColor[startVertex + 1*6+1] = y + poseBolts[i]->positions[j*2+1] + dist.getYComp() + distCW.getYComp();
			coordsAndColor[startVertex + 2*6]   = x + poseBolts[i]->positions[j*2]   + dist.getXComp() - distCW.getXComp();
			coordsAndColor[startVertex + 2*6+1] = y + poseBolts[i]->positions[j*2+1] + dist.getYComp() - distCW.getYComp();
			coordsAndColor[startVertex + 3*6]   = x + poseBolts[i]->positions[j*2]                     - distCW.getXComp();
			coordsAndColor[startVertex + 3*6+1] = y + poseBolts[i]->positions[j*2+1]                   - distCW.getYComp();

			for (int k = 0; k < 4; k++) {
				coordsAndColor[startVertex + k*6+2] = color.getRf();
				coordsAndColor[startVertex + k*6+3] = color.getGf();
				coordsAndColor[startVertex + k*6+4] = color.getBf();
				coordsAndColor[startVertex + k*6+5] = color.getAf();
			}

			indices[startIndex + 0] = startVertex/6 + 0;
			indices[startIndex + 1] = startVertex/6 + 1;
			indices[startIndex + 2] = startVertex/6 + 2;
			indices[startIndex + 3] = startVertex/6 + 2;
			indices[startIndex + 4] = startVertex/6 + 3;
			indices[startIndex + 5] = startVertex/6 + 0;
		}

		Renderer::SubmitBatchedDraw(coordsAndColor, (poseBolts[i]->length-1)*4*(2+4), indices, (poseBolts[i]->length-1)*6);
		delete[] coordsAndColor; delete[] indices;
	}

	for (int i = 0; i < poseBolts.size(); i++) {
		delete poseBolts[i];
	}
}

RectHazard* VerticalLightningHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData& args) {
	if (WallManager::getNumWalls() == 0) [[unlikely]] {
		return nullptr; //don't bother trying to see if a vertical lightning could go from edge to edge
	}

	int attempts = 0;
	RectHazard* randomized = nullptr;
	double xpos, ypos, width, height;
	const double minHeight = 40, maxHeight = 160;

	do {
		width = RNG::randNumInRange(12, 24);
		for (int i = 0; i < WallManager::getNumWalls(); i++) {
			const Wall* wa = WallManager::getWall(i);
			xpos = wa->x + RNG::randFunc() * std::clamp<double>(wa->w - width, 0, wa->w);
			ypos = wa->y + wa->h;
			int j, wallAttempts = 0;
			do {
				j = RNG::randFunc() * WallManager::getNumWalls();
				wallAttempts++;
			} while ((wallAttempts < 8) && (j == i));
			if (j != i) [[likely]] {
				const Wall* otherWall = WallManager::getWall(j);
				height = otherWall->y - ypos;
			} else {
				height = 0;
			}
		}
		if ((height >= minHeight) && (height <= maxHeight) && (xpos >= x_start) && (xpos + width <= x_start + area_width) && (ypos >= y_start) && (ypos + height <= y_start + area_height)) {
			RectHazard* testVerticalLightning = new VerticalLightningHazard(xpos, ypos, width, height);
			if (testVerticalLightning->reasonableLocation()) {
				randomized = testVerticalLightning;
				break;
			} else {
				delete testVerticalLightning;
			}
		}
		attempts++;
	} while (attempts < 32);

	return randomized;
}
