/********************************************************************************
 * 
 *						PACKING circleS INTO A circle
 *
 *		This program try to put n circles with a realtion between its area into
 *      a bigger circle of center x0,y0 in the way that they are the biggest
 *      possible and that they do not overlapp.
 *      
 *      Arguments:
 *					1. X coordiante of the center of the biggest circle
 *					2. Y coordiante of the center of the biggest circle
 *                  3. Radius of the bigest circle
 *                  4. Number of circles to put in
 *					5. vector with the weigth of each circle w
 * 
 * ******************************************************************************/

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "distribute_components.h"
#include "parameters.h"

#define pi 3.14159265358979323846264338327950288419716939937510


int distribute_components(double X0,double Y0,double R,int N,double *w,double *x,double *y,double *r){
	gsl_rng * randgsl = gsl_rng_alloc(gsl_rng_taus);/// we initialize the random generator of the gsl
	gsl_rng_set(randgsl,parameters.getSeed());
	
 /******************************************************************************
      Arguments
 *******************************************************************************/
	
	double increment = 1.01;
	double factor_corrector = 1.10;
	int maxtries = 10*N;
	
	int i,j=0;
	double norm = 0;
	for (i=0; i<N; ++i){							/// we normalize the weights
		
		norm = norm + w[i];
	}
	for (i=0; i<N; ++i){							
		
		w[i] = w[i]/norm;
	}
	
 /******************************************************************************
	 we create the circles
 *******************************************************************************/
	
	double alpha = parameters.getAlpha();
	double beta  = parameters.getBeta();

	int i_max_rad = -1;
	double max_rad = -1;
	double theta,rad;
	for (i=0; i<N; ++i){							/// we give a random position to each circle between 0,R

		if (parameters.getCoordDistributionAlgorithm()=="log")
			r[i]  = R*sqrt(alpha*pow(log(1+ w[i]),beta));		/// a radius corresponding to its weight. We hope is small enoughand all cercles fit in the big circle
		else
			r[i]  = R*sqrt(alpha*pow(w[i],beta));		/// a radius corresponding to its weight. We hope is small enoughand all cercles fit in the big circle

		if (r[i] > max_rad) {
			max_rad = r[i];
			i_max_rad = i;
		}
//		std::cout << "Ri " << r[i] << std::endl;
		theta = gsl_rng_uniform(randgsl)*2*pi;      /// a random position
		rad   = gsl_rng_uniform(randgsl)*(R-r[i]);
		x[i]  = X0 + rad*cos(theta);				
		y[i]  = Y0 + rad*sin(theta);
	}
	//x[i_max_rad] = X0;
	//y[i_max_rad] = Y0;
		
 /******************************************************************************
	 we start the apcking procedure
 *******************************************************************************/
	
	double p,distance;
	
	int c1,c2,finish;
	for(finish=0; (finish==0);){
		
//		std::cout << "Alfa: " << alpha << std::endl;		
	  /*******we check if there is any circle out of the mother circle *****/
		for(i=0;i<N;++i){			
			
			distance = sqrt(pow(x[i]-X0,2)+pow(y[i]-Y0,2));
			
			if( distance > R - r[i] ) {	/// if it is the case we move it to a random new position
				
				if(!give_new_random_position(i,N,x,y,r,X0,Y0,R,maxtries,randgsl,-1)){	///if we could not move somewhere its finish
					finish=1;
					break;
				}
			}
		}
		
	  /******* we check if there is any overlap ***************************/

			for(i=0;i<N-1;++i){		
			  			
				for(j=i+1;j<N;++j){
				
					distance = sqrt(pow(x[i]-x[j],2)+pow(y[i]-y[j],2));
						
					if( distance < r[i]+r[j] ) {
						//std::cout << "superp " << i << ", " << j << std::endl;
						p = gsl_rng_uniform(randgsl);	/// we choose one of the two circles that averalp at random. we will move it to a random position
						if(r[i]<r[j]){ c1 = i; c2 = j;}
						else     { c1 = j; c2 = i;}
					
						if(!give_new_random_position(c1,N,x,y,r,X0,Y0,R,maxtries,randgsl,c2)){		/// we try yo move the first circle
					
							if(!give_new_random_position(c2,N,x,y,r,X0,Y0,R,maxtries,randgsl,c1)){ /// if we could not we try with the other one

									finish=1;	/// if we could not move any of two the process its finish
									break;
							}
						}
					}
				
				}
				if(finish) break;
			
			}
		
//		std::cout << "finish: " << finish << std::endl;
		if(finish) break;
		
		else{ /*******if there is no overlap we increase all the radius **********/
	 
			alpha = alpha*increment;
			for (i=0; i<N; ++i){							
				double r_prev = r[i];
	
				if (parameters.getCoordDistributionAlgorithm()=="log")
					r[i]  = R*sqrt(alpha*pow(log(1+ w[i]),beta));
				else
					r[i] = R*sqrt(alpha*pow(w[i],beta));

				double delta_r = r[i] - r_prev;
				x[i] = (sqrt(x[i]*x[i]+y[i]*y[i])-delta_r)*cos(atan2(y[i],x[i]));
				y[i] = (sqrt(x[i]*x[i]+y[i]*y[i])-delta_r)*sin(atan2(y[i],x[i]));
								
			}
							
		}
			
		
		
		
	}
	
 /******************************************************************************
	 we put the last alpha that worked
 *******************************************************************************/
	
	alpha = alpha/factor_corrector;
	for (i=0; i<N; ++i){							
		
		if (parameters.getCoordDistributionAlgorithm()=="log")
			r[i] = R*sqrt(alpha*pow(log(1+ w[i]),beta));
		else
			r[i] = R*sqrt(alpha*pow(w[i],beta));
		
	}
	
	return 1;
	
}
/******************************************************************************
  *******************************************************************************
								FUNCTIONS
  *******************************************************************************
 *******************************************************************************/
/// PUTS A CERCLE IN A NEW RANDOM POSITION
int give_new_random_position(int c,int N,double *x,double *y,double *r,double X0,double Y0,double R,int maxtries,gsl_rng * randgsl, int other){
	
	int i,overlap=0,tries;
	
	double theta,rad,xn=0,yn=0,distance;
			
	for(tries=0; tries < maxtries ; ){
		
		theta = gsl_rng_uniform(randgsl)*2.*pi;      /// a random new position
		rad   = gsl_rng_uniform(randgsl)*(R-r[c]);
		xn    = X0 + rad*cos(theta);				
		yn    = Y0 + rad*sin(theta);
		//std::cout << "R es " << R << std::endl;
		//std::cout << "xn " << xn << " yn " << yn << "radio " << r[c] << std::endl;
		//std::cout << "  el otro tiene xn " << x[other] << " yn " << y[other] << "radio " << r[other] << std::endl;
		overlap=0;
		
		for(i=0;i<N;++i){	///i check if there is another overlap
			if(i!=c) {
				distance = sqrt(pow(xn-x[i],2) + pow(yn-y[i],2));
				if( distance < r[c]+r[i] ) { 
					overlap = 1;
					tries++;
					break;
				}
			}
		}
		if(!overlap)break;
			
	}
//	std::cout << "listo" << std::endl;
	if(tries==maxtries){
	//	std::cout << "maxtries" << std::endl;
		return 0;
	}
	else {
		
		x[c] = xn;	///if we found a new available position we give it
		y[c] = yn;
		return  1;
	}
	
	
}

int give_new_random_position_2(int c1, int c2,int N,double *x,double *y,double *r,double X0,double Y0,double R,int maxtries,gsl_rng * randgsl){
	
	int i,overlap=0,tries;
	
	double theta,rad,xn1=0,yn1=0,xn2=0,yn2=0,distance;
			
	for(tries=0; tries < maxtries ; ){
		
		theta = gsl_rng_uniform(randgsl)*2.*pi;      /// a random new position
		rad   = gsl_rng_uniform(randgsl)*(R-r[c1]);
		xn1    = X0 + rad*cos(theta);				
		yn1    = Y0 + rad*sin(theta);

		theta = gsl_rng_uniform(randgsl)*2.*pi;      /// a random new position
		rad   = gsl_rng_uniform(randgsl)*(R-r[c2]);
		xn2    = X0 + rad*cos(theta);				
		yn2    = Y0 + rad*sin(theta);
		//std::cout << "R es " << R << std::endl;
		//std::cout << "xn " << xn << " yn " << yn << "radio " << r[c] << std::endl;
		//std::cout << "  el otro tiene xn " << x[other] << " yn " << y[other] << "radio " << r[other] << std::endl;
		overlap=0;
		
		distance = sqrt(pow(xn1-xn2,2) + pow(yn1-yn2,2));
		if( distance < r[c1]+r[c2] ) { 
			overlap = 1;
			tries++;
			break;
		}

		if(!overlap)break;
			
	}
//	std::cout << "listo" << std::endl;
	if(tries==maxtries){
	//	std::cout << "maxtries" << std::endl;
		return 0;
	}
	else {
		
		x[c1] = xn1;	///if we found a new available position we give it
		y[c1] = yn1;
		x[c2] = xn2;
		y[c2] = yn2;
		return  1;
	}
	
	
}
