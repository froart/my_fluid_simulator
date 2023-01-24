#include "fluid_dynamics.hpp"
#include "opengl_setup.hpp"
#include <math.h>
#include <iostream>
#include <omp.h>
#include <algorithm>
#include <math.h>

using namespace std;

#define SWAP(a,b) {float* tmp=a;a=b;b=tmp;}

Fluid::Fluid(float* image = NULL,
				 int width = 100, 
				 int height = 100, 
				 float dt = 0.1,
				 float diffusion_rate = 0.1,
				 int iterations = 5) 
				 : dens_(image),
					 width_(width), 	
					 height_(height),
					 dt_(dt),
				 	 diffusion_rate_(diffusion_rate),
					 iterations_(iterations) {
	vx0_ = new float[width * height];
	vx_ = new float[width * height];
	vy0_ = new float[width * height];
	vy_ = new float[width * height];
	dens0_ = new float[width * height];
};

void Fluid::addInk(int x, int y, float amount) {
	dens_[I(x,y)] += amount;
}

void Fluid::addFluidFlow(int x, int y, float x_amount, float y_amount) {
	vx_[I(x,y)] += x_amount;
	vy_[I(x,y)] += y_amount;
}

void Fluid::evaluate() {

/* INK DIFFUSION */
	SWAP(dens0_, dens_);
	// STABLE VERSION
	float how_fast = diffusion_rate_ * dt_ ;
	for(int k = 0; k < iterations_; ++k)
		for(int j = 0; j < height_; ++j)
			for(int i = 0; i < width_; ++i) {
				dens_[I(i,j)] = (dens0_[I(i,j)] 
											 + how_fast * (dens_[I(i-1,j)] + dens_[I(i+1,j)]
																	 + dens_[I(i,j-1)] + dens_[I(i,j+1)]))
											   / (1+4*how_fast);		
			}
	// --------------
	/* UNSTABLE VERSION
	float how_fast = diffusion_rate_ * dt_ ;
	for(int j = 0; j < height_; ++j)
		for(int i = 0; i < width_; ++i) {
			float p_left = (float) (i-1 >= 0);
			float p_right = (float) (i+1 < width_);
			float p_bottom = (float) (j-1 >= 0);
			float p_top = (float) (j+1 < height_);
			dens_[I(i,j)] = dens0_[I(i, j)] 
		 							+ how_fast * (-4       * dens0_[I(i,  j)]
															+ p_left   * dens0_[I(i-1,j)]
								 							+ p_right  * dens0_[I(i+1,j)] 
						  								+ p_bottom * dens0_[I(i,j-1)] 
								 							+ p_top    * dens0_[I(i,j+1)]) 
									/ (p_left + p_right + p_bottom + p_top + 1) ; 
		}
	*/

/* INK ADVECTION */
	SWAP(dens0_, dens_);
	for(int j = 0; j < height_; ++j)
		for(int i = 0; i < width_; ++i) {
			float x0 = (float) i;
			float y0 = (float) j;
	 		float x1 = x0 + vx_[I((int)x0,(int)y0)] * dt_;
	 		float y1 = y0 + vy_[I((int)x0,(int)y0)] * dt_;
	 		if((abs(x1-floor(x1)) != 0) && (abs(y1-floor(y1)) != 0)) {
	 			dens_[(int)floor(x1), (int)floor(y1)] += abs((y1 - floor(y1)) 
																		 * (x1 - floor(x1))) * dens0_[(int)x0, (int)y0];
	 			dens_[(int)ceil(x1), (int)floor(y1)] += abs((y1 - floor(y1)) 
																		* (x1 - ceil(x1))) * dens0_[(int)x0, (int)y0];
	 			dens_[(int)ceil(x1), (int)ceil(y1)] += abs((y1 - ceil(y1)) 
																		* (x1 - ceil(x1))) * dens0_[(int)x0, (int)y0];
	 			dens_[(int)floor(x1), (int)ceil(y1)] += abs((y1 - ceil(y1)) 
																		* (x1 - floor(x1))) * dens0_[(int)x0, (int)y0];
	 		} else if(abs(x1-floor(x1)) == 0) {
	 			dens_[(int)x1, (int)floor(y1)] += abs(y1 - floor(y1)) * dens0_[(int)x0, (int)y0];
	 			dens_[(int)x1, (int)ceil(y1)] += abs(y1 - ceil(y1)) * dens0_[(int)x0, (int)y0];
			} else if(abs(y1-floor(y1)) == 0) {
	 			dens_[(int)floor(x1), (int)y1] += abs(x1 - floor(x1)) * dens0_[(int)x0, (int)y0];
	 			dens_[(int)ceil(x1), (int)y1] += abs(x1 - ceil(x1)) * dens0_[(int)x0, (int)y0];
	 		} else if((abs(x1-floor(x1)) == 0) && (abs(y1-floor(y1)) == 0)) {
	 			dens_[(int)x1, (int)y1] += dens0_[(int)x0, (int)y0];
			}
		}
	// TODO boundary conditions

/* ADVECT FLUID */
	for(int j = 0; j < height_; ++j)
		for(int i = 0; i < width_; ++i) {
			//vx[I(i,j)] = vx[I(i-1,j] > vx[I(i+1,j] ? 
		}
	// TODO boundary conditions
}
