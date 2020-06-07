#pragma once

class ResetThings {
public:
	static void reset() { ResetThings::reset(0); }
	static void reset(int);
	static void firstReset(); //temporary; just for main() to do its thing while I'm still testing
};
