#pragma once
#include "collisionhandler.h"
#include "circle.h"
#include "rect.h"
#include <math.h>
#include "constants.h"
#include <iostream>
using namespace std;

bool CollisionHandler::partiallyCollided(Rect* a, Rect* b) {
	return ((a->x + a->w >= b->x) && (a->x- a->w <= b->x + b->w) && (a->y+ a->h >= b->y) && (a->y - a->h <= b->y + b->h));
}

bool CollisionHandler::partiallyCollided(Rect* a, Circle* b) {
	if ((b->x < a->x) && (b->y < a->y)) { //bottom left
		return cornerCollided(b, a->x, a->y);
	} else if ((b->x > (a->x + a->w)) && (b->y < a->y)) { //bottom right
		return cornerCollided(b, (a->x + a->w), a->y);
	} else if ((b->x < a->x) && (b->y > (a->y + a->h))) { //top left
		return cornerCollided(b, a->x, (a->y + a->h));
	} else if ((b->x > (a->x + a->w)) && (b->y > (a->y + a->h))) { //top right
		return cornerCollided(b, (a->x + a->w), (a->y + a->h));
	} else {
		return (((b->x + b->r) >= a->x) && ((b->x - b->r) <= (a->x + a->w)) && ((b->y + b->r) >= a->y) && ((b->y - b->r) <= (a->y + a->h)));
	}
}
//bool CollisionHandler::partiallyCollided(Circle* a, Rect* b);

bool CollisionHandler::partiallyCollided(Circle* a, Circle* b) {
	if ((abs(a->x - b->x) <= a->r + b->r) && (abs(a->y - b->y) <= a->r + b->r)) {
		return (sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)) <= a->r + b->r);
	}
	return false;
}

bool CollisionHandler::cornerCollided(Circle* a, double x, double y) { //effectively C-C but C2->r = 0
	if ((abs(x - a->x) <= a->r) && (abs(y - a->y) <= a->r)) {
		double d = sqrt(pow(x - a->x, 2) + pow(y - a->y, 2));
		return (d <= a->r);
	}
	return false;
}

bool CollisionHandler::fullyCollided(Rect* a, Rect* b) {
	return ((a->x >= b->x) && ((a->x + a->w) <= (b->x + b->w)) && (a->y >= b->y) && ((a->y + a->h) <= (b->y + b->h)));
}
bool CollisionHandler::fullyCollided(Rect* a, Circle* b) { //rectangle inside circle
	if ((a->x >= (b->x - b->r)) && ((a->x + a->w) <= (b->x + b->r)) && (a->y >= (b->y - b->r)) && ((a->y + a->h) <= (b->y + b->r))) { //check R-R collision
		return ((sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)) <= b->r) && //check distance between each corner to circle center
		        (sqrt(pow(a->x - b->x, 2) + pow((a->y + a->h) - b->y, 2)) <= b->r) &&
		        (sqrt(pow((a->x + a->w) - b->x, 2) + pow(a->y - b->y, 2)) <= b->r) &&
		        (sqrt(pow((a->x + a->w) - b->x, 2) + pow((a->y + a->h) - b->y, 2)) <= b->r));
	}
	return false;
}
bool CollisionHandler::fullyCollided(Circle* a, Rect* b) { //circle inside rectangle
	return (((a->x - a->r) >= b->x) && ((a->x + a->r) <= (b->x + b->w)) && ((a->y - a->r) >= b->y) && ((a->y + a->r) <= (b->y + b->h)));
}
bool CollisionHandler::fullyCollided(Circle* a, Circle* b) {
	if (((a->x - a->r) >= (b->x - b->r)) && ((a->x + a->r) <= (b->x + b->r)) && ((a->y - a->r) >= (b->y - b->r)) && ((a->y + a->r) <= (b->y + b->r))) { //check R-R collision
		return (sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)) <= a->r + b->r);
	}
	return false;
}



//these should only be called if collision was checked (and true) first; they literally move each object to the other's edge
void CollisionHandler::pushMovableAwayFromImmovable(Rect* movable, Rect* immovable) {
	if ((movable->y + movable->h / 2) - immovable->y <= (immovable->h / immovable->w) * ((movable->x + movable->w / 2) - immovable->x)) { //I think this is top right
		if ((movable->y + movable->h / 2) - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * ((movable->x + movable->w / 2) - immovable->x)) { //top
			movable->y = immovable->y - movable->h;
		} else { //right
			movable->x = immovable->x + immovable->w;
		}
	}
	else { //bottom left?
		if ((movable->y + movable->h / 2) - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * ((movable->x + movable->w / 2) - immovable->x)) { //left
			movable->x = immovable->x - movable->w;
		} else { //bottom
			movable->y = immovable->y + immovable->h;
		}
	}

	/*
	if (movable->x <= immovable->x) {
		movable->x = immovable->x - movable->w;
	}
	else if((movable->x + movable->w) >= (immovable->x + immovable->w)){
		movable->x = immovable->x + immovable->w;
	}
	else { //movable is inside immovable
		if (abs(movable->x - (immovable->x + immovable->w / 2)) >= abs((movable->x + movable->w / 2) - (immovable->x + immovable->w / 2))) { //left side of movable further from center of immmovable
			movable->x = immovable->x - movable->w;
		}
		else {
			movable->x = immovable->x + immovable->w;
		}
	}

	if (movable->y <= immovable->y) {
		movable->y = immovable->y - movable->h;
	}
	else if ((movable->y + movable->h) >= (immovable->y + immovable->h)) {
		movable->y = immovable->y + immovable->h;
	}
	else { //movable is inside immovable
		if (abs(movable->y - (immovable->y + immovable->h / 2)) >= abs((movable->y + movable->h) - (immovable->y + immovable->h / 2))) { //could be simplified to only care about x-x and (x+h)-(x+h) distances
			movable->y = immovable->y - movable->h;
		}
		else {
			movable->y = immovable->y + immovable->h;
		}
	}
	*/
}
void CollisionHandler::pushMovableAwayFromMovable(Rect* movable1, Rect* movable2) {
	if ((movable1->y + movable1->h / 2) - movable2->y <= (movable2->h / movable2->w) * ((movable1->x + movable1->w / 2) - movable2->x)) { //I think this is top right
		if ((movable1->y + movable1->h / 2) - (movable2->y + movable2->h) <= (-movable2->h / movable2->w) * ((movable1->x + movable1->w / 2) - movable2->x)) { //top
			double d = (movable1->y + movable1->h) - movable2->y;
			movable1->y -= d/2;
			movable2->y += d/2;
		} else { //right
			double d = (movable2->x + movable2->w) - movable1->x;
			movable1->x += d/2;
			movable2->x -= d/2;
		}
	} else { //bottom left?
		if ((movable1->y + movable1->h / 2) - (movable2->y + movable2->h) <= (-movable2->h / movable2->w) * ((movable1->x + movable1->w / 2) - movable2->x)) { //left
			double d = (movable1->x + movable1->w) - movable2->x;
			movable1->x -= d/2;
			movable2->x += d/2;
		} else { //bottom
			double d = (movable2->y + movable2->h) - movable1->y;
			movable1->y += d/2;
			movable1->y -= d/2;
		}
	}

	/*
	if (movable1->x <= movable2->x) {
		double d = (movable1->x + movable1->w) - movable2->x;
		movable1->x -= d/2;
		movable2->x += d/2;
	}
	else if ((movable1->x + movable1->w) >= (movable2->x + movable2->w)) {
		double d = (movable2->x + movable2->w) - movable1->x;
		movable1->x += d/2;
		movable2->x -= d/2;
	}
	else { //movable1 is inside movable2
		if (abs(movable1->x - (movable2->x + movable2->w / 2)) >= abs((movable1->x + movable1->w) - (movable2->x + movable2->w / 2))) { //left side of movable1 further from center of immmovable1
			double d = (movable1->x + movable1->w) - movable2->x;
			movable1->x -= d/2;
			movable2->x += d/2;
		}
		else {
			double d = (movable2->x + movable2->w) - movable1->x;
			movable1->x += d/2;
			movable2->x -= d/2;
		}
	}

	if (movable1->y <= movable2->y) {
		double d = (movable1->y + movable1->h) - movable2->y;
		movable1->y -= d/2;
		movable2->y += d/2;
	}
	else if ((movable1->y + movable1->h) >= (movable2->y + movable2->h)) {
		double d = (movable2->y + movable2->h) - movable1->y;
		movable1->y += d/2;
		movable2->y -= d/2;
	}
	else { //movable1 is inside movable2
		if (abs(movable1->y - (movable2->y + movable2->h / 2)) >= abs((movable1->y + movable1->h) - (movable2->y + movable2->h / 2))) { //could be simplified to only care about x-x and (x+h)-(x+h) distances
			double d = (movable1->y + movable1->h) - movable2->y;
			movable1->y -= d/2;
			movable2->y += d/2;
		}
		else {
			double d = (movable2->y + movable2->h) - movable1->y;
			movable1->y += d/2;
			movable2->y -= d/2;
		}
	}
	*/
}

void CollisionHandler::pushMovableAwayFromImmovable(Circle* movable, Rect* immovable) {
	//typically Tank-Wall collision
	
	bool cornerCollided = false;
	if ((movable->x < immovable->x) && (movable->y < immovable->y)) { //circle in bottom left
		cornerPushMovableAwayFromImmovable(movable, immovable->x, immovable->y);
		cornerCollided = true;
	}
	//can't do elses because more than one corner could be in the circle (TODO: really?)
	if ((movable->x > (immovable->x + immovable->w)) && (movable->y < immovable->y)) { //circle in bottom right
		cornerPushMovableAwayFromImmovable(movable, (immovable->x + immovable->w), immovable->y);
		cornerCollided = true;
	}
	if ((movable->x < immovable->x) && (movable->y > (immovable->y + immovable->h))) { //circle in top left
		cornerPushMovableAwayFromImmovable(movable, immovable->x, (immovable->y + immovable->h));
		cornerCollided = true;
	}
	if ((movable->x > (immovable->x + immovable->w)) && (movable->y > (immovable->y + immovable->h))) { //circle in top right
		cornerPushMovableAwayFromImmovable(movable, (immovable->x + immovable->w), (immovable->y + immovable->h));
		cornerCollided = true;
	}

	if (cornerCollided) {
		return;
	}

	//find which side the circle is near
	if (movable->y - immovable->y <= (immovable->h / immovable->w) * (movable->x - immovable->x)) { //I think this is bottom right
		if (movable->y - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * (movable->x - immovable->x)) { //bottom
			movable->y = immovable->y - movable->r;
		} else { //right
			movable->x = immovable->x + immovable->w + movable->r;
		}
	} else { //top left?
		if (movable->y - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * (movable->x - immovable->x)) { //left
			movable->x = immovable->x - movable->r;
		} else { //top
			movable->y = immovable->y + immovable->h + movable->r;
		}
	}
}
//void CollisionHandler::pushMovableAwayFromMovable(Circle* movable1, Rect* movable2);

//these two are weird
void CollisionHandler::pushMovableAwayFromImmovable(Rect* movable, Circle* immovable) {
	//first, find if circle is in outer corner (as opposed to near a side)
	//if on side, R-R stuff
	//else, C-C stuff (if dist < asdf then move, but don't use else)
	
	if ((immovable->x < movable->x) && (immovable->y < movable->y)) { //circle in top left
		cornerPushMovableAwayFromImmovable(movable, immovable, movable->x, movable->y);
	}
	//can't do elses because more than one corner could be in the circle
	if ((immovable->x > (movable->x + movable->w)) && (immovable->y < movable->y)) { //circle in top right
		cornerPushMovableAwayFromImmovable(movable, immovable, (movable->x + movable->w), movable->y);
	}
	if ((immovable->x < movable->x) && (immovable->y > (movable->y + movable->h))) { //circle in bottom left
		cornerPushMovableAwayFromImmovable(movable, immovable, movable->x, (movable->y + movable->h));
	}
	if ((immovable->x > (movable->x + movable->w)) && (immovable->y > (movable->y + movable->h))) { //circle in bottom right
		cornerPushMovableAwayFromImmovable(movable, immovable, (movable->x + movable->w), (movable->y + movable->h));
	}
	
	/*
	if (sqrt(pow(movable->x - immovable->x, 2) + pow(movable->y - immovable->y, 2)) <= immovable->r) { //top left rectangle corner
		double angle = atan2((movable->y - immovable->y), (movable->x - immovable->x));
		double d = immovable->r - sqrt(pow(movable->x - immovable->x, 2) + pow(movable->y - immovable->y, 2));
		movable->y -= sin(angle) * d;
		movable->x -= cos(angle) * d;
	}
	if (sqrt(pow((movable->x + movable->w) - immovable->x, 2) + pow(movable->y - immovable->y, 2)) <= immovable->r) { //top right rectangle corner
		double angle = atan2((movable->y - immovable->y), ((movable->x + movable->w) - immovable->x));
		double d = immovable->r - sqrt(pow((movable->x + movable->w) - immovable->x, 2) + pow(movable->y - immovable->y, 2));
		movable->y -= sin(angle) * d;
		movable->x -= cos(angle) * d;
	}
	if (sqrt(pow(movable->x - immovable->x, 2) + pow((movable->y + movable->h) - immovable->y, 2)) <= immovable->r) { //bottom left rectangle corner
		double angle = atan2(((movable->y + movable->h) - immovable->y), (movable->x - immovable->x));
		double d = immovable->r - sqrt(pow(movable->x - immovable->x, 2) + pow((movable->y + movable->h) - immovable->y, 2));
		movable->y -= sin(angle) * d;
		movable->x -= cos(angle) * d;
	}
	if (sqrt(pow((movable->x + movable->w) - immovable->x, 2) + pow((movable->y + movable->h) - immovable->y, 2)) <= immovable->r) { //bottom right rectangle corner
		double angle = atan2(((movable->y + movable->h) - immovable->y), ((movable->x + movable->w) - immovable->x));
		double d = immovable->r - sqrt(pow((movable->x + movable->w) - immovable->x, 2) + pow((movable->y + movable->h) - immovable->y, 2));
		movable->y -= sin(angle) * d;
		movable->x -= cos(angle) * d;
	}
	*/

	//find which side the circle is near

	if (immovable->y - movable->y <= (movable->h / movable->w) * (immovable->x - movable->x)) { //I think this is top right
		if (immovable->y - (movable->y + movable->h) <= (-movable->h / movable->w) * (immovable->x - movable->x)) { //top
			movable->y = immovable->y - immovable->r - movable->h;
		} else { //right
			movable->x = immovable->x + immovable->r;
		}
	} else { //bottom left?
		if (immovable->y - (movable->y + movable->h) <= (-movable->h / movable->w) * (immovable->x - movable->x)) { //left
			movable->x = immovable->x - immovable->r - movable->w;
		} else { //bottom
			movable->y = immovable->y + immovable->r;
		}
	}



	/*
	if (immovable->y <= movable->y) {
		movable->y = immovable->y + immovable->r;
	}
	else if (immovable->y >= (movable->y + movable->h)) {
		movable->y = immovable->y - immovable->r;
	}
	else if (immovable->x <= movable->x) {
		movable->x = immovable->x + immovable->r;
	}
	else if (immovable->x >= (movable->x + movable->w)) {
		movable->x = immovable->x - immovable->r;
	}
	//else?
	*/
	
}
void CollisionHandler::pushMovableAwayFromMovable(Rect* movable1, Circle* movable2) {
	if ((movable2->x < movable1->x) && (movable2->y < movable1->y)) { //circle in top left
		cornerPushMovableAwayFromMovable(movable1, movable2, movable1->x, movable1->y);
	}
	//can't do elses because more than one corner could be in the circle
	if ((movable2->x >(movable1->x + movable1->w)) && (movable2->y < movable1->y)) { //circle in top right
		cornerPushMovableAwayFromMovable(movable1, movable2, (movable1->x + movable1->w), movable1->y);
	}
	if ((movable2->x < movable1->x) && (movable2->y >(movable1->y + movable1->h))) { //circle in bottom left
		cornerPushMovableAwayFromMovable(movable1, movable2, movable1->x, (movable1->y + movable1->h));
	}
	if ((movable2->x >(movable1->x + movable1->w)) && (movable2->y >(movable1->y + movable1->h))) { //circle in bottom right
		cornerPushMovableAwayFromMovable(movable1, movable2, (movable1->x + movable1->w), (movable1->y + movable1->h));
	}

	if (movable2->y - movable1->y <= (movable1->h / movable1->w) * (movable2->x - movable1->x)) { //I think this is top right
		if (movable2->y - (movable1->y + movable1->h) <= (-movable1->h / movable1->w) * (movable2->x - movable1->x)) { //top
			double d = movable2->y + movable2->r - movable1->y;
			movable2->y -= d/2;
			movable1->y += d/2;
		} else { //right
			double d = (movable1->x + movable1->w) - (movable2->x - movable2->r);
			movable2->x += d/2;
			movable1->x -= d/2;
		}
	} else { //bottom left?
		if (movable2->y - (movable1->y + movable1->h) <= (-movable1->h / movable1->w) * (movable2->x - movable1->x)) { //left
			double d = (movable2->x + movable2->r) - movable1->x;
			movable2->x -= d/2;
			movable1->x += d/2;
		} else { //bottom
			double d = (movable1->y + movable1->h) - (movable2->y - movable2->r);
			movable2->y += d/2;
			movable1->y -= d/2;
		}
	}
}

void CollisionHandler::pushMovableAwayFromImmovable(Circle* movable, Circle* immovable) {
	double angle = atan2((movable->y - immovable->y), (movable->x - immovable->x));
	double d = sqrt(pow(movable->x - immovable->x, 2) + pow(movable->y - immovable->y, 2)) - (movable->r + immovable->r);
	movable->y -= sin(angle) * d;
	movable->x -= cos(angle) * d;
}
void CollisionHandler::pushMovableAwayFromMovable(Circle* movable1, Circle* movable2) {
	double angle = atan2((movable1->y - movable2->y), (movable1->x - movable2->x));
	double d = sqrt(pow(movable1->x - movable2->x, 2) + pow(movable1->y - movable2->y, 2)) - (movable1->r + movable2->r);
	movable1->y -= sin(angle) * d/2;
	movable1->x -= cos(angle) * d/2;
	movable2->y += sin(angle) * d/2;
	movable2->x += cos(angle) * d/2;
}

void CollisionHandler::cornerPushMovableAwayFromImmovable(Circle* movable, double x, double y) {
	double angle = atan2((y - movable->y), (x - movable->x));
	double d = movable->r - sqrt(pow(x - movable->x, 2) + pow(y - movable->y, 2));
	movable->y -= sin(angle) * d;
	movable->x -= cos(angle) * d;
}
void CollisionHandler::cornerPushMovableAwayFromMovable(Circle* movable1, Rect* movable2, double x, double y) {
	double angle = atan2((y - movable1->y), (x - movable1->x));
	double d = movable1->r - sqrt(pow(x - movable1->x, 2) + pow(y - movable1->y, 2));
	movable1->y -= sin(angle) * d/2;
	movable1->x -= cos(angle) * d/2;
	movable2->y += sin(angle) * d/2;
	movable2->x += cos(angle) * d/2;
}
void CollisionHandler::cornerPushMovableAwayFromImmovable(Rect* movable, Circle* immovable, double x, double y) {
	double angle = atan2((immovable->y - y), (immovable->x - x));
	double d = immovable->r - sqrt(pow(x - immovable->x, 2) + pow(y - immovable->y, 2));
	movable->y -= sin(angle) * d;
	movable->x -= cos(angle) * d;
}
//void CollisionHandler::cornerPushMovableAwayFromMovable(Rect* movable1, Circle* movable2, double x, double y);














/*
assuming origin is top left, not bottom left:

#pragma once
#include "collisionhandler.h"
#include "circle.h"
#include "rect.h"
#include <math.h>
#include "constants.h"
#include <iostream>
using namespace std;

bool CollisionHandler::partiallyCollided(Rect* a, Rect* b) {
return ((a->x + a->w >= b->x) && (a->x- a->w <= b->x + b->w) && (a->y+ a->h >= b->y) && (a->y - a->h <= b->y + b->h));
}

bool CollisionHandler::partiallyCollided(Rect* a, Circle* b) {
if ((b->x < a->x) && (b->y < a->y)) { //top left
return cornerCollided(b, a->x, a->y);
} else if ((b->x > (a->x + a->w)) && (b->y < a->y)) { //top right
return cornerCollided(b, (a->x + a->w), a->y);
} else if ((b->x < a->x) && (b->y > (a->y + a->h))) { //bottom left
return cornerCollided(b, a->x, (a->y + a->h));
} else if ((b->x > (a->x + a->w)) && (b->y > (a->y + a->h))) { //bottom right
return cornerCollided(b, (a->x + a->w), (a->y + a->h));
} else {
return (((b->x + b->r) >= a->x) && ((b->x - b->r) <= (a->x + a->w)) && ((b->y + b->r) >= a->y) && ((b->y - b->r) <= (a->y + a->h)));
}
}
//bool CollisionHandler::partiallyCollided(Circle* a, Rect* b);

bool CollisionHandler::partiallyCollided(Circle* a, Circle* b) {
cout << a->x << " " << b->x << endl;
//std::cout << (abs(a->x - b->x) <= a->r + b->r) << " " << (abs(a->y - b->y) <= a->r + b->r) << std::endl;
if ((abs(a->x - b->x) <= a->r + b->r) && (abs(a->y - b->y) <= a->r + b->r)) {
return (sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)) <= a->r + b->r);
}
return false;
}

bool CollisionHandler::cornerCollided(Circle* a, double x, double y) { //effectively C-C but C2->r=0
if ((abs(x - a->x) <= a->r) && (abs(y - a->y) <= a->r)) {
double d = sqrt(pow(x - a->x, 2) + pow(y - a->y, 2));
return (d <= a->r);
}
return false;
}

bool CollisionHandler::fullyCollided(Rect* a, Rect* b) {
return ((a->x >= b->x) && ((a->x + a->w) <= (b->x + b->w)) && (a->y >= b->y) && ((a->y + a->h) <= (b->y + b->h)));
}
bool CollisionHandler::fullyCollided(Rect* a, Circle* b) { //rectangle inside circle
if ((a->x >= (b->x - b->r)) && ((a->x + a->w) <= (b->x + b->r)) && (a->y >= (b->y - b->r)) && ((a->y + a->h) <= (b->y + b->r))) { //check R-R collision
return ((sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)) <= b->r) && //check distance between each corner to circle center
(sqrt(pow(a->x - b->x, 2) + pow((a->y + a->h) - b->y, 2)) <= b->r) &&
(sqrt(pow((a->x + a->w) - b->x, 2) + pow(a->y - b->y, 2)) <= b->r) &&
(sqrt(pow((a->x + a->w) - b->x, 2) + pow((a->y + a->h) - b->y, 2)) <= b->r));
}
return false;
}
bool CollisionHandler::fullyCollided(Circle* a, Rect* b) { //circle inside rectangle
return (((a->x - a->r) >= b->x) && ((a->x + a->r) <= (b->x + b->w)) && ((a->y - a->r) >= b->y) && ((a->y + a->r) <= (b->y + b->h)));
}
bool CollisionHandler::fullyCollided(Circle* a, Circle* b) {
if (((a->x - a->r) >= (b->x - b->r)) && ((a->x + a->r) <= (b->x + b->r)) && ((a->y - a->r) >= (b->y - b->r)) && ((a->y + a->r) <= (b->y + b->r))) { //check R-R collision
return (sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)) <= a->r + b->r);
}
return false;
}



//these should only be called if collision was checked (and true) first; they literally move each object to the other's edge
void CollisionHandler::pushMovableAwayFromImmovable(Rect* movable, Rect* immovable) {
if ((movable->y + movable->h / 2) - immovable->y <= (immovable->h / immovable->w) * ((movable->x + movable->w / 2) - immovable->x)) { //I think this is top right
if ((movable->y + movable->h / 2) - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * ((movable->x + movable->w / 2) - immovable->x)) { //top
movable->y = immovable->y - movable->h;
}
else { //right
movable->x = immovable->x + immovable->w;
}
}
else { //bottom left?
if ((movable->y + movable->h / 2) - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * ((movable->x + movable->w / 2) - immovable->x)) { //left
movable->x = immovable->x - movable->w;
}
else { //bottom
movable->y = immovable->y + immovable->h;
}
}
}
void CollisionHandler::pushMovableAwayFromMovable(Rect* movable1, Rect* movable2) {
	if ((movable1->y + movable1->h / 2) - movable2->y <= (movable2->h / movable2->w) * ((movable1->x + movable1->w / 2) - movable2->x)) { //I think this is top right
		if ((movable1->y + movable1->h / 2) - (movable2->y + movable2->h) <= (-movable2->h / movable2->w) * ((movable1->x + movable1->w / 2) - movable2->x)) { //top
			double d = (movable1->y + movable1->h) - movable2->y;
			movable1->y -= d / 2;
			movable2->y += d / 2;
		}
		else { //right
			double d = (movable2->x + movable2->w) - movable1->x;
			movable1->x += d / 2;
			movable2->x -= d / 2;
		}
	}
	else { //bottom left?
		if ((movable1->y + movable1->h / 2) - (movable2->y + movable2->h) <= (-movable2->h / movable2->w) * ((movable1->x + movable1->w / 2) - movable2->x)) { //left
			double d = (movable1->x + movable1->w) - movable2->x;
			movable1->x -= d / 2;
			movable2->x += d / 2;
		}
		else { //bottom
			double d = (movable2->y + movable2->h) - movable1->y;
			movable1->y += d / 2;
			movable1->y -= d / 2;
		}
	}
}

void CollisionHandler::pushMovableAwayFromImmovable(Circle* movable, Rect* immovable) {
	//typically Tank-Wall collision

	if ((movable->x < immovable->x) && (movable->y < immovable->y)) { //circle in top left
		cornerPushMovableAwayFromImmovable(movable, immovable->x, immovable->y);
	}
	//can't do elses because more than one corner could be in the circle
	if ((movable->x >(immovable->x + immovable->w)) && (movable->y < immovable->y)) { //circle in top right
		cornerPushMovableAwayFromImmovable(movable, (immovable->x + immovable->w), immovable->y);
	}
	if ((movable->x < immovable->x) && (movable->y >(immovable->y + immovable->h))) { //circle in bottom left
		cornerPushMovableAwayFromImmovable(movable, immovable->x, (immovable->y + immovable->h));
	}
	if ((movable->x >(immovable->x + immovable->w)) && (movable->y >(immovable->y + immovable->h))) { //circle in bottom right
		cornerPushMovableAwayFromImmovable(movable, (immovable->x + immovable->w), (immovable->y + immovable->h));
	}

	//find which side the circle is near
	if (movable->y - immovable->y <= (immovable->h / immovable->w) * (movable->x - immovable->x)) { //I think this is top right
		if (movable->y - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * (movable->x - immovable->x)) { //top
			movable->y = immovable->y - movable->r;
		}
		else { //right
			movable->x = immovable->x + immovable->w + movable->r;
		}
	}
	else { //bottom left?
		if (movable->y - (immovable->y + immovable->h) <= (-immovable->h / immovable->w) * (movable->x - immovable->x)) { //left
			movable->x = immovable->x - movable->r;
		}
		else { //bottom
			movable->y = immovable->y + immovable->h + movable->r;
		}
	}
}
//void CollisionHandler::pushMovableAwayFromMovable(Circle* movable1, Rect* movable2);

//these two are weird
void CollisionHandler::pushMovableAwayFromImmovable(Rect* movable, Circle* immovable) {
	//first, find if circle is in outer corner (as opposed to near a side)
	//if on side, R-R stuff
	//else, C-C stuff (if dist < asdf then move, but don't use else)

	if ((immovable->x < movable->x) && (immovable->y < movable->y)) { //circle in top left
		cornerPushMovableAwayFromImmovable(movable, immovable, movable->x, movable->y);
	}
	//can't do elses because more than one corner could be in the circle
	if ((immovable->x >(movable->x + movable->w)) && (immovable->y < movable->y)) { //circle in top right
		cornerPushMovableAwayFromImmovable(movable, immovable, (movable->x + movable->w), movable->y);
	}
	if ((immovable->x < movable->x) && (immovable->y >(movable->y + movable->h))) { //circle in bottom left
		cornerPushMovableAwayFromImmovable(movable, immovable, movable->x, (movable->y + movable->h));
	}
	if ((immovable->x >(movable->x + movable->w)) && (immovable->y >(movable->y + movable->h))) { //circle in bottom right
		cornerPushMovableAwayFromImmovable(movable, immovable, (movable->x + movable->w), (movable->y + movable->h));
	}

	//find which side the circle is near

	if (immovable->y - movable->y <= (movable->h / movable->w) * (immovable->x - movable->x)) { //I think this is top right
		if (immovable->y - (movable->y + movable->h) <= (-movable->h / movable->w) * (immovable->x - movable->x)) { //top
			movable->y = immovable->y - immovable->r - movable->h;
		}
		else { //right
			movable->x = immovable->x + immovable->r;
		}
	}
	else { //bottom left?
		if (immovable->y - (movable->y + movable->h) <= (-movable->h / movable->w) * (immovable->x - movable->x)) { //left
			movable->x = immovable->x - immovable->r - movable->w;
		}
		else { //bottom
			movable->y = immovable->y + immovable->r;
		}
	}

}
void CollisionHandler::pushMovableAwayFromMovable(Rect* movable1, Circle* movable2) {
	if ((movable2->x < movable1->x) && (movable2->y < movable1->y)) { //circle in top left
		cornerPushMovableAwayFromMovable(movable1, movable2, movable1->x, movable1->y);
	}
	//can't do elses because more than one corner could be in the circle
	if ((movable2->x >(movable1->x + movable1->w)) && (movable2->y < movable1->y)) { //circle in top right
		cornerPushMovableAwayFromMovable(movable1, movable2, (movable1->x + movable1->w), movable1->y);
	}
	if ((movable2->x < movable1->x) && (movable2->y >(movable1->y + movable1->h))) { //circle in bottom left
		cornerPushMovableAwayFromMovable(movable1, movable2, movable1->x, (movable1->y + movable1->h));
	}
	if ((movable2->x >(movable1->x + movable1->w)) && (movable2->y >(movable1->y + movable1->h))) { //circle in bottom right
		cornerPushMovableAwayFromMovable(movable1, movable2, (movable1->x + movable1->w), (movable1->y + movable1->h));
	}

	if (movable2->y - movable1->y <= (movable1->h / movable1->w) * (movable2->x - movable1->x)) { //I think this is top right
		if (movable2->y - (movable1->y + movable1->h) <= (-movable1->h / movable1->w) * (movable2->x - movable1->x)) { //top
			double d = movable2->y + movable2->r - movable1->y;
			movable2->y -= d / 2;
			movable1->y += d / 2;
		}
		else { //right
			double d = (movable1->x + movable1->w) - (movable2->x - movable2->r);
			movable2->x += d / 2;
			movable1->x -= d / 2;
		}
	}
	else { //bottom left?
		if (movable2->y - (movable1->y + movable1->h) <= (-movable1->h / movable1->w) * (movable2->x - movable1->x)) { //left
			double d = (movable2->x + movable2->r) - movable1->x;
			movable2->x -= d / 2;
			movable1->x += d / 2;
		}
		else { //bottom
			double d = (movable1->y + movable1->h) - (movable2->y - movable2->r);
			movable2->y += d / 2;
			movable1->y -= d / 2;
		}
	}
}

void CollisionHandler::pushMovableAwayFromImmovable(Circle* movable, Circle* immovable) {
	double angle = atan2((movable->y - immovable->y), (movable->x - immovable->x));
	double d = sqrt(pow(movable->x - immovable->x, 2) + pow(movable->y - immovable->y, 2)) - (movable->r + immovable->r);
	movable->y -= sin(angle) * d;
	movable->x -= cos(angle) * d;
}
void CollisionHandler::pushMovableAwayFromMovable(Circle* movable1, Circle* movable2) {
	double angle = atan2((movable1->y - movable2->y), (movable1->x - movable2->x));
	double d = sqrt(pow(movable1->x - movable2->x, 2) + pow(movable1->y - movable2->y, 2)) - (movable1->r + movable2->r);
	movable1->y -= sin(angle) * d / 2;
	movable1->x -= cos(angle) * d / 2;
	movable2->y += sin(angle) * d / 2;
	movable2->x += cos(angle) * d / 2;
}

void CollisionHandler::cornerPushMovableAwayFromImmovable(Circle* movable, double x, double y) {
	double angle = atan2((y - movable->y), (x - movable->x));
	double d = movable->r - sqrt(pow(x - movable->x, 2) + pow(y - movable->y, 2));
	movable->y -= sin(angle) * d;
	movable->x -= cos(angle) * d;
}
void CollisionHandler::cornerPushMovableAwayFromMovable(Circle* movable1, Rect* movable2, double x, double y) {
	double angle = atan2((y - movable1->y), (x - movable1->x));
	double d = movable1->r - sqrt(pow(x - movable1->x, 2) + pow(y - movable1->y, 2));
	movable1->y -= sin(angle) * d / 2;
	movable1->x -= cos(angle) * d / 2;
	movable2->y += sin(angle) * d / 2;
	movable2->x += cos(angle) * d / 2;
}
void CollisionHandler::cornerPushMovableAwayFromImmovable(Rect* movable, Circle* immovable, double x, double y) {
	double angle = atan2((immovable->y - y), (immovable->x - x));
	double d = immovable->r - sqrt(pow(x - immovable->x, 2) + pow(y - immovable->y, 2));
	movable->y -= sin(angle) * d;
	movable->x -= cos(angle) * d;
}
//void CollisionHandler::cornerPushMovableAwayFromMovable(Rect* movable1, Circle* movable2, double x, double y);

*/
