#include "Scene.h"

using namespace glm;
using namespace std;

extern int window_width;
extern int window_height;

FTGLPixmapFont fontCredit1 = FTGLPixmapFont("media\\pixel.ttf");
FTGLPixmapFont fontCredit2 = FTGLPixmapFont("media\\pixel.ttf");
FTGLPixmapFont fontStartGame = FTGLPixmapFont("media\\pixel.ttf");
FTGLPixmapFont fontStartGame2 = FTGLPixmapFont("media\\pixel.ttf");
FTGLPixmapFont fontCheater = FTGLPixmapFont("media\\pixel.ttf");

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
	bgBrown = new Sprite("media\\img\\bg_brown.jpg", vec3(), vec2(window_width, window_height));
	bgBrown->setZOrder(-900.0f);
	bgBrown->attachTo(camera);
	bgBrown->setVisible(0);

	bgGreen = new Sprite("media\\img\\bg_green.jpg", vec3(), vec2(window_width, window_height));
	bgGreen->setZOrder(-900.0f);
	bgGreen->attachTo(camera);

	tilemap = new Tilemap("media\\img\\tileset.png", vec2(32, 32), "media\\map1.txt", vec2(64, 64));
	tilemap->setZOrder(-890.0f);

	guy = new Guy(guyStartPosition);
	guy->setupMapCollision(tilemap);

	npc = new NPC(vec3(), NPCTYPE::RED);
	npcBlue = new NPC(vec3(), NPCTYPE::BLUE);
	coin = new Sprite("media\\img\\coin.png", vec3(), vec2(32, 32));

	blanket = new Sprite("media\\img\\white.png", vec3(), vec2(window_width, window_height), true);
	blanket->attachTo(camera);
	blanket->setZOrder(0.9f);
	fadeTimer = new Timer(0.01f);
	fadeTimer->start();
	spawnTimer = new Timer(0.5f);
	gameTimer = new Timer(1.0);

	fontCredit1.FaceSize(100);
	fontCredit2.FaceSize(50);
	fontStartGame.FaceSize(100);
	fontStartGame2.FaceSize(40);
	fontCheater.FaceSize(300);

	resetScene();

	stateLoaded = 1;
}

void Scene::resetScene()
{
	gameWon = 0;
	stateLoaded = 1;
	guy->reset();
	secondsElasped = 0;
	//hardMode = 1;
	//gameWon = 1;

	if (hardMode)
	{
		bgBrown->setVisible(1);
		bgGreen->setVisible(0);
		spawnTimer->start();
		guy->addScore(1000);
	}

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

	bgBrown->setSize(vec2(window_width, window_height));
	bgBrown->update(dt);
	bgGreen->setSize(vec2(window_width, window_height));
	bgGreen->update(dt);

	if (stateLoaded > 2)
	{
		guy->update(dt);

		// Start the game when player hit the platform
		if (stateLoaded == 3 && guy->getIsOnPlatform())
		{
			stateLoaded = 4;
		}

		// Game is running
		if (stateLoaded == 4)
		{
			camera->moveTo(guy->getPosition() + vec3(0, 50, 0));

			if (gameTimer->hasTicked())
			{
				secondsElasped++;
			}
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
	if (spawnTimer->hasTicked() && !gameWon)
	{
		if (rand() % 2)
		{
			// Don't spawn inside tilemap walls
			vec3 spawnPos = guy->getPosition() + vec3(rand() % 1000 - 500, 500, 0);
			if (tilemap->getValueAtPos(spawnPos) <= 0)
			{
				NPC* newNpc = new NPC(npc, spawnPos);
				if (rand() % 2)
				{
					newNpc->moveLeft();
				}
				else
				{
					newNpc->moveRight();
				}
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
		gameTimer->stop();
	}

	// Fade in/out and reset scene
	if (gameState == 1)
	{
		if (fadeTimer->hasTicked())
		{
			blanket->setAlpha(blanket->getAlpha() - 2 * dt);
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
			blanket->setAlpha(blanket->getAlpha() + 2 * dt);
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
		fontStartGame2.Render(ss.str().c_str(), -1, FTPoint(90, window_height - 100, 0));
		ss.str("");
		ss.clear();
		ss << "Monster Stomped: " << guy->getNpcsKilled();
		fontStartGame2.Render(ss.str().c_str(), -1, FTPoint(window_width - 525, window_height - 100, 0));
		ss.str("");
		ss.clear();
		ss << "Time: " << secondsElasped;
		fontStartGame2.Render(ss.str().c_str(), -1, FTPoint(window_width / 2 - 100, window_height - 100, 0));

		if (gameWon)
		{

			glPixelTransferf(GL_RED_BIAS, 1.0f);
			glPixelTransferf(GL_GREEN_BIAS, 1.0f);
			glPixelTransferf(GL_BLUE_BIAS, -1.0f);
			fontCredit1.Render("You Win!");
			if (bgGreen->getVisibie())
			{
				fontCredit2.Render("Press 1 for HardMode.", -1, FTPoint(450, 0, 0));
			}
			else
			{
				fontCredit2.Render("Press 1 to play again.", -1, FTPoint(450, 0, 0));
			}

			ss.str("");
			ss.clear();
			glPixelTransferf(GL_RED_BIAS, -1.0f);
			glPixelTransferf(GL_GREEN_BIAS, -1.0f);
			glPixelTransferf(GL_BLUE_BIAS, -1.0f);
			ss << "Score: " << guy->getScore();
			fontCredit2.Render(ss.str().c_str(), -1, FTPoint(window_width / 2 - 150, window_height / 2 - 340, 0));
			ss.str("");
			ss.clear();
			ss << "Monster Stomped:+" << guy->getNpcsKilled();
			fontCredit2.Render(ss.str().c_str(), -1, FTPoint(window_width / 2 - 463, window_height / 2 - 300, 0));
			ss.str("");
			ss.clear();
			ss << "Coins Collected:+" << guy->getCoinsCollected();
			fontCredit2.Render(ss.str().c_str(), -1, FTPoint(window_width / 2 - 387, window_height / 2 - 260, 0));

			if (bgBrown->getVisibie())
			{
				fontCredit2.Render("HardMode:+1000", -1, FTPoint(window_width / 2 - 257, window_height / 2 - 220, 0));
			}

			if (devMode)
			{
				glPixelTransferf(GL_RED_BIAS, 1.0f);
				glPixelTransferf(GL_GREEN_BIAS, -0.5f);
				glPixelTransferf(GL_BLUE_BIAS, -0.7f);
				fontCheater.Render("CHEATER", -1, FTPoint(window_width / 2 - 550, window_height / 2 - 300, 0));
				fontCheater.Render("CHEATER", -1, FTPoint(window_width / 2 - 550, window_height / 2 - 100, 0));
				fontCheater.Render("CHEATER", -1, FTPoint(window_width / 2 - 550, window_height / 2 + 100, 0));
			}
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
		// Start the game
		if (stateLoaded == 2)
		{
			stateLoaded = 3;
			guy->jump();
			gameTimer->start();
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