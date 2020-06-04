#include "colorvalueholder.h"
#include "mylib.h"

ColorValueHolder::ColorValueHolder(float r, float g, float b) {
	R = constrain_f(r, 0.0, 1.0);
	G = constrain_f(g, 0.0, 1.0);
	B = constrain_f(b, 0.0, 1.0);
	A = 1;
}

ColorValueHolder::ColorValueHolder(float r, float g, float b, float a) : ColorValueHolder(r, g, b){
	A = constrain_f(a, 0.0, 1.0); //small inefficiency because A is defined twice but what can I do other than copying code
}
