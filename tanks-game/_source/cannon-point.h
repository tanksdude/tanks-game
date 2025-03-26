#pragma once

struct CannonPoint {
	friend class Tank; //unnecessary

	float angleFromCenter;

	CannonPoint(float angle) {
		this->angleFromCenter = angle;
	}
	CannonPoint() { this->angleFromCenter = 0; }
};

struct ExtraCannonPoint {
	friend class Tank; //unnecessary

	float angleFromCenter;
	float angleFromEdge; //0 means straight out from center
	//note: two doubles don't satisfy std::atomic<T>::is_lock_free(), which doesn't really matter here

	ExtraCannonPoint(float angle) {
		this->angleFromCenter = angle;
		this->angleFromEdge = 0;
	}
	ExtraCannonPoint(float angleCenter, float angleEdge) {
		this->angleFromCenter = angleCenter;
		this->angleFromEdge = angleEdge;
	}
	ExtraCannonPoint() { this->angleFromCenter = this->angleFromEdge = 0; }
};
