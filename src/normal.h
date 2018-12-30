#ifndef _NORMAL_H_
#define _NORMAL_H_

#include "math.h"

//This class implements a normal distribution number generator

class Normal {
  private:
	float mean, dev;

  public:
	Normal(float m, float d) : mean(m), dev(d) {};
	
	inline void setMean(float m) { mean = m;}
	
	inline void setDev(float d) { dev = d;}
	
	inline float getValue() {
		float v = 1;
		int n = 100;
		//A [0,1] normal distribution sample is obtained by averaging samples from a [-1,1] uniform distribution
		for (int i=1;i<=n;i++)
			v += (float)2*rand()/RAND_MAX - 1;
		v *= sqrt((float)3/n);
		//Denormalization
		return (mean + v*dev);
	}
};

#endif

