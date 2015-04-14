#include "NPC.h"

using namespace std;
using namespace glm;

vector<NPC*> NPC::listNPCs;

vector<NPC*>& NPC::getListNPCs()
{
	return listNPCs;
}

void NPC::updateNPCs(float dt)
{
	for_each(listNPCs.begin(), listNPCs.end(), [dt](NPC*& npc) {
		if (npc->getIsDead())
		{
			delete npc;
			npc = NULL;
		}
		else
		{
			npc->update(dt);
		}
	});
	listNPCs.erase(remove(listNPCs.begin(), listNPCs.end(), static_cast<NPC*>(NULL)), listNPCs.end());
}

NPC::NPC(NPC* npc, vec3 position) : Character(npc, position)
{
	this->type = npc->getType();

	jumpSpeed = 600.0f;
	moveSpeed = 350.0f;
	
	if (type == NPCTYPE::RED)
	{
		setupAnimation(vec2(64, 64), 0.1f, 18, 21);
		setupCollision(vec2(50, 45), vec3(0, -6, 0));
	}
	else
	{
		setupAnimation(vec2(64, 64), 0.1f, 54, 57);
		setupCollision(vec2(40, 55), vec3(0, -1, 0));
	}

	startAnimation();
	//setBoundingRectVisible(1);

	state = NPCSTATE::MOVING;
	// Randomly start moving left or right
	isMovingLeft = 1;

	// Duration of sprite stomped state, before fading state.
	stompTimer = new Timer(0.0f, 1.0f);
	fadeTimer = new Timer(0.1f, 0.0f);

	listNPCs.push_back(this);
}

NPC::NPC(vec3 position, NPCTYPE type) : Character("media\\img\\char.png", position, vec2(64, 64))
{
	this->type = type;
}

NPC::~NPC()
{
	stompTimer->destroy();
	fadeTimer->destroy();
}

void NPC::update(float dt)
{
	if (state == NPCSTATE::IDLE)
	{
		isThreat = 1;
		isMovingLeft = 0;
		isMovingRight = 0;
		if (type == NPCTYPE::RED)
		{
			setFrameRange(6, 6);
		}
		else
		{
			setFrameRange(42, 42);
		}
	}
	else if (state == NPCSTATE::MOVING)
	{
		isThreat = 1;
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
		isThreat = 0;
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
			alpha -= 0.05f;
			if (alpha <= 0.0f)
			{
				isDead = 1;
			}
		}
	}

	Character::update(dt);
}

void NPC::stomped()
{
	if (isThreat)
	{
		state = NPCSTATE::STOMPED;
		stompTimer->start();
	}
}

void NPC::destroy()
{
	isDead = 1;
}

void NPC::setIdle(bool idle)
{
	if (idle)
	{
		isMovingLeft = 0;
		isMovingRight = 0;
		state = NPCSTATE::IDLE;
		hasGravity = 0;
	}
}

void NPC::moveLeft()
{
	isMovingLeft = 1;
	isMovingRight = 0;
}

void NPC::moveRight()
{
	isMovingLeft = 0;
	isMovingRight = 1;
	isFlippedX = 1;
}

NPCSTATE NPC::getState() const
{
	return state;
}

NPCTYPE NPC::getType() const
{
	return type;
}

bool NPC::getIsThreat() const
{
	return isThreat;
}