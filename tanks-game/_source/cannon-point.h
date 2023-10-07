#pragma once

struct CannonPoint {
	friend class Tank; //unnecessary

	double angleFromCenter;

	CannonPoint(double angle) {
		this->angleFromCenter = angle;
	}
	CannonPoint() { this->angleFromCenter = 0; }
};

struct ExtraCannonPoint {
	friend class Tank; //unnecessary

	double angleFromCenter;
	double angleFromEdge; //0 means straight out from center
	//note: two doubles don't satisfy std::atomic<T>::is_lock_free(), which doesn't really matter here

	ExtraCannonPoint(double angle) {
		this->angleFromCenter = angle;
		this->angleFromEdge = 0;
	}
	ExtraCannonPoint(double angleCenter, double angleEdge) {
		this->angleFromCenter = angleCenter;
		this->angleFromEdge = angleEdge;
	}
	ExtraCannonPoint() { this->angleFromCenter = this->angleFromEdge = 0; }
};
