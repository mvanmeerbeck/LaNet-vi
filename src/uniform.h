#ifndef _UNIFORM_H_
#define _UNIFORM_H_

#include "stdlib.h"
#include <iostream>

//This class implements a uniform distribution number generator

class Uniform {
  private:
	float min, max;

  public:
	Uniform(float mn, float mx) : min(mn), max(mx) {};

	inline void setMin(float mi) { min = mi;}
	inline void setMax(float mi) { max = mi;}
	
	inline float getValue() {
		return (min + (max - min)*((float)rand()/RAND_MAX));}
		
};

#endif

extern Uniform uniform2Pi;

