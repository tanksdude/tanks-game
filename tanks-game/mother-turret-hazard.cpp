#include "mother-turret-hazard.h"
#include "renderer.h"
#include "constants.h"
#include <cmath>
#include "color-mixer.h"
#include "background-rect.h"
#include <algorithm>
#include "mylib.h"
#include "tank.h"
#include "tank-manager.h"
#include "bullet-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "collision-handler.h"
#include "rng.h"
#include "targeting-turret-hazard.h" //in case mother turrets no longer extend targeting turrets
#include <iostream>

VertexArray* MotherTurretHazard::va;
VertexBuffer* MotherTurretHazard::vb;
IndexBuffer* MotherTurretHazard::ib;
VertexArray* MotherTurretHazard::cannon_va;
VertexBuffer* MotherTurretHazard::cannon_vb;
bool MotherTurretHazard::initialized_GPU = false;

std::unordered_map<std::string, float> MotherTurretHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 0.5f });
	weights.insert({ "random-vanilla", 0.25f });
	weights.insert({ "random", 0.125f });
	return weights;
}

MotherTurretHazard::MotherTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren, double childDistMultiplier) : TargetingTurretHazard(xpos, ypos, angle, true) {
	//x = xpos;
	//y = ypos;
	//velocity = SimpleVector2D(angle, 0, true);
	r = TANK_RADIUS * 2; // that's a little big

	stateMultiplier[1] *= 4;
	turningIncrement *= 2;

	maxChildTurrets = (maxChildren/2)*2; //even numbers only
	childDistFromMother = r * childDistMultiplier; //probably default: (r/4) * 3 //TODO: r, r/2, r*.75 (3/4*r because child turret diameter + radius)
	initialAngle = angle;
	targetingNum = -1;

	childTurretIDs = std::vector<Game_ID>(maxChildTurrets, -1);
	childTurretAlive = std::vector<bool>(maxChildTurrets, false);
	//pushInitialChildren(startChildren);
	initialChildren = startChildren;

	canAcceptPowers = false; //... true?

	initializeGPU();

	//idea: mitosis turrets; grows for some time, then splits (that'd be hard to program...)
}

MotherTurretHazard::MotherTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren) : MotherTurretHazard(xpos, ypos, angle, maxChildren, startChildren, .75) {
	//nothing
	//child distance options: r, r/2, r*.75
}

//16 or 8 max turrets? 16 max turrets is certainly sizable, 8 is more reasonable...
MotherTurretHazard::MotherTurretHazard(double xpos, double ypos, double angle) : MotherTurretHazard(xpos, ypos, angle, 8, 4) {
	//something?
	//reason for choosing 8 max by default: don't want the randomly-placed ones to be excessive
	//other reason: wouldn't it be interesting if there was only one that was different? it's the small details that count
}

MotherTurretHazard::~MotherTurretHazard() {
	//delete[] stateMultiplier;
	//delete[] stateColors;

	//uninitializeGPU();
}

bool MotherTurretHazard::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//body:
	float positions[(Circle::numOfSides+1)*(2+4)];
	positions[0] = 0;
	positions[1] = 0;
	positions[2] = 0.5f;
	positions[3] = 0.5f;
	positions[4] = 0.5f;
	positions[5] = 1.0f;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		positions[i*6]   = cos((i-1) * 2*PI / Circle::numOfSides);
		positions[i*6+1] = sin((i-1) * 2*PI / Circle::numOfSides);
		positions[i*6+2] = 0.5f;
		positions[i*6+3] = 0.5f;
		positions[i*6+4] = 0.5f;
		positions[i*6+5] = 1.0f;
	}

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = 0;
		indices[i*3+1] = i+1;
		indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	vb = VertexBuffer::MakeVertexBuffer(positions, (Circle::numOfSides+1)*(2+4) * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	vb->SetLayout(layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	va = VertexArray::MakeVertexArray();
	va->AddVertexBuffer(vb);
	va->SetIndexBuffer(ib);

	ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	//cannon:
	float cannon_positions[(2+4)*2] = {
		0.0f, 0.0f,    0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f,    0.0f, 0.0f, 0.0f, 1.0f
	};
	cannon_vb = VertexBuffer::MakeVertexBuffer(cannon_positions, (2+4)*2 * sizeof(float));
	VertexBufferLayout cannon_layout = {
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	cannon_vb->SetLayout(layout);

	cannon_va = VertexArray::MakeVertexArray();
	cannon_va->AddVertexBuffer(cannon_vb);

	initialized_GPU = true;
	return true;
}

bool MotherTurretHazard::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;
	delete cannon_va;
	delete cannon_vb;

	initialized_GPU = false;
	return true;
}

CircleHazard* MotherTurretHazard::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count_basic = args.getDataPortionLength(0);

		if (count_basic >= 3) {
			double* arr_basic = (double*)args.getDataPortion(0);
			double x = arr_basic[0];
			double y = arr_basic[1];
			double a = arr_basic[2];

			if (args.getDataCount() >= 2) {
				int count_num = args.getDataPortionLength(1);

				if (count_num >= 1) {
					int* arr_num = (int*)args.getDataPortion(1);

					int child_max = arr_num[0];
					int child_start = ((count_num >= 2) ? arr_num[1] : 0);

					if ((args.getDataCount() >= 3) && (args.getDataPortionLength(2) >= 1)) {
						double* arr_dist = (double*)args.getDataPortion(2);
						return new MotherTurretHazard(x, y, a, child_max, child_start, arr_dist[0]);
					}

					return new MotherTurretHazard(x, y, a, child_max, child_start);
				}
			}

			return new MotherTurretHazard(x, y, a);
		}
	}

	return new MotherTurretHazard(0, 0, 0, 0, 0);
}

//probably works
inline void MotherTurretHazard::pushInitialChildren(int childCount) {
	childCount = (childCount/2)*2; //even numbers only

	if ((maxChildTurrets == 0) || (childCount == 0)) {
		return;
	}

	//for every power of 2 of maxChildren, split working area in half
	/* pretend this is a circle:
	 * 
	 *         @
	 *       @   @
	 *     @       @
	 *   @           @
	 * @       X       @
	 *   @           @
	 *     @       @
	 *       @   @
	 *         @
	 * 
	 *         @
	 *       @   @
	 *     @       @
	 *   @           @
	 * @-------X-------@
	 *   @           @
	 *     @       @
	 *       @   @
	 *         @
	 * 
	 *         @
	 *       @ | @
	 *     @   |   @
	 *   @     |     @
	 * @-------X-------@
	 *   @     |     @
	 *     @   |   @
	 *       @ | @
	 *         @
	 * 
	 *         @
	 *       @ | @
	 *     @ \ | / @
	 *   @    \|/    @
	 * @-------X-------@
	 *   @    /|\    @
	 *     @ / | \ @
	 *       @ | @
	 *         @
	 * 
	 */
	//after splitting the working area, place the initial children in there, with the same pattern for each part

	if (childCount >= maxChildTurrets) {
		for (int i = 0; i < maxChildTurrets; i++) {
			pushChild(i);
		}
		return;
	}

	int currentMaxChildren = maxChildTurrets;
	int currentInitialChildren = childCount;
	int angleSplit = 1;
	while ((currentMaxChildren%2 == 0) && (currentInitialChildren%2 == 0)) {
		angleSplit *= 2;
		currentMaxChildren /= 2;
		currentInitialChildren /= 2;
	}

	/*
	const double turretAngleDiff = (2*PI) / maxChildTurrets;
	const double workingAngle = (2*PI) / angleSplit;
	//angle area to choose turret spots: (workingAngle*i, workingAngle*(i+1)]
	for (int i = 0; i < angleSplit; i++) {
		const double currentAngleStart = workingAngle * i;
		const double currentAngleEnd = workingAngle * (i+1);
		if (currentInitialChildren >= currentMaxChildren) {
			for (int j = 1; j <= currentMaxChildren; j++) {
				pushChild(this->velocity.getAngle() + currentAngleStart + turretAngleDiff*j);
			}
		} else {
			for (int j = 1; j <= currentInitialChildren; j++) {
				pushChild(this->velocity.getAngle() + currentAngleStart + turretAngleDiff*round((j * workingAngle/double(currentInitialChildren+1)) / turretAngleDiff));
			}
		}
	}
	*/

	const double turretAngleDiff = (2*PI) / maxChildTurrets;
	const double workingAngle = (2*PI) / angleSplit;
	for (int i = 0; i < angleSplit; i++) {
		for (int j = 1; j <= currentInitialChildren; j++) {
			pushChild(i*currentMaxChildren + round((j * workingAngle/double(currentInitialChildren+1)) / turretAngleDiff));
		}
	}
}

inline CircleHazard* MotherTurretHazard::makeTurret(int turretNum) const {
	GenericFactoryConstructionData constructionData;
	double angle = getChildTurretAngle(turretNum);
	double* posArr = new double[3]{ this->x + (this->r+childDistFromMother) * cos(angle), this->y + (this->r+childDistFromMother) * sin(angle), angle };
	constructionData = GenericFactoryConstructionData(3, posArr);
	CircleHazard* childTurret = HazardManager::getCircleHazardFactory("vanilla", "targeting_turret")(constructionData);
	delete[] posArr;
	return childTurret;
}

void MotherTurretHazard::pushChild(int turretNum) {
	//make turret
	CircleHazard* childTurret = makeTurret(turretNum);

	/*
	//find turret position number
	int turretNum = (int)round(((2*PI) / maxChildTurrets) / abs(angle)) % maxChildTurrets;
	while (this->childTurretIDs.size() <= turretNum) {
		this->childTurretIDs.push_back(-1);
	}
	while (this->childTurretAlive.size() <= turretNum) {
		this->childTurretIDs.push_back(false);
	}
	*/

	//add turret to child list
	this->childTurretIDs[turretNum] = childTurret->getGameID();
	this->childTurretAlive[turretNum] = true;

	//push turret
	HazardManager::pushCircleHazard(childTurret);
}

double MotherTurretHazard::getChildTurretAngle(int turretNum) const {
	const double turretAngleDiff = (2*PI) / maxChildTurrets;
	return initialAngle + turretNum * turretAngleDiff;
}

void MotherTurretHazard::initialize() {
	pushInitialChildren(initialChildren);
}

void MotherTurretHazard::tick() {
	//tickCount is unused... maybe targetingCount should replace it...

	updateChildCount();

	if (currentState == 0) { //either turning to next turret spot or doing nothing
		if (targeting) { //tracking spot
			tick_trackSpot();
		}
		if (!targeting) { //looking for an open spot
			tick_chooseSpot();
		}
	}

	if (currentState == 1) { //charging up
		tick_chargeUp();
	}

	if (currentState == 2) { //cooling down
		tick_cooldown();
	}

	//maxState is 3; not using else in case tickCycle is zero
}

inline void MotherTurretHazard::tick_chooseSpot() {
	//choose the spot that will result in the most time spent turning
	if (getChildCount() < maxChildTurrets) {
		const double childAngleDiff = (2*PI) / maxChildTurrets;
		//double* angleDiff = new double[maxChildTurrets];
		std::vector<double> angleDiff; angleDiff.reserve(maxChildTurrets);
		for (int i = 0; i < maxChildTurrets; i++) {
			if (childTurretAlive[i]) {
				//angleDiff[i] = -2*PI;
				angleDiff.push_back(-2*PI);
			} else {
				//angleDiff[i] = SimpleVector2D::angleBetween(this->velocity, SimpleVector2D(getChildTurretAngle(i), 0, true));
				angleDiff.push_back(abs(SimpleVector2D::angleBetween(this->velocity, SimpleVector2D(getChildTurretAngle(i), 0, true))));
			}
		}
		targetingNum = findMaxIndex(angleDiff.data(), maxChildTurrets);
		targeting = true;
		//delete[] angleDiff;
	} else {
		targeting = false;
		targetingCount = 0;
	}
}

inline void MotherTurretHazard::tick_trackSpot() {
	turnTowardsPoint(targetingNum);
	if (isPointedAt(targetingNum)) {
		targetingCount++;
		if (targetingCount >= stateMultiplier[0] * tickCycle) {
			currentState = 1;
			targetingCount = 0;
		}
	} else {
		targetingCount--;
		if (targetingCount < 0) {
			targetingCount = 0;
		}
	}
}

inline void MotherTurretHazard::tick_chargeUp() {
	targetingCount++;
	if (targetingCount >= stateMultiplier[1] * tickCycle) {
		//BulletManager::pushBullet(new Bullet(x + r*cos(velocity.getAngle()), y + r*sin(velocity.getAngle()), r*BULLET_TO_TANK_RADIUS_RATIO, velocity.getAngle(), Tank::default_maxSpeed*BULLET_TO_TANK_SPEED_RATIO, this->getTeamID(), BulletParentType::individual, this->getGameID()));
		pushChild(targetingNum);
		currentState = 2;
		targetingCount = 0;
		targeting = false; //allows target to change (also controls whether the reticule is drawn)
		//targetingNum = -1; //needed?
	}
}

inline void MotherTurretHazard::tick_cooldown() {
	targetingCount++;
	if (targetingCount >= stateMultiplier[2] * tickCycle) {
		targetingCount = 0;
		currentState = 0;
	}
}

inline int MotherTurretHazard::getChildCount() const {
	return std::count(childTurretAlive.begin(), childTurretAlive.end(), true);
}

int MotherTurretHazard::updateChildCount() {
	//loop through current children; it's alive if there's an ID match
	int count = 0;
	for (int i = 0; i < maxChildTurrets; i++) {
		if (!childTurretAlive[i]) {
			continue;
		}

		if (HazardManager::getCircleHazardByID(childTurretIDs[i]) == nullptr) {
			//not found, dead
			childTurretIDs[i] = -1;
			childTurretAlive[i] = false;
		} else {
			//found, alive
			count++;
		}
		/*
		bool found = false;
		for (int j = 0; j < HazardManager::getNumCircleHazards(); j++) {
			CircleHazard* ch = HazardManager::getCircleHazard(j);
			if (ch->getGameID() == childTurretIDs[i]) {
				//found, is alive
				count++;
				found = true;
				break;
			}
		}

		//not found, isn't alive
		if (!found) {
			childTurretIDs[i] = -1;
			childTurretAlive[i] = false;
		}
		*/
	}

	return count;
}

void MotherTurretHazard::turnTowardsPoint(int turretNum) {
	//see TargetingTurretHazard::turnTowardsTank
	double angle = getChildTurretAngle(turretNum);
	SimpleVector2D distToChild = SimpleVector2D(getChildTurretAngle(turretNum), this->r+childDistFromMother, true);
	float theta = SimpleVector2D::angleBetween(distToChild, velocity);
	if (abs(theta) < PI/turningIncrement) {
		//too small to adjust angle
	} else {
		//large angle adjustment needed
		if (theta < 0) {
			this->velocity.changeAngle(PI/turningIncrement);
		} else {
			this->velocity.changeAngle(-PI/turningIncrement);
		}
	}
}

bool MotherTurretHazard::isPointedAt(int turretNum) const {
	double angle = getChildTurretAngle(turretNum);
	return (abs(SimpleVector2D::angleBetween(velocity, SimpleVector2D(angle, 0, true))) < PI/turningIncrement);
}

bool MotherTurretHazard::reasonableLocation() const {
	for (int i = 0; i < maxChildTurrets; i++) {
		TargetingTurretHazard* testChild = (TargetingTurretHazard*)makeTurret(i);
		if (!testChild->reasonableLocation()) {
			delete testChild;
			return false;
		}
		for (int j = 0; j < WallManager::getNumWalls(); j++) {
			if (CollisionHandler::lineRectCollision(this->x, this->y, testChild->getX(), testChild->getY(), WallManager::getWall(j))) {
				delete testChild;
				return false;
			}
		}
		delete testChild;
	}
	//probably enough

	return TargetingTurretHazard::reasonableLocation();
}

void MotherTurretHazard::draw() const {
	drawShootingTimer();
	drawBody();
	drawOutline();
	drawBarrel();
}

void MotherTurretHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for MotherTurretHazard::draw!" << std::endl;
		case DrawingLayers::normal:
			drawShootingTimer();
			drawBody();
			drawOutline();
			drawBarrel();
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

void MotherTurretHazard::poseDraw() const {
	//TODO: adjust so drawBody will only draw with the normal color?
	drawBody();
	drawOutline();
	drawBarrel();
}

void MotherTurretHazard::poseDraw(DrawingLayers layer) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for MotherTurretHazard::poseDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBody();
			drawOutline();
			drawBarrel();
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

void MotherTurretHazard::ghostDraw(float alpha) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	drawShootingTimer(alpha); //TODO: should this happen?
	drawBody(alpha);
	drawOutline(alpha);
	drawBarrel(alpha);
}

void MotherTurretHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for MotherTurretHazard::ghostDraw!" << std::endl;
		case DrawingLayers::normal:
			drawShootingTimer(alpha); //TODO: should this happen?
			drawBody(alpha);
			drawOutline(alpha);
			drawBarrel(alpha);
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

inline void MotherTurretHazard::drawBody(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	ColorValueHolder color = getColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*va, *ib, *shader);
}

inline void MotherTurretHazard::drawOutline(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	glLineWidth(1.0f);

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void MotherTurretHazard::drawBarrel(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	glLineWidth(2.0f);

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, 1, velocity.getAngle(), x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void MotherTurretHazard::drawShootingTimer(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	if (currentState != 1) {
		return;
	}

	double shootingOutlinePercent;
	if (tickCycle <= 0) {
		shootingOutlinePercent = 0;
	} else {
		shootingOutlinePercent = constrain<double>(targetingCount / (stateMultiplier[1] * tickCycle), 0, 1);
	}
	unsigned int shootingOutlineVertices = Circle::numOfSides * shootingOutlinePercent;

	if (shootingOutlineVertices > 0) {
		ColorValueHolder color = ColorValueHolder(1.0f, 1.0f, 1.0f);
		color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
		shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

		double rotateAngle = velocity.getAngle() + 2*PI*(1 - double(shootingOutlineVertices)/Circle::numOfSides)/2;
		modelMatrix = Renderer::GenerateModelMatrix(r * 5.0/4.0, r * 5.0/4.0, rotateAngle, x, y);
		shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

		Renderer::Draw(*va, *ib, *shader, shootingOutlineVertices*3);
	}
}

CircleHazard* MotherTurretHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData& args) {
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, angle;

	int count = 0;
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);
	}
	if (count >= 1) {
		double* arr = (double*)(args.getDataPortion(0));
		angle = arr[0];
	} else {
		angle = RNG::randFunc() * 2*PI;
	}

	do {
		xpos = RNG::randFunc() * (area_width - 2*TANK_RADIUS*2) + (x_start + TANK_RADIUS*2);
		ypos = RNG::randFunc() * (area_height - 2*TANK_RADIUS*2) + (y_start + TANK_RADIUS*2);
		CircleHazard* testMotherTurret = new MotherTurretHazard(xpos, ypos, angle);
		if (testMotherTurret->reasonableLocation()) {
			randomized = testMotherTurret;
			break;
		} else {
			delete testMotherTurret;
		}
		attempts++;
	} while (attempts < 128);

	return randomized;
}
