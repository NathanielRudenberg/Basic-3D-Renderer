#pragma once
class Slope {
	float cur, step;

public:
	Slope();
	Slope(float begin, float end, int numSteps);
	float get() const;
	void advance();
};

