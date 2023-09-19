#pragma once

//TODO: probably need another struct which holds angle relative to tank as well as the angle the bullet will be shot from (think triple or shotgun)
struct CannonPoint {
	friend class Tank; //only if, by chance, something makes the other stuff private (which might be me in the future)

	double angle;
	CannonPoint(double angle) {
		this->angle = angle;
	}
	CannonPoint() { this->angle = 0; }
};
