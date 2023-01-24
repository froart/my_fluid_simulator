#ifndef FLUID_DYNAMICS_HPP_
#define FLUID_DYNAMICS_HPP_

#define I(x,y) ((x)+width_*(y))

class Fluid {
	private:
		int width_; 
		int height_;	
		float dt_;
		float diffusion_rate_;
		int iterations_;
		float* vx0_;
		float* vx_;
		float* vy0_;
		float* vy_;
		float* dens0_;
		float* dens_;
	public:
		Fluid(float*, int, int, float, float, int);
		void addInk(int, int, float);
		void addFluidFlow(int, int, float, float);
		void evaluate();
};

#endif // FLUID_DYNAMICS_HPP_
