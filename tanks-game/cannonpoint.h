#pragma once

struct CannonPoint {
	friend class Tank; //only if, by chance, something makes the other stuff private (might be me in the future)

	double angle;
	CannonPoint(double angle);
	CannonPoint();
};