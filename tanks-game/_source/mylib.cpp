#include "mylib.h"

#include "rng.h" //for weightedSelect

//algorithm from https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
/*
Copyright (c) 1970-2003, Wm. Randolph Franklin

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

1.  Redistributions of source code must retain the above copyright notice, this 
    list of conditions and the following disclaimers.
2.  Redistributions in binary form must reproduce the above copyright notice in 
    the documentation and/or other materials provided with the distribution.
3.  The name of W. Randolph Franklin may not be used to endorse or promote 
    products derived from this Software without specific prior written permission. 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
*/
bool pointInPolygon(int vertNum, const double* vertX, const double* vertY, double testX, double testY) {
	int i, j, c = 0;
	for (i = 0, j = vertNum-1; i < vertNum; j = i++) {
		if ( ((vertY[i]>testY) != (vertY[j]>testY)) &&
		     (testX < (vertX[j]-vertX[i]) * (testY-vertY[i]) / (vertY[j]-vertY[i]) + vertX[i]) )
			c = !c;
	}
	return c;
}
bool pointInPolygon(int vertNum, const float* vertX, const float* vertY, float testX, float testY) {
	int i, j, c = 0;
	for (i = 0, j = vertNum-1; i < vertNum; j = i++) {
		if ( ((vertY[i]>testY) != (vertY[j]>testY)) &&
		     (testX < (vertX[j]-vertX[i]) * (testY-vertY[i]) / (vertY[j]-vertY[i]) + vertX[i]) )
			c = !c;
	}
	return c;
}

//algorithm from https://forum.unity.com/threads/weighted-random-selection.221890/ and https://forum.unity.com/threads/randomising-shuffling-library.29977/
template<typename T>
int weightedSelect(const T* weights, int num) {
	//TODO: unit tests
	T weightTotal = 0;
	for (int i = 0; i < num; i++) {
		weightTotal += weights[i];
	}
	T targetWeight = RNG::randFunc() * weightTotal;
	T newWeight = 0;
	int newIndex = 0;
	while (targetWeight >= newWeight) {
		newWeight += weights[newIndex];
		newIndex++;
	}
	return newIndex-1;
}
template int weightedSelect<double>(const double*, int);
template int weightedSelect<float>(const float*, int);
