#ifndef _CIRCULAR_AVERAGE_
#define _CIRCULAR_AVERAGE_

//This class implements a weighted circular average calculator

class Circular_average {
  public:
	float average(float a, float weight_a, float b, float weight_b) {
		while (a<0)
			a+=2*pi;
		while (a>2*pi)
			a-=2*pi;
		while (b<0)
			b+=2*pi;
		while (b>2*pi)
			b-=2*pi;

		float aperture;
		float first;
		float weight_second;

		if (a>b) {
			aperture = a-b;
			first = b;
			weight_second = weight_a;
		}
		else {
			aperture = b-a;
			first = a;
			weight_second = weight_b;
		}
		if (aperture > pi) {
			aperture = 2*pi - aperture;
			if (first==a) {
				first=b;
				weight_second = weight_a;
			}
			else {
				first=a;
				weight_second = weight_b;
			}
		}

		float res = first + (aperture*weight_second)/(weight_a+weight_b);
		return res;

/*		float xa = cos(weight_a*a);
		float ya = sin(weight_a*a);
		
		float xb = cos(weight_b*b);
		float yb = sin(weight_b*b);
		
		float xav = xa + xb;
		float yav = ya + yb;
		
		return atan2(yav, xav);
*/	}
};

#endif

