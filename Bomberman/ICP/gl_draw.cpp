#include "gl_draw.h"

// OpenGL Extension Wrangler
#include <GL/glew.h> 
#include <GL/wglew.h> //WGLEW = Windows GL Extension Wrangler (change for different platform) 

// GLFW toolkit
#include <GLFW/glfw3.h>

// OpenGL math
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


void draw_simple_triangle(void)
{
	glm::vec3 color_red = { 1.0f, 0.0f, 0.0f };
	glm::vec3 color_green = { 0.0f, 1.0f, 0.0f };
	glm::vec3 color_blue = { 0.0f, 0.0f, 1.0f };

	//draw triangle (relative to origin)
	glBegin(GL_TRIANGLES);

	glColor3f(1.0, 0.0, 0.0);                // example of using immediate value for colour
	glVertex3f(-25.0, -14.0, 0.0);               // example of using immediate value for vertex coordinates
	glColor3fv(glm::value_ptr(color_green)); // example of using memory array value for colour
	glVertex3f(25.0, -14.0, 0.0);
	glColor3fv(glm::value_ptr(color_blue));
	glVertex3fv(glm::value_ptr(color_red));   // example of using memory array value for vertex coordinates

	glEnd();
}

void draw_dynamic_circle(const float center_x, const float center_y, const float r, const int num_segments)
{
	int i;
	float theta, x, y;

	glBegin(GL_LINE_LOOP);
	for (i = 0; i < num_segments; i++)
	{
		theta = 2.0f * 3.1415926f * i / num_segments;

		x = r * cosf(theta);
		y = r * sinf(theta);

		glVertex2f(x + center_x, y + center_y);
	}
	glEnd();
}
