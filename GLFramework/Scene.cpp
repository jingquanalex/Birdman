#include "Scene.h"

using namespace glm;
using namespace std;

extern int window_width;
extern int window_height;

Scene::Scene(Camera* camera, Cursor* cursor)
{
	this->camera = camera;
	this->cursor = cursor;
}

Scene::~Scene()
{
	
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

	npc = new NPC(vec3(), NPCTYPE::RED);
	coin = new Sprite("media\\img\\coin.png", vec3(), vec2(32, 32));

	// Populate map with coins and npcs
	for (Item item : tilemap->getListItems())
	{
		if (item.type == -2)
		{
			Sprite* newCoin = new Sprite(coin, item.position);
			newCoin->setupAnimation(vec2(32, 32), 0.1f);
			newCoin->startAnimation();
			listCoins.push_back(newCoin);
		}
		else if (item.type == -3)
		{
			NPC* newNpc = new NPC(npc, item.position);
			newNpc->setupMapCollision(tilemap);
		}
	}

	camera->setPosition(guy->getPosition());

	stateLoaded = 1;
}

void Scene::update(float dt)
{
	if (!stateLoaded) return;

	// Prevent dt spike during loading.
	if (stateLoaded == 1)
	{
		dt = 0.001f;
		stateLoaded = 2;
	}

	background->update(dt);
	guy->update(dt);
	NPC::updateNPCs(dt);

	camera->moveTo(guy->getPosition() + vec3(0, 50, 0));

	// Character collision logics
	for (NPC* npc : NPC::getListNPCs())
	{
		if (!guy->getIsInvuln() && npc->getIsThreat() && guy->isCollidingWith(npc) && npc->getType() == NPCTYPE::RED)
		{
			// Guy can only kill the npc if he is falling on top of it.
			float guyPosY = guy->getPosition().y + guy->getBoundingRect().y;
			float npcPosY = npc->getPosition().y;
			if (npc->getState() != NPCSTATE::STOMPED && guyPosY >= npcPosY && guy->getVelocity().y <= 0.0f)
			{
				npc->stomped();
				guy->bounce();
			}
			else
			{
				guy->damageTaken();
			}
		}
	}

	// Update coin list, if collected, set visible to 0.
	for_each(listCoins.begin(), listCoins.end(), [=](Sprite*& coin) {
		if (!coin->getVisibie())
		{
			delete coin;
			coin = NULL;
		}
		else
		{
			if (guy->isCollidingWith(coin))
			{
				coin->setVisible(0);
			}

			coin->update(dt);
		}
	});
	listCoins.erase(remove(listCoins.begin(), listCoins.end(), static_cast<Sprite*>(NULL)), listCoins.end());
}

void Scene::draw()
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

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		NPC* newNpc = new NPC(npc, cursor->getPosition());
		newNpc->setupMapCollision(tilemap);
	}
}

void Scene::keyboard(unsigned char key)
{
	if (!stateLoaded) return;

	guy->keyboard(key);

	if (key == 'g')
	{
		
	}
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