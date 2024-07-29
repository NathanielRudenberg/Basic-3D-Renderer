#include "Slope.h"

Slope::Slope(float begin, float end, int numSteps) {
	float invStep = 1.0f / numSteps;
	cur = begin;
	step = (end - begin) * invStep;
}

float Slope::get() const { return cur; }

void Slope::advance() { cur += step;  }