#pragma once
#include "circle.h"
#include "rect.h"
#include "doublepositionholder.h"

class CollisionHandler {
private:
	static bool cornerCollided(Circle*, double x, double y);
	static bool cornerCollidedIgnoreEdge(Circle*, double x, double y);
	static void cornerPushMovableAwayFromImmovable(Circle* movable, double x, double y);
	static void cornerPushMovableAwayFromMovable(Circle* movable1, Rect* movable2, double x, double y);
	static void cornerPushMovableAwayFromImmovable(Rect* movable, Circle*, double x, double y);
	static void cornerPushMovableAwayFromMovable(Rect* movable1, Circle* movable2, double x, double y) {
		CollisionHandler::cornerPushMovableAwayFromMovable(movable2, movable1, x, y);
	}

public: //for tanks and bullets and stuff (things that move around)
	static bool partiallyOutOfBounds(Rect*);
	static bool partiallyOutOfBoundsIgnoreEdge(Rect*);
	static bool fullyOutOfBounds(Rect*);
	static bool fullyOutOfBoundsIgnoreEdge(Rect*);

	//below: only does simple check, treating the circle like a square
	static bool partiallyOutOfBounds(Circle*);
	static bool partiallyOutOfBoundsIgnoreEdge(Circle*);
	static bool fullyOutOfBounds(Circle*);
	static bool fullyOutOfBoundsIgnoreEdge(Circle*);

	static void edgeConstrain(Rect*);
	static void edgeConstrain(Circle*);

public: //collision detection and handling (just moving)
	static bool partiallyCollided(Rect*, Rect*);
	static bool partiallyCollided(Rect*, Circle*);
	static bool partiallyCollided(Circle* c, Rect* r) {
		return CollisionHandler::partiallyCollided(r, c);
	}
	static bool partiallyCollided(Circle*, Circle*);

	static bool partiallyCollidedIgnoreEdge(Rect*, Rect*);
	static bool partiallyCollidedIgnoreEdge(Rect*, Circle*);
	static bool partiallyCollidedIgnoreEdge(Circle* c, Rect* r) {
		return CollisionHandler::partiallyCollidedIgnoreEdge(r, c);
	}
	static bool partiallyCollidedIgnoreEdge(Circle*, Circle*);

	static bool fullyCollided(Rect*, Rect*);
	static bool fullyCollided(Rect*, Circle*);
	static bool fullyCollided(Circle* c, Rect* r); //different
	static bool fullyCollided(Circle*, Circle*);

	static void pushMovableAwayFromImmovable(Rect* movable, Rect* immovable);
	static void pushMovableAwayFromMovable(Rect* movable1, Rect* movable2);

	static void pushMovableAwayFromImmovable(Circle* movable, Rect* immovable);
	static void pushMovableAwayFromMovable(Circle* movable1, Rect* movable2) {
		CollisionHandler::pushMovableAwayFromMovable(movable2, movable1);
	}

	//these two are weird
	static void pushMovableAwayFromImmovable(Rect* movable, Circle* immovable);
	static void pushMovableAwayFromMovable(Rect* movable1, Circle* movable2);

	static void pushMovableAwayFromImmovable(Circle* movable, Circle* immovable);
	static void pushMovableAwayFromMovable(Circle* movable1, Circle* movable2);

	/*
	//interesting...
	static void pullMovableIntoImmovable(Rect* movable, Rect* immovable);
	static void pullMovableIntoMovable(Rect* movable1, Rect* movable2);
	static void pullMovableNearImmovable(Rect* movable, Rect* immovable); //like a reverse pull (but not a push because one has to stay inside the other)

	static void pullMovableIntoImmovable(Circle* movable, Rect* immovable);
	static void pullMovableIntoMovable(Circle* movable1, Rect* movable2);
	static void pullMovableNearImmovable(Circle* movable, Rect* immovable); //like a reverse pull (but not a push because one has to stay inside the other)

	static void pullMovableIntoImmovable(Rect* movable, Circle* immovable);
	static void pullMovableIntoMovable(Rect* movable1, Circle* movable2);
	static void pullMovableNearImmovable(Rect* movable, Circle* immovable); //like a reverse pull (but not a push because one has to stay inside the other)

	static void pullMovableIntoImmovable(Circle* movable, Circle* immovable);
	static void pullMovableIntoMovable(Circle* movable1, Circle* movable2);
	static void pullMovableNearImmovable(Circle* movable, Circle* immovable); //like a reverse pull (but not a push because one has to stay inside the other)
	*/

public: //stuff that gives a location of intersection (only use if collision is guranteed) (this should be expanded)
	static DoublePositionHolder circleLineIntersection(Circle*, double lineX1, double lineY1, double lineX2, double lineY2);

public: //other stuff
	static bool lineLineCollision(double line1X, double line1Y, double line2X, double line2Y, double line3X, double line3Y, double line4X, double line4Y);
	static bool lineRectCollision(double line1X, double line1Y, double line2X, double line2Y, Rect* r);
};
