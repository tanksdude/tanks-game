#pragma once
#include "tank.h"
#include "constants.h"
#include "colormixer.h"
#include <math.h>
#include <string>
#include <iostream>

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

Tank::Tank(double x_, double y_, double a, char id_, std::string name_) {
	x = x_;
	y = y_;
	angle = a;
	id = id_;
	r = 16;
	name = name_;
}

Tank::Tank() {
	x = -100;
	y = -100;
	angle = 0;
	id = -1;
	r = 16;
	name = "";
}

void Tank::move() {
	//TODO: go through each power in the power vector, then use the function that it points to for movement; if none modified movement, use this default

	//if (!forward || !document.getElementById("moveturn").checked) { //change && to || and remove second ! to flip playstyle
		if (turnL) {
			angle += PI / turningIncrement;
		}
		if (turnR) {
			angle -= PI / turningIncrement;
		}
	//}
	//if (!document.getElementById("acceleration").checked) {
		if (forward)
			velocity += acceleration;
		else
			velocity -= acceleration; //can result in negative velocities, but that's okay, altered in terminalVelocity()
		terminalVelocity();
	//}
	/*else {
	if (forward)
	velocity = maxSpeed;
	else
	velocity = 0;
	}*/

	x += cos(angle) * velocity;
	y += sin(angle) * velocity;
}

void Tank::terminalVelocity() {
	if (velocity > maxSpeed + acceleration) {
		velocity -= acceleration;
		if (forward && velocity > maxSpeed) //so the tank doesn't stay at a high velocity if it loses its ability to go as fast as it previously could
			velocity -= acceleration;
	}
	else if (velocity > maxSpeed)
		velocity = maxSpeed;
	else if (velocity < 0)
		velocity = 0;
}

void Tank::shoot() {
	//TODO: fix so it can handle multiple shooting cooldowns(?)
	
	if(shootCount > 0) //check isn't really needed, but it also doesn't decrease performance by a real amount
		shootCount--;

	if(shooting && shootCount <= 0){
		std::vector<BulletPower*> bp;
		for (int i = 0; i < tankPowers.size(); i++) {
			bp.push_back(tankPowers[i]->makeBulletPower());
		}
		bullets.push_back(new Bullet(x + r*cos(angle), y + r*sin(angle), r/4, angle, maxSpeed*2, id, bp)); //should be maxSpeed*4
		shootCount = maxShootCount;
	}

	//if (shooting && shootCount <= 0) {
		//figure it out later


		/*
		if (this.specialShooting)
			for (let i = 0; i<this.power.type.length; i++)
				tankShooting[this.power.type[i]](this.index);
		else
			shots.push(new Shot(this.xpos + Math.cos(this.angle)*this.size, this.ypos + Math.sin(this.angle)*this.size, this.angle, this.maxSpeed*this.shotVelocityMultiplier, 0, 0, 0, this.index, this.size / 4 * this.shotSizeMultiplier, this.shootingPower)); //normal
		*/
		//this.shootCount = shootSpeed*this.shootMultiplier;
	//}
}

void Tank::powerCalculate() {
	for (int i = tankPowers.size() - 1; i >= 0; i--) {
		tankPowers[i]->tick(); //I don't think any power will use this, but whatever
		if (tankPowers[i]->isDone()) {
			removePower(i);
		} else { //to make each power last its full length, not n-1 length
			tankPowers[i]->powerTick();
		}
	}
}

void Tank::removePower(int index) {
	tankPowers[index]->removeEffects(this);
	delete tankPowers[index];
	tankPowers.erase(tankPowers.begin() + index);
}

void Tank::powerReset() {
	for (int i = tankPowers.size() - 1; i >= 0; i--) {
		removePower(i);
	}
}

ColorValueHolder Tank::getBodyColor() {
	if (tankPowers.size() == 0) {
		return defaultColor;
	}
	else {
		return ColorMixer::mix(tankPowers);
	}
}

void Tank::draw() {
	//TODO: need ability for more special drawing
	draw(x, y);
}

void Tank::draw(double xpos, double ypos) {

	//shooting cooldown outline:
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);

	glVertex3f(xpos, ypos, 0);
	for (int i = 0; i < Circle::numOfSides, (double)i / Circle::numOfSides < shootCount/maxShootCount; i++) {
		glVertex3f(xpos + r*cos(i * 2*PI / Circle::numOfSides + angle) * 5/4, ypos + r*sin(i * 2*PI / Circle::numOfSides + angle) * 5/4, 0);
	}
	glVertex3f(xpos, ypos, 0);

	glEnd();

	//power cooldown outlines:
	for (int i = 0; i < tankPowers.size(); i++) {
		ColorValueHolder c = tankPowers[i]->getColor();

		glColor3f(c.getRf(), c.getGf(), c.getBf());
		glBegin(GL_POLYGON);

		glVertex3f(xpos, ypos, 0);
		for (int j = 0; j < Circle::numOfSides, (double)j / Circle::numOfSides < tankPowers[i]->timeLeft / tankPowers[i]->maxTime; j++) {
			glVertex3f(xpos + r*cos(j * 2*PI / Circle::numOfSides + angle) * 9/8, ypos + r*sin(j * 2*PI / Circle::numOfSides + angle) * 9/8, 0);
		}
		glVertex3f(xpos, ypos, 0);

		glEnd();
	}

	//main body:
	ColorValueHolder color = getBodyColor();
	glColor3f(color.getRf(), color.getGf(), color.getBf());
	
	glBegin(GL_POLYGON);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex3f(xpos + r*cos(i * 2*PI / Circle::numOfSides), ypos + r*sin(i * 2*PI / Circle::numOfSides), 0);
	}

	glEnd();

	//outline:
	glColor3f(0, 0, 0);
	glLineWidth(2);

	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex2f(xpos + r*cos(i * 2 * PI / Circle::numOfSides), ypos + r*sin(i * 2 * PI / Circle::numOfSides));
	}

	glEnd();

	//barrel:
	glColor3f(0, 0, 0);
	glLineWidth(2);

	glBegin(GL_LINES);

	glVertex2f(x, y);
	glVertex2f(x + r*cos(angle), y + r*sin(angle));

	glEnd();

	//if (shootCount > 0) { //shooting countdown
		

		/*
		canvas.beginPath();
		canvas.arc(this.xpos*multiplier, yp*multiplier, (this.size + 4)*multiplier, this.angle, Math.PI * 2 * this.shootCount / (shootSpeed*this.shootMultiplier) + this.angle); // *5/4
		canvas.lineTo(this.xpos*multiplier, yp*multiplier);
		canvas.fillStyle = defaultColor; //getColor()
		canvas.fill();
		*/
	//}
	if (false /*this.power.color.length>0*/) { //power-up countdown
		

		/*
		let powerOrder = this.powerCount.slice(0); //sort the power colors from greatest amount left to least amount left (then reverse)
		for (let i = 0; i<powerOrder.length; i++)
			powerOrder[i] /= this.maxPowerCount[i];
		powerOrder = powerCountSort(powerOrder);
		//for(let i=0; i<powerOrder[0].length; i++)
		//powerOrder[0][i]*=this.maxPowerCount[powerOrder[1][i]];
		for (let i = powerOrder[0].length - 1; i >= 0; i--) { //drawn in reverse order, was done so the power countdowns don't overlap others so you can see all countdowns
			canvas.beginPath();
			canvas.arc(this.xpos*multiplier, yp*multiplier, (this.size + 2)*multiplier, this.angle, Math.PI * 2 * powerOrder[0][i]/(0&this.maxPowerCount[powerOrder[1][i]] +1) + this.angle); // *9/8
			canvas.lineTo(this.xpos*multiplier, yp*multiplier);
			canvas.fillStyle = this.power.color[powerOrder[1][i]];
			canvas.fill();
		}
		*/
	}

	/*
	canvas.beginPath(); //tank body
	canvas.arc(this.xpos*multiplier, yp*multiplier, this.size*multiplier, 0, Math.PI * 2);

	//not used:
	if(document.getElementById("powerFade").checked){
	if(this.power.rainbow)
	canvas.fillStyle=colorMix(this.colorDefault,level.getRainbow(),this.powerCount[this.power.type.indexOf(18)]/this.maxPowerCount[this.power.type.indexOf(18));
	else if(this.power.color.length>0){
	let colors=[];
	for(let i=0; i<this.power.color.length; i++)
	colors.push(colorMix(this.colorDefault,this.power.color[i],this.powerCount[i]/this.maxPowerCount[i]));
	canvas.fillStyle=colorCombine(colors);
	}else
	canvas.fillStyle=this.colorDefault;
	}else{
	//end not used

	if (this.power.rainbow)
		canvas.fillStyle = level.getRainbow();
	else if (this.power.color.length>0)
		canvas.fillStyle = colorCombine(this.power.color);
	else
		canvas.fillStyle = this.colorDefault;
	//}
	canvas.fill();
	*/

	/*
	if (!performanceMode && this.power.type.length>0 && !this.power.rainbow) //extra lines
		for (let i = 0; i<this.power.type.length; i++)
			extraTankDrawings[this.power.type[i]](this.xpos, yp, this.size, this.angle);
	*/

	
}

void Tank::drawName() {
	drawName(x, y);
}

void Tank::drawName(double xpos, double ypos) {
	if (name.size() == 0)
		return;

	const double scaleFactor = 6;

	glPushMatrix();

	double totalWidth = 0;
	double* widths = new double[name.size()];
	double fillWidth;
	for (int i = 0; i < name.size(); i++) {
		totalWidth += glutStrokeWidth(GLUT_STROKE_ROMAN, name[i]);
		widths[i] = glutStrokeWidth(GLUT_STROKE_ROMAN, name[i]);
	}
	fillWidth = totalWidth/scaleFactor;

	double height = glutStrokeHeight(GLUT_STROKE_ROMAN);

	double center;
	if(xpos - fillWidth/2 < 0){
		center = fillWidth/2;
	}else if(xpos + fillWidth/2 > GAME_WIDTH){
		center = GAME_WIDTH - fillWidth/2;
	}else{
		center = xpos;
	}

	//TODO: add ability for drawName to just draw the name, no adjustment based on y-pos (probably just need a boolean in the parameters)
	double centerY;
	if(ypos + r + 8 + height/scaleFactor > GAME_HEIGHT){
		centerY = ypos - r - 8 - height/scaleFactor;
	} else {
		centerY = ypos + r + 8;
	}
	glTranslatef(center - fillWidth/2, centerY, 0);
	glScalef(1.0/scaleFactor, 1.0/scaleFactor, 1.0/scaleFactor);
	for (int i = 0; i < name.size(); i++) {
		glColor3f(defaultNameStroke.getRf(), defaultNameStroke.getGf(), defaultNameStroke.getBf());
		glLineWidth(8);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, name[i]);

		glTranslatef(-widths[i], 0, 0);

		glColor3f(defaultNameFill.getRf(), defaultNameFill.getGf(), defaultNameFill.getBf());
		glLineWidth(2);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, name[i]);
	}

	delete[] widths;

	glPopMatrix();

}

void Tank::resetThings(double x, double y, double a, char id, std::string name) {
	this->x = x;
	this->y = y;
	this->angle = a;
	this->id = id;
	this->name = name;
	shootCount = 0;

	this->powerReset();
}

void Tank::edgeConstrain() {
	if (x + r > GAME_WIDTH) {
		x = GAME_WIDTH - r;
	}
	else if (x - r < 0) {
		x = r;
	}
	if (y + r > GAME_HEIGHT) {
		y = GAME_HEIGHT - r;
	}
	else if (y - r < 0) {
		y = r;
	}
	//technically, checking down before up (and left before right) would probably have a slight efficiency increase, but it would be extremely (negligible) small
}

bool Tank::isPartiallyOutOfBounds() {
	return ((x + r > GAME_WIDTH) || (x - r < 0) || (y + r > GAME_HEIGHT) || (y - r < 0));
} //doesn't care if touching edge

bool Tank::isFullyOutOfBounds() {
	return ((x - r > GAME_WIDTH) || (x + r < 0) || (y - r > GAME_HEIGHT) || (y + r < 0));
}

Tank::~Tank() {
	//delete explosionColor;
	for (int i = 0; i < tankPowers.size(); i++) {
		delete tankPowers[i];
	}

	tankPowers.clear(); //don't know if this is needed
}
