#pragma once

class ResetThings {
public:
	static void reset() { ResetThings::reset(0); }
	static void reset(int);
};
