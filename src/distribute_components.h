#ifndef _DISTRIBUTE_COMPONENTS_
#define _DISTRIBUTE_COMPONENTS_

#include <gsl/gsl_rng.h>

int distribute_components(double X0,double Y0,double R,int N,double *w,double *x,double *y,double *r);
int give_new_random_position(int c,int N,double *x,double *y,double *r,double X0,double Y0,double R,int maxtries,gsl_rng * randgsl, int other);
int give_new_random_position_2(int c1, int c2,int N,double *x,double *y,double *r,double X0,double Y0,double R,int maxtries,gsl_rng * randgsl);

#endif

