#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#define _USE_MATH_DEFINES
#include <math.h>

//glfw
const int width = 640;
const int height = 480;

float* pixels = new float[width*height * 3];

void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue)
{
	pixels[(i + width* j) * 3 + 0] = red;
	pixels[(i + width* j) * 3 + 1] = green;
	pixels[(i + width* j) * 3 + 2] = blue;
}
void drawThicknerLine(const int& thickness, const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue) {
	for (int i = i0; i <= i1; i++)
	{
		const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

		for (int k = 0; k < thickness; k++) {
			drawPixel(i + k, j, red, green, blue);
		}
	}
}
// scratched from https://courses.engr.illinois.edu/ece390/archive/archive-f2000/mp/mp4/anti.html
// see 'Rasterization' part.
void drawLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue)
{
	for (int i = i0; i <= i1; i++)
	{
		const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

		drawPixel(i, j, red, green, blue);
	}
}
void drawSquare(int thickness, const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue) {
	int i_center = (i0 + i1) / 2;
	int j_center = (j0 + j1) / 2;

	for (int j = j_center - thickness; j < j_center + thickness; j++) {
		for (int i = i_center - thickness; i < i_center + thickness; i++) {
			drawPixel(i, j, red, green, blue);
		}
	}
}
void drawPolygon() {
	/*
	//오각형
	int i_center = 120;
	int j_center = 130;
	int size = 10;
	int j_start = j_center - size;
	int j_end = j_center + size;
	for (int j = j_start; j <= j_end + size; j++) {
	int i_start = i_center - size + (j / 5);
	int i_end = i_center + size - (j / 5);
	for (int i = i_start; i <= i_end; i++) {
	//if(j == j_start || j == j_end || i == i_start || i == i_end)
	drawPixel(i, j, 1.0f, 0.0f, 0.0f);
	}
	}
	*/
	drawLine(500, 200, 550, 200, 0.0f, 0.0f, 0.0f);
	drawLine(480, 250, 525, 290, 0.0f, 0.0f, 0.0f);
	drawLine(525, 290, 570, 250, 0.0f, 0.0f, 0.0f);

	drawLine(480, 250, 500, 200, 0.0f, 0.0f, 0.0f);
	drawLine(480, 251, 500, 201, 0.0f, 0.0f, 0.0f);

	drawLine(550, 200, 570, 250, 0.0f, 0.0f, 0.0f);
	drawLine(550, 201, 570, 251, 0.0f, 0.0f, 0.0f);
}
void drawCircle(const double& rad, const int& i0, const int& j0, const float& red, const float& green, const float& blue) {

	{
		for (double i = 0.0; i < 360.0; i += 0.1)
		{
			double angle = i *  M_PI / 180;

			int x = (int)(i0 + rad * cos(angle));
			int y = (int)(j0 + rad * sin(angle));

			drawPixel(x, y, red, green, blue);
		}
	}

}
void drawTriangle() {

	int i_center = 100;
	int j_center = 100;
	int size = 70;
	int j_start = j_center - size;
	int j_end = j_center + size;

	for (int j = j_start; j <= j_end; j++) {
		int i_start = i_center - size + (j / 2);
		int i_end = i_center + size - (j / 2);

		for (int i = i_start; i <= i_end; i++) {
			if (j == j_start || j == j_end || i == i_start || i == i_end)
				drawPixel(i, j, 1.0f, 0.0f, 0.0f);
		}
	}
}

void drawEmptySquare(int size, const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue) {
	int i_center = (i0 + i1) / 2;
	int j_center = (j0 + j1) / 2;

	for (int j = j_center - size; j <= j_center + size; j++) {
		//for (int i = i_center - thickness; i < i_center + thickness; i++) {
		for (int i = i_center - size; i <= i_center + size; i = i + size * 2)
			drawPixel(i, j, red, green, blue);

	}
	//for (int j = j_center - thickness; j < j_center + thickness; j++) {
	for (int i = i_center - size; i <= i_center + size; i++) {
		for (int j = j_center - size; j <= j_center + size; j = j + size * 2)
			drawPixel(i, j, red, green, blue);
	}
}
void drawOnPixelBuffer()
{
	//std::memset(pixels, 1.0f, sizeof(float)*width*height * 3); // doesn't work
	//std::fill_n(pixels, width*height * 3, 1.0f);	// white background

	// 흰색 배경
	for (int i = 0; i<width*height; i++) {
		pixels[i * 3 + 0] = 1.0f; // red 
		pixels[i * 3 + 1] = 1.0f; // green
		pixels[i * 3 + 2] = 1.0f; // blue
	}

	const int i = rand() % width, j = rand() % height;
	drawPixel(i, j, 0.0f, 0.0f, 0.0f);

	// drawing a line
	//TODO: anti-aliasing
	const int i0 = 100, i1 = 200;
	const int j0 = 300, j1 = 400;
	const int thickness = 10;
	const int size = 30;
	//drawLine(i0, j0, i1, j1, 1.0f, 0.0f, 0.0f);
	drawThicknerLine(thickness, i0, j0, i1, j1, 0.0f, 1.0f, 0.0f);
	drawSquare(size, i0 + 300, j0, i1 + 300, j1, 0.0f, 1.0f, 1.0f);
	drawCircle(50, i0 + 100, i1 + 100, 0.0f, 1.0f, 0.0f);
	drawEmptySquare(size, i0 + 100, j0, i1 + 100, j1, 1.0f, 0.0f, 0.0f);
	drawTriangle();
	drawPolygon();
	//TODO: try moving object
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

							  /* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);

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
