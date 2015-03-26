#include "Scene.h"

using namespace glm;
using namespace std;

extern int window_width;
extern int window_height;

Sprite* background;
Tilemap* tilemap;
Guy* guy;

Scene::Scene(Camera* camera, Cursor* cursor)
{
	this->camera = camera;
	this->cursor = cursor;
}

void Scene::load()
{
	background = new Sprite("media\\img\\bg_brown.jpg", vec3(), vec2(window_width, window_height));
	background->setZOrder(-90.0f);
	background->attachTo(camera);

	tilemap = new Tilemap("media\\img\\tileset.png", vec2(32, 32), "media\\map1.txt", vec2(64, 64));
	tilemap->setZOrder(-89.0f);

	guy = new Guy(vec3(440, -2155, 0));
	guy->setupMapCollision(tilemap);

	camera->setPosition(guy->getPosition());

	Sprite::sortToZOrder();
}

void Scene::update(float dt)
{
	background->update(dt);
	guy->update(dt);
	camera->moveTo(guy->getPosition() + vec3(0, 50, 0));

	// Handle character-map collision response.

}

void Scene::draw()
{
	
}

void Scene::destroy()
{

}

void Scene::mouse(int button, int state)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("%i, %i \n", (int)cursor->getPosition().x, (int)cursor->getPosition().y);
		//camera->moveTo(cursor->getGlobalPosition());
		guy->setPosition(cursor->getGlobalPosition());
		guy->setVelocity(vec3());
	}
}

void Scene::keyboard(unsigned char key)
{
	guy->keyboard(key);
}

void Scene::keyboardUp(unsigned char key)
{
	guy->keyboardUp(key);
}

void Scene::keyboardSpecial(int key)
{
	guy->keyboardSpecial(key);
}

void Scene::keyboardSpecialUp(int key)
{
	guy->keyboardSpecialUp(key);
}