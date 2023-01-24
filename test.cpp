#include "opengl_setup.hpp"
#include "fluid_dynamics.hpp"
#include <iostream>
#include <omp.h>

using namespace std;

int width = 200;
int height = 200;
int brush_width = 20;
float* image = new float[width * height];
Fluid f(image, width, height, 0.1, 100, 6);			

void loop_code() {
	f.evaluate();	
	return;
}

int main() {
	runSimulation("Fluid Simulator");
	return 0;
}

void mouse(int button, int state, int x, int y){
	if(button == GLUT_RIGHT_BUTTON)
		std::fill_n(image, width*height, 0);
	if(button == GLUT_LEFT_BUTTON) {
		for(int i = -brush_width/2; i <= brush_width/2; ++i)
			for(int j = -brush_width/2; j <= brush_width/2; ++j)
				f.addInk(x+i, height-y+j, 0.5);
	}
}

void keyboard(unsigned char c, int x, int y) {
  if(c == 27) {
	for(int j = 0; j < height; ++j)
		for(int i = 0; i < width; ++i) {
			int k = i + j*width;	
			//cout << image[k] << (k == (width-1) ? "\n" : " "); 
		}
    exit(0);
	}
}


