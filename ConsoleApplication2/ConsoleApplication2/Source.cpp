#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <string>


//glfw
const int width = 640;
const int height = 480;

float* pixels = new float[width*height * 3];

void drawPixel(const int& i, const int& j, const int& color);
void drawLine(const int& i0, const int& j0, const int& i1, const int& j1, const int& color);


class Box 
{

private:
	int x = 10;
	int y = 10;
	int size = 100;
	int color = 0x0000FF;

public:
	void draw()
	{
			drawLine(x, y, x, y + size, color);
			drawLine(x, y, x + size, y, color);
			drawLine(x, y + size, x + size, y + size, color);
			drawLine(x + size, y, x + size, y + size, color);

	}
};

class Circle 
{
private :
	const int& x=20;
	const int& y=20;
	const int& size = 200;
	const int& bold = 70;
	int color = 0x228b22;

public:
	
	void draw()
	{
		int bSize = size / 2;
		int cSize = (size - 10) / 2;
		for (int i = 0; i < size; i++) {
			 		for (int j = 0; j < size; j++) {
				 			if (!(
									(i - bSize)*(i - bSize) + (j - bSize)*(j - bSize) <= (cSize * cSize) - bold
					 || (i - bSize)*(i - bSize) + (j - bSize)*(j - bSize) >= (cSize * cSize) + bold
					)) {
					 				drawPixel(x + i, y + j, color);
				}
				
			}
		}

	}
};


class GeometricObjectInterface 
{
public:
	virtual void draw() = 0; // 순수 가상 함수.
};

// And implement an templatized GeometricObject class.

template <typename T>
class GeometricObject : public GeometricObjectInterface // 구현합니다.
{
public:
	void draw() 
	{
		T drawfigure;
		drawfigure.draw();
	}
};


	std::vector<GeometricObjectInterface*> obj_list;


void drawPixel(const int& i, const int& j, const int& color)
{
	float red = (float)((color & 0xFF0000) / 0x00FFFF) / 255;
	float green = (float)((color & 0x00FF00) / 0x0000FF) / 255;
	float blue = (float)(color & 0x0000FF) / 255;
	if (i < 0 || i >= width) return;
	if (j < 0 || j >= height) return;
	pixels[(i + width * (height - j - 1)) * 3 + 0] = red;
	pixels[(i + width * (height - j - 1)) * 3 + 1] = green;
	pixels[(i + width * (height - j - 1)) * 3 + 2] = blue;

}

// scratched from https://courses.engr.illinois.edu/ece390/archive/archive-f2000/mp/mp4/anti.html
// see 'Rasterization' part.


 void drawLine(const int& i0, const int& j0, const int& i1, const int& j1, const int& color)
{
 	if (i1 - i0 == 0) {
				for (int i = j0; i <= j1; i++) {
						drawPixel(i0 + 0, i, color);
						drawPixel(i0 + 1, i, color);
						drawPixel(i0 - 1, i, color);
						drawPixel(i0, i + 1, color);
						drawPixel(i0, i - 1, color);
		}
				return;
	}
	

		 	int preJ = -1;
		for (int i = i0; i <= i1; i++) 
		{
				const int j = (j1 - j0) * (i - i0) / (i1 - i0) + j0;
		 		drawPixel(i + 0, j, color);
		 		drawPixel(i + 1, j, color);
		 		drawPixel(i - 1, j, color);
	 			drawPixel(i, j + 1, color);
		 		drawPixel(i, j - 1, color);
		

				if (preJ != -1 && abs(abs(preJ) - abs(j)) > 3) {
				 		drawPixel(i - 1, j - 1, color);
			 			drawPixel(i - 1, j + 1, color);
						drawPixel(i + 1, j - 1, color);
						drawPixel(i + 1, j + 1, color);
			

				 	
				 		drawPixel(i - 1, j - 2, color);
						drawPixel(i - 0, j - 2, color);
			 			drawPixel(i + 1, j - 2, color);
			

						
				 		drawPixel(i - 1, j + 2, color);
						drawPixel(i - 0, j + 2, color);
			 			drawPixel(i + 1, j + 2, color);
			
		}
		 		preJ = j;
	}
}


void drawOnPixelBuffer()
{
	//std::memset(pixels, 1.0f, sizeof(float)*width*height * 3); // doesn't work
	std::fill_n(pixels, width*height * 3, 1.0f);	// white background

	// 흰색 배경
	
	for (auto itr : obj_list)
	{
		itr->draw();
	}

	//TODO: anti-aliasing

	//TODO: try moving object
	
	return;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background

	obj_list.push_back(new GeometricObject<Circle>);
	obj_list.push_back(new GeometricObject<Box>);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		drawOnPixelBuffer();

		//TODO: RGB struct
		//Make a pixel drawing function
		//Make a line drawing function

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	delete[] pixels; // or you may reuse pixels array 

	return 0;
}


