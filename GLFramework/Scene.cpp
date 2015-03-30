#include "Scene.h"

using namespace glm;
using namespace std;

extern int window_width;
extern int window_height;

Sprite* background;
Tilemap* tilemap;
Guy* guy;
NPC* npc;

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

	npc = new NPC(vec3(1430, -2200, 0), NPCTYPE::RED);
	npc->setupMapCollision(tilemap);

	camera->setPosition(guy->getPosition());

	Sprite::sortToZOrder();

	stateLoaded = 1;
}

void Scene::update(float dt)
{
	// Prevent dt spike during loading.
	if (stateLoaded == 1)
	{
		dt = 0.0f;
		stateLoaded = 2;
	}

	if (!stateLoaded) return;

	background->update(dt);
	Character::updateCharacters(dt);
	camera->moveTo(guy->getPosition() + vec3(0, 50, 0));

	// Character collision logics
	for (NPC* npc : NPC::getListNPCs())
	{
		if (guy->isCollidingWith(npc))
		{
			if (guy->getIsOnPlatform())
			{
				guy->damageTaken();
			}
			else
			{
				npc->stomped();
			}
		}
	}
}

void Scene::draw()
{
	if (!stateLoaded) return;
}

void Scene::destroy()
{
	if (!stateLoaded) return;
}

void Scene::mouse(int button, int state)
{
	if (!stateLoaded) return;

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
	if (!stateLoaded) return;

	guy->keyboard(key);
}

void Scene::keyboardUp(unsigned char key)
{
	if (!stateLoaded) return;

	guy->keyboardUp(key);
}

void Scene::keyboardSpecial(int key)
{
	if (!stateLoaded) return;

	guy->keyboardSpecial(key);
}

void Scene::keyboardSpecialUp(int key)
{
	if (!stateLoaded) return;

	guy->keyboardSpecialUp(key);
}

int Scene::getIsLoaded() const
{
	return stateLoaded;
}