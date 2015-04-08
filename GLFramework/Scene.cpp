#include "Scene.h"

using namespace glm;
using namespace std;

extern int window_width;
extern int window_height;

FTGLPixmapFont fontCredit1 = FTGLPixmapFont("media\\pixel.ttf");
FTGLPixmapFont fontCredit2 = FTGLPixmapFont("media\\pixel.ttf");
FTGLPixmapFont fontStartGame = FTGLPixmapFont("media\\pixel.ttf");
FTGLPixmapFont fontCoin = FTGLPixmapFont("media\\pixel.ttf");
FTGLPixmapFont fontNpc = FTGLPixmapFont("media\\pixel.ttf");

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
	background->setZOrder(-900.0f);
	background->attachTo(camera);

	tilemap = new Tilemap("media\\img\\tileset.png", vec2(32, 32), "media\\map1.txt", vec2(64, 64));
	tilemap->setZOrder(-890.0f);

	guy = new Guy(guyStartPosition);
	guy->setupMapCollision(tilemap);

	npc = new NPC(vec3(), NPCTYPE::RED);
	coin = new Sprite("media\\img\\coin.png", vec3(), vec2(32, 32));

	fontCredit1.FaceSize(99);
	fontCredit2.FaceSize(49);
	fontStartGame.FaceSize(100);

	resetScene();

	stateLoaded = 1;
}

void Scene::resetScene()
{
	guy->resetScores();

	// Reset sprite initial auto zOrder
	Sprite::resetZOrder();

	// Mark coins for deletion
	for (Sprite* coin : listCoins)
	{
		coin->setVisible(0);
	}

	// Mark npc for deletion
	for (NPC* npc : NPC::getListNPCs())
	{
		npc->destroy();
	}

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

	guy->setPosition(guyStartPosition);
	camera->setPosition(camStartPosition);
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

	background->setSize(vec2(window_width, window_height));
	background->update(dt);

	if (stateLoaded > 2)
	{
		guy->update(dt);
		camera->moveTo(guy->getPosition() + vec3(0, 50, 0));
	}

	NPC::updateNPCs(dt);

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
				guy->killedNpc();
			}
			else
			{
				guy->damageTaken();
			}
		}

		for (Projectile* projectile : Projectile::getListProjectiles())
		{
			if (npc->getIsThreat() && projectile->isCollidingWith(npc))
			{
				if (projectile->getIsFlippedX())
				{
					npc->knockback(1);
				}
				else
				{
					npc->knockback(2);
				}
				projectile->destroy();
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
				guy->collectCoin();
			}

			coin->update(dt);
		}
	});
	listCoins.erase(remove(listCoins.begin(), listCoins.end(), static_cast<Sprite*>(NULL)), listCoins.end());
}

void Scene::draw()
{
	if (!stateLoaded) return;

	Sprite::drawSprites();

	glPushMatrix();
	fontCredit1.Render("BirdMan");
	fontCredit2.Render("by Alex Zhang", -1, FTPoint(400, 0, 0));
	glColor4d(1.0, 0.0, 0.0, 1.0);
	fontStartGame.Render("START", -1, FTPoint(window_width / 2 - 100, window_height / 2 + 150, 0));
	glPopMatrix();
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

	if (key == 'r')
	{
		resetScene();
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