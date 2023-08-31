#pragma once
#include "circle.h"
#include "rect.h"
#include "position-holder.h"
#include <utility>

class CollisionHandler {
protected:
	static bool cornerCollided(const Circle*, double x, double y);
	static bool cornerCollidedIgnoreEdge(const Circle*, double x, double y);
	static void cornerPushMovableAwayFromImmovable(Circle* movable, double x, double y);
	static void cornerPushMovableAwayFromMovable(Circle* movable1, Rect* movable2, double x, double y);
	static void cornerPushMovableAwayFromImmovable(Rect* movable, Circle*, double x, double y);
	static void cornerPushMovableAwayFromMovable(Rect* movable1, Circle* movable2, double x, double y) {
		CollisionHandler::cornerPushMovableAwayFromMovable(movable2, movable1, x, y);
	}

public: //for tanks and bullets and stuff (things that move around)
	static bool partiallyOutOfBounds(const Rect*);
	static bool partiallyOutOfBoundsIgnoreEdge(const Rect*);
	static bool fullyOutOfBounds(const Rect*);
	static bool fullyOutOfBoundsIgnoreEdge(const Rect*);

	//below: only does simple check, treating the circle like a square
	static bool partiallyOutOfBounds(const Circle*);
	static bool partiallyOutOfBoundsIgnoreEdge(const Circle*);
	static bool fullyOutOfBounds(const Circle*);
	static bool fullyOutOfBoundsIgnoreEdge(const Circle*);

	static void edgeConstrain(Rect*);
	static void edgeConstrain(Circle*);
	static void edgeConstrain(Rect*, double distFromEdge); //why? at the very least, it's better than powers having to do it themselves, I guess
	static void edgeConstrain(Circle*, double distFromEdge);

public: //collision detection and handling (just moving)
	static bool partiallyCollided(const Rect*, const Rect*);
	static bool partiallyCollided(const Rect*, const Circle*);
	static bool partiallyCollided(const Circle* c, const Rect* r) {
		return CollisionHandler::partiallyCollided(r, c);
	}
	static bool partiallyCollided(const Circle*, const Circle*);

	static bool partiallyCollidedIgnoreEdge(const Rect*, const Rect*);
	static bool partiallyCollidedIgnoreEdge(const Rect*, const Circle*);
	static bool partiallyCollidedIgnoreEdge(const Circle* c, const Rect* r) {
		return CollisionHandler::partiallyCollidedIgnoreEdge(r, c);
	}
	static bool partiallyCollidedIgnoreEdge(const Circle*, const Circle*);

	static bool fullyCollided(const Rect*, const Rect*);
	static bool fullyCollided(const Rect*, const Circle*);
	static bool fullyCollided(const Circle*, const Rect*); //different
	static bool fullyCollided(const Circle*, const Circle*);

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
	static std::pair<PositionHolder, PositionHolder> circleLineIntersection(const Circle*, double lineX1, double lineY1, double lineX2, double lineY2);

public: //other stuff
	static bool lineLineCollision(double line1X, double line1Y, double line2X, double line2Y, double line3X, double line3Y, double line4X, double line4Y);
	static bool lineRectCollision(double line1X, double line1Y, double line2X, double line2Y, const Rect* r);

private:
	CollisionHandler() = delete;
	CollisionHandler(const CollisionHandler&) = delete;
};
