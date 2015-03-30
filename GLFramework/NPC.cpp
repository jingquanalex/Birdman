#include "NPC.h"

using namespace std;
using namespace glm;

vector<NPC*> NPC::listNPCs;

NPC::NPC(vec3 position, NPCTYPE type) : Character("media\\img\\char.png", position, vec2(64, 64))
{
	listNPCs.push_back(this);

	jumpSpeed = 600.0f;
	moveSpeed = 350.0f;
	if (type == NPCTYPE::RED)
	{
		setupAnimation(vec2(64, 64), 0.1f, 18, 21);
	}
	else
	{
		setupAnimation(vec2(64, 64), 0.1f, 54, 57);
	}
	startAnimation();
	setupCollision(vec2(38, 50), vec3(0, -5, 0));
	//setBoundingRectVisible(1);

	isNPC = 1;
	state = NPCSTATE::MOVING;
	isMovingRight = 1;

	stompTimer = new Timer(0.0f, 2.0f);
	fadeTimer = new Timer(0.1f, 4.0f);
}

void NPC::update(float dt)
{
	Character::update(dt);

	if (state == NPCSTATE::MOVING)
	{
		if (isMovingLeft && collidingX == -1)
		{
			isMovingLeft = 0;
			isMovingRight = 1;
			isFlippedX = 1;
		}
		else if (isMovingRight && collidingX == 1)
		{
			isMovingLeft = 1;
			isMovingRight = 0;
			isFlippedX = 0;
		}
	}
	else if (state == NPCSTATE::STOMPED)
	{
		isMovingLeft = 0;
		isMovingRight = 0;
		setFrameRange(8, 8);

		if (!stompTimer->getIsRunning())
		{
			state = NPCSTATE::FADING;
			fadeTimer->start();
		}
	}
	else if (state == NPCSTATE::FADING)
	{
		if (fadeTimer->hasTicked())
		{
			alpha -= 1.0 / (fadeTimer->getDuration() / fadeTimer->getTickInterval());
		}
	}
}

void NPC::destroy()
{
	Character::destroy();
	delete this;
}

void NPC::stomped()
{
	state = NPCSTATE::STOMPED;
	stompTimer->reset();
	stompTimer->start();
}

vector<NPC*> NPC::getListNPCs()
{
	return listNPCs;
}