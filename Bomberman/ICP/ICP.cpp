// icp.cpp 
// Author: Tadeas a Tyna

// C++ 
#include <iostream>
#include <chrono>
#include <stack>
#include <random>
#include <numeric>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
// OpenCV 
#include <opencv2\opencv.hpp>

// OpenGL Extension Wrangler
#include <GL/glew.h> 
#include <GL/wglew.h> //WGLEW = Windows GL Extension Wrangler (change for different platform) 

// GLFW toolkit
#include <GLFW/glfw3.h>

// OpenGL math
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// OpenGL textures
#include <gli/gli.hpp>

//project includes...
#include "globals.h"
#include "init.h"
#include "callbacks.h"
#include "glerror.h" // Check for GL errors
#include "gl_draw.h"

//#include "mesh_init.h"
//#include "mesh.h"
#include "OBJloader.h"
#include "texture.h"
#include "shaders.h"
#include "mesh.h"
#include "mesh_init.h"
#include <fstream>

bool shader_ready;
int countA = 0;
int poseA = 0;
int countB = 0;
int poseB = 0;
int bombXA[3], bombYA[3], bombXB[3], bombYB[3];
int winA = 0;
int winB = 0;

PlayerAvatar player1 = { 10.0f, 0.0f, 0.0f, 0.0f };
PlayerAvatar player2 = { 10.0f, 0.0f, 0.0f, 0.0f };

Avatar camera = { 10.0f, 0.0f, 0.0f, 0.0f };

GLuint shader_id;
GLuint shader_id2;
bool use_shader = false;
mesh mesh_floor;
mesh mesh_brick;
mesh mesh_brick2;
mesh mesh_treasure;
mesh gamer1;
mesh gamer2;
mesh bomb;
mesh skull;

bool left_click = false;
int enableLight = 0;

std::stack<glm::mat4> stack_modelview;
constexpr auto BLOCK_SIZE = 10.0f;

char data1[144] = {  '#','#','#','#','#','#','#','#','#','#','#','#',
					'#','.','.','.','.','.','.','.','.','.','$','#',
					'#','.','$','$','.','$','$','.','$','.','$','#',
					'#','.','$','.','.','$','$','.','$','.','.','#',
					'#','.','$','.','$','$','$','.','$','$','.','#',
					'#','.','.','.','$','.','$','.','$','.','.','#',
					'#','.','$','$','$','.','$','.','$','.','$','#',
					'#','.','$','$','.','.','.','.','$','.','$','#',
					'#','.','.','.','.','$','$','.','.','.','.','#',
					'#','.','$','$','.','$','$','.','$','$','.','#',
					'#','.','.','.','.','.','.','.','.','.','.','#',
					'#','#','#','#','#','#','#','#','#','#','#','#' };
char data2[144] = { '#','#','#','#','#','#','#','#','#','#','#','#',
					'#','.','.','$','.','.','.','.','.','.','$','#',
					'#','$','.','$','.','$','$','.','$','.','$','#',
					'#','.','.','$','.','$','$','.','$','.','.','#',
					'#','.','$','$','.','$','$','.','$','$','.','#',
					'#','.','$','$','.','$','$','.','$','.','.','#',
					'#','.','.','.','.','.','.','.','$','.','$','#',
					'#','$','.','$','.','$','$','$','.','.','.','#',
					'#','.','.','$','.','$','$','.','.','$','.','#',
					'#','.','$','.','.','.','$','.','$','$','.','#',
					'#','.','.','.','$','.','.','.','.','.','.','#',
					'#','#','#','#','#','#','#','#','#','#','#','#' };
char data3[144] = { '#','#','#','#','#','#','#','#','#','#','#','#',
					'#','.','.','.','.','.','.','.','.','.','.','#',
					'#','.','$','$','.','$','$','.','$','$','.','#',
					'#','.','.','.','.','.','.','.','.','.','$','#',
					'#','$','$','.','$','$','.','$','$','.','.','#',
					'#','.','.','.','.','.','.','.','$','$','.','#',
					'#','.','$','$','.','$','$','.','.','.','.','#',
					'#','.','.','.','.','.','.','$','$','.','$','#',
					'#','$','$','.','$','$','.','.','.','.','.','#',
					'#','.','.','.','.','.','.','$','$','$','.','#',
					'#','.','.','$','$','$','.','.','.','.','.','#',
					'#','#','#','#','#','#','#','#','#','#','#','#' };
char data4[144] = { '#','#','#','#','#','#','#','#','#','#','#','#',
					'#','.','.','.','.','.','.','.','.','.','.','#',
					'#','.','$','$','$','.','$','.','$','$','.','#',
					'#','.','$','.','.','.','.','.','.','$','.','#',
					'#','.','$','.','$','$','.','$','.','.','.','#',
					'#','.','.','.','$','.','.','.','.','$','.','#',
					'#','.','$','.','.','.','.','$','.','.','.','#',
					'#','.','.','.','$','.','$','$','.','$','.','#',
					'#','.','$','.','.','.','.','.','.','$','.','#',
					'#','.','$','$','.','$','.','$','$','$','.','#',
					'#','.','.','.','.','.','.','.','.','.','.','#',
					'#','#','#','#','#','#','#','#','#','#','#','#' };
char tmp[144];
cv::Mat maps;

static void local_init(void);
static void local_init_mesh(void);

bool reset = false;

void reset_projection(void);

uchar getmap(cv::Mat& map, int x, int y)
{
	if (x < 0)
	{
		std::cerr << "Map: X too small: " << x << std::endl;
		x = 0;
	}

	if (x >= map.cols)
	{
		std::cerr << "Map: X too big: " << x << std::endl;
		x = map.cols - 1;
	}

	if (y < 0)
	{
		std::cerr << "Map: Y too small: " << y << std::endl;
		y = 0;
	}

	if (y >= map.rows)
	{
		std::cerr << "Map: Y too big: " << y << std::endl;
		y = map.rows - 1;
	}

	return map.at<uchar>(y, x);
}

//---------------------------------------------------------------------
// Generate map
//---------------------------------------------------------------------
void genLabyrinth2(void)
{
	int i, j;
	//print map
	for (j = 0; j < maps.rows; j++) {
		for (i = 0; i < maps.cols; i++) {
			std::cout << getmap(maps, i, j);
		}
		std::cout << std::endl;
	}
	player1.posX = 10.0f;
	player1.posY = 10.0f;
	maps.at<uchar>(cv::Point(1, 1)) = 'p';
	player2.posX = 100.0f;
	player2.posY = 100.0f;
	maps.at<uchar>(cv::Point(10, 10)) = 'o';
}

//---------------------------------------------------------------------
// Set camera position
//---------------------------------------------------------------------
void avatarCamera(Avatar* avatar)
{
	constexpr auto SCENE_SHIFT = -1.0;
	glTranslatef(0.0, -180.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, SCENE_SHIFT, 0.0);
	glRotatef(55.0, 1.0, 0.0, 0.0);      
	glTranslatef(-70, -290, 0.0);  
	glTranslatef(avatar->posX, avatar->posY, 0.0);  
}

//---------------------------------------------------------------------
// Check collision
//---------------------------------------------------------------------
static unsigned char collision(PlayerAvatar* avatar)
{
	int kvadrantX, kvadrantY;

	kvadrantX = ceilf(avatar->posX / BLOCK_SIZE);
	kvadrantY = ceilf(avatar->posY / BLOCK_SIZE);

	return getmap(maps, abs(kvadrantX), abs(kvadrantY));
}

//---------------------------------------------------------------------
// Draw objects
//---------------------------------------------------------------------
void DrawAll(void)
{
	int i, j;

	//light on/off
	if (enableLight == 0)
	{	
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHTING);
	}
	else if (enableLight == 1) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	else if (enableLight == 2) {
		glEnable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	avatarCamera(globals.camera);
	mesh_draw(mesh_floor);
	
	for (j = 0; j < maps.rows; j++)
	{
		for (i = 0; i < maps.cols; i++)
		{
			switch (getmap(maps, i, j))
			{
			case '.':
				continue;
				break;
			case '#':
				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE, j * BLOCK_SIZE, 0.0f);
				mesh_draw(mesh_brick);
				glPopMatrix();
				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE+5, j * BLOCK_SIZE+5, -20.0f);
				glScalef(0.25f, 0.25f, 0.25f);
				glRotatef(180, 1, 0, 0);
				mesh_draw(skull);
				glPopMatrix();
				break;
			case '$':
				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE, j * BLOCK_SIZE, 0.0f);
				mesh_draw(mesh_brick2);
				glPopMatrix();
				break;
			case 'p':
				glPushMatrix();
				glTranslatef(player1.posX , player1.posY, 0.0f);
				mesh_draw(player1.object);
				glPopMatrix();
				break;
			case 'o':
				glPushMatrix();
				glTranslatef(player2.posX, player2.posY, 0.0f);
				mesh_draw(player2.object);
				glPopMatrix();
				break;
			case 'b':
				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE + 3, j * BLOCK_SIZE + 3, 0.0f);
				mesh_draw(bomb);
				glPopMatrix();
				break;
			case 'l':
				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE + 3, j * BLOCK_SIZE + 3, 0.0f);
				mesh_draw(bomb);
				glPopMatrix();
				break;
			case 'Q':
				glPushMatrix();
				glTranslatef(player1.posX, player1.posY, 0.0f);
				mesh_draw(player1.object);
				glPopMatrix();
				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE + 3, j * BLOCK_SIZE + 3, 0.0f);
				mesh_draw(bomb);
				glPopMatrix();
				break;
			case 'W':
				glPushMatrix();
				glTranslatef(player2.posX, player2.posY, 0.0f);
				mesh_draw(player2.object);
				glPopMatrix();
				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE + 3, j * BLOCK_SIZE + 3, 0.0f);
				mesh_draw(bomb);
				glPopMatrix();
				break;
			case 'E':
				glPushMatrix();
				glTranslatef(player1.posX, player1.posY, 0.0f);
				mesh_draw(player1.object);
				glPopMatrix();
				glPushMatrix();
				glTranslatef(player2.posX, player2.posY, 0.0f);
				mesh_draw(player2.object);
				glPopMatrix();
				break;
			default:
				break;
			}
		}
	}
	
}

//---------------------------------------------------------------------
// Select random map  
//---------------------------------------------------------------------
void select_random_map() {

	srand(time(NULL));
	int numberOfMap = rand() % 4 + 1;
	switch (numberOfMap)
	{
	case 1:
		for (int i = 0; i < 144; i++) {
			tmp[i] = data1[i];
		}
		break;
	case 2:
		for (int i = 0; i < 144; i++) {
			tmp[i] = data2[i];
		}
		break;
	case 3:
		for (int i = 0; i < 144; i++) {
			tmp[i] = data3[i];
		}
		break;
	case 4:
		for (int i = 0; i < 144; i++) {
			tmp[i] = data4[i];
		}
		break;
	default:
		break;
	}
	maps = cv::Mat(12, 12, CV_8U, tmp);
}

//---------------------------------------------------------------------
// Bomb logic
//---------------------------------------------------------------------
void Boooom(int bX, int bY) {
	bool wall[4] = { false, false, false, false };
	bool gamerA = false;
	bool gamerB = false;
	bool end = false;

	for (int i = 1; i < 4; i++) {
		if (!wall[0]) {
			if (getmap(maps, bX, bY - i) == '#' || getmap(maps, bX, bY - i) == 'b' || getmap(maps, bX, bY - i) == 'l' || getmap(maps, bX, bY - i) == '$') {
				wall[0] = true;
			}
			else if (getmap(maps, bX, bY - i) == 'Q' || getmap(maps, bX, bY - i) == 'p' || getmap(maps, bX, bY - i) == 'W' || getmap(maps, bX, bY - i) == 'o' || getmap(maps, bX, bY - i) == 'E') {
				if (getmap(maps, bX, bY - i) == 'Q' || getmap(maps, bX, bY - i) == 'p') {
					gamerA = true;
				}
				else if (getmap(maps, bX, bY - i) == 'W' || getmap(maps, bX, bY - i) == 'o') {
					gamerB = true;
				}
				else if (getmap(maps, bX, bY - i) == 'E') {
					gamerA = true;
					gamerB = true;
				}
			}
		}
		if (!wall[1]) {
			if (getmap(maps, bX, bY + i) == '#' || getmap(maps, bX, bY + i) == 'b' || getmap(maps, bX, bY + i) == 'l' || getmap(maps, bX, bY + i) == '$') {
				wall[1] = true;
			}
			else if (getmap(maps, bX, bY + i) == 'Q' || getmap(maps, bX, bY + i) == 'p' || getmap(maps, bX, bY + i) == 'W' || getmap(maps, bX, bY + i) == 'o' || getmap(maps, bX, bY + i) == 'E') {
				if (getmap(maps, bX, bY + i) == 'Q' || getmap(maps, bX, bY + i) == 'p') {
					gamerA = true;
				}
				else if (getmap(maps, bX, bY + i) == 'W' || getmap(maps, bX, bY + i) == 'o') {
					gamerB = true;
				}
				else if (getmap(maps, bX, bY + i) == 'E') {
					gamerA = true;
					gamerB = true;
				}
			}
		}
		if (!wall[2]) {
			if (getmap(maps, bX - i, bY) == '#' || getmap(maps, bX - i, bY) == 'b' || getmap(maps, bX - i, bY) == 'l' || getmap(maps, bX - i, bY) == '$') {
				wall[2] = true;
			}
			else if (getmap(maps, bX - i, bY) == 'Q' || getmap(maps, bX - i, bY) == 'p' || getmap(maps, bX - i, bY) == 'W' || getmap(maps, bX - i, bY) == 'o' || getmap(maps, bX - i, bY) == 'E') {
				if (getmap(maps, bX - i, bY) == 'Q' || getmap(maps, bX - i, bY) == 'p') {
					gamerA = true;
				}
				else if (getmap(maps, bX - i, bY) == 'W' || getmap(maps, bX - i, bY) == 'o') {
					gamerB = true;
				}
				else if (getmap(maps, bX - i, bY) == 'E') {
					gamerA = true;
					gamerB = true;
				}
			}
		}
		if (!wall[3]) {
			if (getmap(maps, bX + i, bY) == '#' || getmap(maps, bX + i, bY) == 'b' || getmap(maps, bX + i, bY) == 'l' || getmap(maps, bX + i, bY) == '$') {
				wall[3] = true;
			}
			else if (getmap(maps, bX + i, bY) == 'Q' || getmap(maps, bX + i, bY) == 'p' || getmap(maps, bX + i, bY) == 'W' || getmap(maps, bX + i, bY) == 'o' || getmap(maps, bX + i, bY) == 'E') {
				if (getmap(maps, bX + i, bY) == 'Q' || getmap(maps, bX + i, bY) == 'p') {
					gamerA = true;
				}
				else if (getmap(maps, bX + i, bY) == 'W' || getmap(maps, bX + i, bY) == 'o') {
					gamerB = true;
				}
				else if (getmap(maps, bX + i, bY) == 'E') {
					gamerA = true;
					gamerB = true;
				}
			}
		}
	}
	if (gamerA && gamerB) {
		std::cout << "REMIZA" << std::endl;
		end = true;
	}
	else if (gamerA) {
		std::cout << "VYHRAL HRAC B" << std::endl;
		end = true;
		winB++;
	}
	else if (gamerB) {
		std::cout << "VYHRAL HRAC A" << std::endl;
		end = true;
		winA++;
	}
	if (end) {

		countA = 0;
		countB = 0;
		poseA = 0;
		poseB = 0;
		for (int i = 0; i < 3; i++) {
			bombXA[i] = 0;
			bombYA[i] = 0;
			bombXB[i] = 0;
			bombYB[i] = 0;
		}
		std::cout << "SKORE: (A:B) " << winA << ':' << winB << std::endl;
		select_random_map();
		genLabyrinth2();
	}
}

//---------------------------------------------------------------------
// Main
//---------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// Call all initialization.
	init_glfw();
	init_glew();
	gl_print_info();

	local_init();
	local_init_mesh();
	
	// Run until exit is requested.
	while (!glfwWindowShouldClose(globals.window))
	{
		// Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Use ModelView matrix for following trasformations (translate,rotate,scale)
		glMatrixMode(GL_MODELVIEW);
		// Clear all tranformations
		glLoadIdentity();

		DrawAll();

		// Swap front and back buffers 
		// Calls glFlush() inside
		glfwSwapBuffers(globals.window);

		// Check for errors in current frame
		gl_check_error();

		// Poll for and process events
		glfwPollEvents();
	}

	finalize(EXIT_SUCCESS);

}

void reset_projection(void)
{
	float ratio = globals.width * 1.0f / globals.height;

	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(60.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		ratio,			     // Aspect Ratio. Depends on the size of your window.
		0.1f,                // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		20000.0f              // Far clipping plane. Keep as little as possible.
	);

	if (shader_ready)
	{
		GLint currProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);

		// set projection for all shaders
		glUniformMatrix4fv(glGetUniformLocation(currProgram, "uProj_m"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}
}
static void local_init_mesh(void)
{
	select_random_map();
	
	genLabyrinth2();

	mesh_floor = gen_mesh_floor(maps, BLOCK_SIZE);
	
	mesh_brick = gen_mesh_brick(BLOCK_SIZE, "resources/box.png");
	mesh_brick2 = gen_mesh_brick(BLOCK_SIZE, "resources/box2.png");
	if (!loadOBJ(skull, "resources/obj/skull.obj"))
	{
		std::cerr << "loadOBJ failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	gamer1 = gen_mesh_brick(10.0f, "resources/player1.png");
	gamer2 = gen_mesh_brick(10.0f, "resources/player2.png");
	player1.object = gamer1;
	player2.object = gamer2;

	bomb = gen_mesh_brick(6.0f, "resources/houba.png");
	
	camera.movement_speed = 2.0f;	
	camera.radius = BLOCK_SIZE / 5.0f;
	globals.camera = &camera;
}
static void local_init(void)
{
	//
	// OpenGL settings
	// 

	glClearColor(0.2f, 0.2f, 0.4f, 0.0f);               // color-buffer clear colour
	glEnable(GL_CULL_FACE);  // disable draw of back face
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);                        // set Gouraud shading

	glEnable(GL_DEPTH_TEST);                        // enable depth test  
	glPolygonMode(GL_FRONT, GL_FILL);       // enable polygon fill

	GLfloat light_position[] = {0.0f, 0.0f, 45.0f, 1.0f};
	GLfloat light_direction[] = { 0.0, 0.0, -1.0 };
	GLfloat light_color[] = { 1.0f, 0.0f, 0.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);			// light setup 
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 12.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);

	GLfloat light_ambient[] = { 1.0f, 1.0f, 1.0f };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	{
		if (!glewIsExtensionSupported("GL_ARB_compatibility"))
			std::cerr << ":-(" << std::endl;

		//####===---       SHADERS         ---===###//
		std::vector<GLuint> shaders_ids;
		shaders_ids.push_back(compile_shader("resources/shaders/simple.vert", GL_VERTEX_SHADER));
		shaders_ids.push_back(compile_shader("resources/shaders/simple.frag", GL_FRAGMENT_SHADER));
		shader_id = link_shader(shaders_ids);
	}
}
void error_callback(int error, const char* description) {
	std::cerr << "Error: " << description << std::endl;
};
void fbsize_callback(GLFWwindow* window, int width, int height) {
	// check for limit case (prevent division by 0)
	if (height == 0)
		height = 1;

	float ratio = (float)width / (float)height;

	globals.width = width;
	globals.height = height;

	glMatrixMode(GL_PROJECTION);				// set projection matrix for following transformations

	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(45.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		ratio,			     // Aspect Ratio. Depends on the size of your window.
		0.1f,                // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		20000.0f              // Far clipping plane. Keep as little as possible.
	);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));

	glViewport(0, 0, width, height);
};
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	PlayerAvatar a = player1;
	PlayerAvatar o = player2;
	int kvadrantX, kvadrantY;
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_Q:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_UP:
			o = avatarMoveForward(o);
			break;
		case GLFW_KEY_W:
			a = avatarMoveForward(a);
			break;
		case GLFW_KEY_DOWN:
			o = avatarMoveBackward(o);
			break;
		case GLFW_KEY_S:
			a = avatarMoveBackward(a);
			break;
		case GLFW_KEY_LEFT:
			o = avatarMoveLeft(o);
			break;
		case GLFW_KEY_A:
			a = avatarMoveLeft(a);
			break;
		case GLFW_KEY_RIGHT:
			o = avatarMoveRight(o);
			break;
		case GLFW_KEY_D:
			a = avatarMoveRight(a);
			break;
		case GLFW_KEY_1:
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			break;
		case GLFW_KEY_2:
			glfwSetWindowMonitor(window, nullptr, 50, 50, 640, 480, GLFW_DONT_CARE);
			break;
		case GLFW_KEY_M:
			//print map
			int i, j;
			for (j = 0; j < maps.rows; j++) {
				for (i = 0; i < maps.cols; i++) {
					std::cout << getmap(maps, i, j);
				}
				std::cout << std::endl;
			}
			break;
		case GLFW_KEY_L:
			if (enableLight == 2) enableLight = -1;
			enableLight++;
			std::cout << "Light: " << enableLight << std::endl;
			break;
		case GLFW_KEY_B:
			int kvadrantX, kvadrantY;
			int bXA, bYA;
			kvadrantX = ceilf(a.posX / 10);
			kvadrantY = ceilf(a.posY / 10);
			if (getmap(maps, abs(kvadrantX), abs(kvadrantY)) != 'E' && getmap(maps, abs(kvadrantX), abs(kvadrantY)) != 'Q') {
				if (countA < 3) {
					kvadrantX = ceilf(a.posX / 10);
					kvadrantY = ceilf(a.posY / 10);
					maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'b';
					bXA = abs(kvadrantX);
					bYA = abs(kvadrantY);
					bombXA[poseA] = bXA;
					bombYA[poseA] = bYA;
					countA += 1;
					poseA += 1;
				}
			}
			break;
		case GLFW_KEY_I:
			int kvadrantXB, kvadrantYB;
			int bXB, bYB;
			kvadrantX = ceilf(o.posX / 10);
			kvadrantY = ceilf(o.posY / 10);
			if (getmap(maps, abs(kvadrantX), abs(kvadrantY)) != 'E' && getmap(maps, abs(kvadrantX), abs(kvadrantY)) != 'W') {
				if (countB < 3) {
					kvadrantXB = ceilf(o.posX / 10);
					kvadrantYB = ceilf(o.posY / 10);
					maps.at<uchar>(cv::Point(abs(kvadrantXB), abs(kvadrantYB))) = 'l';
					bXB = abs(kvadrantXB);
					bYB = abs(kvadrantYB);
					bombXB[poseB] = bXB;
					bombYB[poseB] = bYB;
					countB += 1;
					poseB += 1;
				}
			}
			break;
		case GLFW_KEY_R:
			countA = 0;
			countB = 0;
			poseA = 0;
			poseB = 0;
			for (int i = 0; i < 3; i++) {
				bombXA[i] = 0;
				bombYA[i] = 0;
				bombXB[i] = 0;
				bombYB[i] = 0;
			}
			select_random_map();
			genLabyrinth2();
			reset = true;
			break;
		default:
			break;
		}
		if (!reset)
		{
			//Kolize A
			if (collision(&a) == '.' || collision(&a) == 'p') {
				kvadrantX = ceilf(player1.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player1.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = '.';
				player1 = a;
				kvadrantX = ceilf(player1.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player1.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'p';
			}
			else if (collision(&a) == 'b') {
				kvadrantX = ceilf(player1.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player1.posY / BLOCK_SIZE);
				if (getmap(maps, abs(kvadrantX), abs(kvadrantY)) == 'p') maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = '.';
				if (getmap(maps, abs(kvadrantX), abs(kvadrantY)) == 'Q') maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'b';
				player1 = a;
				kvadrantX = ceilf(player1.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player1.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'Q';
			}
			else if (collision(&a) == 'Q' || collision(&a) == 'E')
				player1 = a;
			else if (collision(&a) == 'o') {
				kvadrantX = ceilf(player1.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player1.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = '.';
				player1 = a;
				kvadrantX = ceilf(player1.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player1.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'E';
			}



			if ((collision(&a) != 'Q') && ((collision(&a) == '.' || collision(&a) == 'p'))) {
				player1 = a;
				for (int i = 0; i < 3; i++) {
					if (bombXA[i] != 0) maps.at<uchar>(cv::Point(bombXA[i], bombYA[i])) = 'b';
				}
			}
			else if ((collision(&a) != 'E') && ((collision(&a) == '.' || collision(&a) == 'p'))) {
				kvadrantX = ceilf(player1.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player1.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'o';
				player1 = a;
				kvadrantX = ceilf(player1.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player1.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'p';
			}



			//Kolize B
			if (collision(&o) == '.' || collision(&o) == 'o') {
				kvadrantX = ceilf(player2.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player2.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = '.';
				player2 = o;
				kvadrantX = ceilf(player2.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player2.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'o';
			}
			else if (collision(&o) == 'l') {
				kvadrantX = ceilf(player2.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player2.posY / BLOCK_SIZE);
				if (getmap(maps, abs(kvadrantX), abs(kvadrantY)) == 'o') maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = '.';
				if (getmap(maps, abs(kvadrantX), abs(kvadrantY)) == 'W') maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'l';
				player2 = o;
				kvadrantX = ceilf(player2.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player2.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'W';
			}
			else if (collision(&o) == 'W' || collision(&o) == 'E')
				player2 = o;
			else if (collision(&o) == 'p') {
				kvadrantX = ceilf(player2.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player2.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = '.';
				player2 = o;
				kvadrantX = ceilf(player2.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player2.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'E';
			}



			if ((collision(&o) != 'W') && ((collision(&o) == '.' || collision(&o) == 'o'))) {
				player2 = o;
				for (int i = 0; i < 3; i++) {
					if (bombXB[i] != 0) maps.at<uchar>(cv::Point(bombXB[i], bombYB[i])) = 'l';
				}
			}
			else if ((collision(&o) != 'E') && ((collision(&o) == '.' || collision(&o) == 'o'))) {
				kvadrantX = ceilf(player2.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player2.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'p';
				player2 = o;
				kvadrantX = ceilf(player2.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player2.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'o';
			}
			


			if (collision(&a) == '.' || collision(&a) == 'p') {
				kvadrantX = ceilf(player1.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player1.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = '.';
				player1 = a;
				kvadrantX = ceilf(player1.posX / BLOCK_SIZE);
				kvadrantY = ceilf(player1.posY / BLOCK_SIZE);
				maps.at<uchar>(cv::Point(abs(kvadrantX), abs(kvadrantY))) = 'p';
			}



			//Bomby
			if (countA == 3) {
				poseA = poseA % 3;
				countA -= 1;
				Boooom(bombXA[poseA], bombYA[poseA]);
				maps.at<uchar>(cv::Point(bombXA[poseA], bombYA[poseA])) = '.';
				bombXA[poseA] = 0;
				bombYA[poseA] = 0;
				maps.at<uchar>(cv::Point(0, 0)) = '#';
			}



			if (countB == 3) {
				poseB = poseB % 3;
				countB -= 1;
				Boooom(bombXB[poseB], bombYB[poseB]);
				maps.at<uchar>(cv::Point(bombXB[poseB], bombYB[poseB])) = '.';
				bombXB[poseB] = 0;
				bombYB[poseB] = 0;
				maps.at<uchar>(cv::Point(0, 0)) = '#';
			}
		}
		reset = false;
	}
};
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	static int first = 1;
	static double old_x;
	static double old_y;
	
	
	if (first) {
		old_x = xpos;
		old_y = ypos;
		first = 0;
	}else if (left_click) {
		if (xpos > old_x) globals.camera->posX += 1;
		if (xpos < old_x) globals.camera->posX -= 1;
		if (xpos > old_y) globals.camera->posY += 1;
		if (xpos < old_y) globals.camera->posY -= 1;
		
		old_x = xpos;
		old_y = ypos;

	}
};
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		left_click = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		left_click = false;
	}
};
