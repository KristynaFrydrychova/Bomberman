#pragma once
#include "globals.h"

void error_callback(int error, const char* description);
void fbsize_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
