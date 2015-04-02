#include "main.h"

using namespace std;

// Initial scene load.
void init()
{
	camera = new Camera(window_width, window_height);
	cursor = new Cursor();
	scene = new Scene(camera, cursor);
	fpsTimer = new Timer(1.0f);
	fpsTimer->start();

	scene->load();
}

// Update loop for game logic and physics.
void idle()
{
	// Calculate dt and fps
	int elaspedTime = glutGet(GLUT_ELAPSED_TIME);
	dt = (elaspedTime - previousTime) / 1000.0f;
	previousTime = elaspedTime;

	fps++;
	if (fpsTimer->hasTicked())
	{
		stringstream title;
		title << g_windowTitle << " [" << fps << "]";
		glutSetWindowTitle(title.str().c_str());

		fps = 0;
	}

	Timer::updateTimers(dt);

	cursor->update(dt, camera, mousex, mousey);
	scene->update(dt);
	camera->update(dt);
	
	glutPostRedisplay();
}

// Update loop for rendering.
void display()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(camera->getMatView()));

	Sprite::drawSprites();

	glutSwapBuffers();
	glFinish();
}

// Window dimension changed, recreate camera proj matrix.
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	camera->setResolution(width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(camera->getMatProjection()));
}

// Mouse buttons
void mouse(int button, int state, int x, int y)
{
	scene->mouse(button, state);
}

// Mouse moved
void mouseMove(int x, int y)
{
	mousex = (float)x;
	mousey = (float)y;
}

// Function, arrow and other special keys.
void keyboardSpecial(int key, int x, int y)
{
	//camera->keyboardSpecial(key);
	scene->keyboardSpecial(key);
}

// Special key up event.
void keyboardSpecialUp(int key, int x, int y)
{
	//camera->keyboardSpecialUp(key);
	scene->keyboardSpecialUp(key);
}

// Common ascii keys.
void keyboard(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
	//else if (key == 'l') scene->load();
	scene->keyboard(key);
}

// Common keys up event.
void keyboardUp(unsigned char key, int x, int y)
{
	scene->keyboardUp(key);
}

int main(int argc, char** argv)
{
	// Initialize glut and create window
	glutInit(&argc, argv);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(window_width, window_height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(g_windowTitle.c_str());
	glewInit();

	init();

	// Callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(keyboardSpecial);
	glutSpecialUpFunc(keyboardSpecialUp);

	glutMainLoop();

	return 0;
}