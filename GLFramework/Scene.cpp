#include "Scene.h"

using namespace glm;
using namespace std;

extern int window_width;
extern int window_height;

FTGLPixmapFont fontCredit1 = FTGLPixmapFont("media\\pixel.ttf");
FTGLPixmapFont fontCredit2 = FTGLPixmapFont("media\\pixel.ttf");
FTGLPixmapFont fontStartGame = FTGLPixmapFont("media\\pixel.ttf");
FTGLPixmapFont fontStartGame2 = FTGLPixmapFont("media\\pixel.ttf");

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
	npcBlue = new NPC(vec3(), NPCTYPE::BLUE);
	coin = new Sprite("media\\img\\coin.png", vec3(), vec2(32, 32));

	blanket = new Sprite("", vec3(), vec2(window_width, window_height), true);
	blanket->attachTo(camera);
	blanket->setZOrder(0.9f);
	fadeTimer = new Timer(0.01f);
	fadeTimer->start();
	spawnTimer = new Timer(0.5f);

	fontCredit1.FaceSize(100);
	fontCredit2.FaceSize(50);
	fontStartGame.FaceSize(100);
	fontStartGame2.FaceSize(40);

	resetScene();

	stateLoaded = 1;
}

void Scene::resetScene()
{
	if (hardMode) spawnTimer->start();
	gameWon = 0;
	stateLoaded = 1;
	guy->reset();

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
		if (item.type == "c")
		{
			Sprite* newCoin = new Sprite(coin, item.position);
			newCoin->setupAnimation(vec2(32, 32), 0.1f);
			newCoin->startAnimation();
			listCoins.push_back(newCoin);
		}
		else if (item.type == "r")
		{
			NPC* newNpc = new NPC(npc, item.position);
			newNpc->setupMapCollision(tilemap);
		}
		else if (item.type == "ir")
		{
			NPC* newNpc = new NPC(npc, item.position);
			newNpc->setIdle(1);
		}
		else if (item.type == "b")
		{
			NPC* newNpc = new NPC(npcBlue, item.position);
			newNpc->setupMapCollision(tilemap);
		}
		else if (item.type == "ib")
		{
			NPC* newNpc = new NPC(npcBlue, item.position);
			newNpc->setIdle(1);
		}
	}

	guy->setPosition(guyStartPosition);
	camera->setPosition(camStartPosition);
}

void Scene::gameOver()
{
	if (gameState == 2)
	{
		gameState = 3;
		fadeTimer->start();
	}
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

	blanket->setSize(vec2(window_width, window_height));
	blanket->update(dt);

	background->setSize(vec2(window_width, window_height));
	background->update(dt);

	if (stateLoaded > 2)
	{
		guy->update(dt);

		if (stateLoaded == 3 && guy->getIsOnPlatform())
		{
			stateLoaded = 4;
		}

		if (stateLoaded == 4)
		{
			camera->moveTo(guy->getPosition() + vec3(0, 50, 0));
		}
	}

	NPC::updateNPCs(dt);

	// Character collision logics
	for (NPC* npc : NPC::getListNPCs())
	{
		// If npc fall off map, destroy
		if (npc->getPosition().y < tilemap->getEndBounds().y)
		{
			npc->destroy();
		}

		if (!guy->getIsInvuln() && npc->getIsThreat() && guy->isCollidingWith(npc))
		{
			if (npc->getType() == NPCTYPE::RED)
			{
				// Guy can only kill the npc if he is falling on top of it.
				float guyPosY = guy->getPosition().y + guy->getBoundingRect().y / 2;
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
					spawnNpcNext++;
				}
			}
			else
			{
				guy->damageTaken();

				if (hardMode)
				{
					spawnNpcNext++;
				}
			}
		}

		for (Projectile* projectile : Projectile::getListProjectiles())
		{
			if (npc->getIsThreat() && projectile->isCollidingWith(npc))
			{
				if (npc->getState() == NPCSTATE::IDLE)
				{
					npc->setHasGravity(1);
				}

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

	// Spawn npc if damage taken
	for (int i = 0; i < spawnNpcNext; i++)
	{
		NPC* newNpc = new NPC(npcBlue, guy->getPosition() + vec3(rand() % 100 - 50, 0, 0));
		newNpc->setupMapCollision(tilemap);
	}
	spawnNpcNext = 0;

	// Randomly spawn NPC in hardmode
	if (spawnTimer->hasTicked())
	{
		if (rand() % 2)
		{
			// Don't spawn inside tilemap walls
			vec3 spawnPos = guy->getPosition() + vec3(rand() % 1000 - 500, 500, 0);
			if (tilemap->getValueAtPos(spawnPos) <= 0)
			{
				NPC* newNpc = new NPC(npc, spawnPos);
				newNpc->setupMapCollision(tilemap);
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

	// Player took 4 hits, reset
	if (guy->getColor().g < 0.0f)
	{
		gameOver();
	}

	// Player fall off map, reset
	if (guy->getPosition().y < tilemap->getEndBounds().y)
	{
		gameOver();
	}

	// Player hit objective, display win.
	if (!gameWon && distance2(guy->getPosition(), vec3(2527, -235, 0)) < 955.0f)
	{
		gameWon = 1;
		hardMode = 1;
		guy->freeze();
		guy->addScore(1000);
	}

	// Fade in/out and reset scene
	if (gameState == 1)
	{
		if (fadeTimer->hasTicked())
		{
			blanket->setAlpha(blanket->getAlpha() - dt);
			if (blanket->getAlpha() <= 0.0f)
			{
				gameState = 2;
				fadeTimer->stop();
			}
		}
	}
	else if (gameState == 3)
	{
		if (fadeTimer->hasTicked())
		{
			blanket->setAlpha(blanket->getAlpha() + dt);
			if (blanket->getAlpha() >= 1.0f)
			{
				gameState = 1;
				resetScene();
			}
		}
	}
}

void Scene::draw()
{
	if (!stateLoaded) return;

	Sprite::drawSprites();

	// UI
	if (stateLoaded != 4)
	{
		glPixelTransferf(GL_RED_BIAS, -1.0f);
		glPixelTransferf(GL_GREEN_BIAS, 1.0f);
		glPixelTransferf(GL_BLUE_BIAS, 1.0f);
		fontCredit1.Render("BirdMan");
		fontCredit2.Render("by Alex Zhang", -1, FTPoint(400, 20, 0));
		glPixelTransferf(GL_RED_BIAS, -1.0f);
		glPixelTransferf(GL_GREEN_BIAS, -1.0f);
		glPixelTransferf(GL_BLUE_BIAS, -1.0f);
		fontStartGame.Render("START", -1, FTPoint(window_width / 2 - 100, window_height / 2 + 150, 0));
		fontStartGame2.Render("Press Space To", -1, FTPoint(window_width / 2 - 120, window_height - 100, 0));
		fontStartGame2.Render("Collect->", -1, FTPoint(window_width / 2 - 220, window_height / 2 + 10, 0));
		fontStartGame2.Render("Collect->", -1, FTPoint(window_width / 2 - 220, window_height / 2 - 60, 0));
		fontStartGame2.Render("Stomp->", -1, FTPoint(window_width / 2 - 220, window_height / 2 - 130, 0));
		fontStartGame2.Render("Avoid->", -1, FTPoint(window_width / 2 + 250, window_height / 2 + 130, 0));
	}
	else if (stateLoaded == 4)
	{
		glPixelTransferf(GL_RED_BIAS, 1.0f);
		glPixelTransferf(GL_GREEN_BIAS, -1.0f);
		glPixelTransferf(GL_BLUE_BIAS, 1.0f);
		stringstream ss;
		ss << "Coins Collected: " << guy->getCoinsCollected();
		fontStartGame2.Render(ss.str().c_str(), -1, FTPoint(100, window_height - 100, 0));
		ss.str("");
		ss.clear();
		ss << "Monster Stomped: " << guy->getNpcsKilled();
		fontStartGame2.Render(ss.str().c_str(), -1, FTPoint(window_width - 525, window_height - 100, 0));

		if (gameWon)
		{
			glPixelTransferf(GL_RED_BIAS, 1.0f);
			glPixelTransferf(GL_GREEN_BIAS, 1.0f);
			glPixelTransferf(GL_BLUE_BIAS, -1.0f);
			fontCredit1.Render("You Win!");
			ss.str("");
			ss.clear();
			ss << "Score: " << guy->getCoinsCollected() + guy->getNpcsKilled();
			fontCredit2.Render(ss.str().c_str(), -1, FTPoint(600, 20, 0));
		}
	}

	blanket->draw();
}

void Scene::mouse(int button, int state)
{
	if (!stateLoaded) return;
	if (!devMode) return;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//printf("%i, %i \n", (int)cursor->getPosition().x, (int)cursor->getPosition().y);
		//camera->moveTo(cursor->getGlobalPosition());
		guy->setPosition(cursor->getGlobalPosition());
		guy->setVelocity(vec3());
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		NPC* newNpc = new NPC(npc, cursor->getPosition());
		newNpc->setupMapCollision(tilemap);
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		NPC* newNpc = new NPC(npcBlue, cursor->getPosition());
		newNpc->setupMapCollision(tilemap);
	}
}

void Scene::keyboard(unsigned char key)
{
	if (!stateLoaded) return;

	// Enable player controls only when game is begun and player is on the ground.
	if (stateLoaded == 4)
	{
		guy->keyboard(key);
	}

	if (key == KEY_SPACE)
	{
		// Set to 3 to start game
		if (stateLoaded == 2)
		{
			stateLoaded = 3;
			guy->jump();
		}
	}
	else if (key == '1')
	{
		gameOver();
	}
	else if (key == '0')
	{
		devMode = 1;
		cursor->setVisible(1);
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

	if (stateLoaded == 4)
	{
		guy->keyboardSpecial(key);
	}
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