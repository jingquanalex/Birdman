#include <iostream>
#include <thread>
#include "globalIncludes.h"
#include "Camera.h"
#include "Cursor.h"
#include "Scene.h"

std::string g_windowTitle = "GLFramework";

// Desired content display area, which maintains its ratio even if the window area is changed.
// For Camera's ortho matrix but unused at the moment.
float g_content_width = 1280;
float g_content_height = 720;

// Window display area.
int window_width = 1280;
int window_height = 800;

float mousex, mousey;

// Store frame deltatime and frames per second.
float dt;
int fps = 0;
int previousTime = 0;

// Framework classes
Camera* camera;
Cursor* cursor;
Scene* scene;
Timer* fpsTimer;