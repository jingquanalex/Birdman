#include "Guy.h"

using namespace std;
using namespace glm;

Guy::Guy(vec3 position) : Character("media\\img\\char.png", position, vec2(64, 64))
{
	// Timer for damage taken sprite flash.
	tHurtFlash = new Timer(0.07f, 1.5f);

	jumpSpeed = 1000.0f;
	moveSpeed = 550.0f;
	setupAnimation(vec2(64, 64), 0.1f, 0, 2);
	startAnimation();
	setupCollision(vec2(38, 50), vec3(0, -5, 0));
	//setBoundingRectVisible(1);
}

Guy::~Guy()
{
	delete tHurtFlash;
}

void Guy::update(float dt)
{
	if (tHurtFlash->getIsRunning())
	{
		// Sprite flashing during invuln period.
		if (tHurtFlash->hasTicked())
		{
			if (alpha == 1.0f)
			{
				alpha = 0.3f;
			}
			else
			{
				alpha = 1.0f;
			}
		}
	}
	else
	{
		alpha = 1.0f;
		isInvuln = 0;
	}

	if (stateKnockedBack)
	{
		setFrameRange(4, 4);
	}
	else if (collidingX && (isMovingLeft || isMovingRight) && !(isMovingLeft && isMovingRight))
	{
		setFrameRange(48, 48);
	}
	else if (!isOnPlatform)
	{
		setFrameRange(15, 16);
		setFrameInterval(0.1f);
	}
	else if ((isMovingLeft || isMovingRight) && !(isMovingLeft && isMovingRight))
	{
		setFrameRange(12, 14);
		setFrameInterval(0.1f);
	}
	else
	{
		setFrameRange(0, 0);
	}

	Character::update(dt);
}

void Guy::damageTaken()
{
	if (!isInvuln)
	{
		stateKnockedBack = 1;
		isInvuln = 1;
		tHurtFlash->reset();
		tHurtFlash->start();
	}
}

void Guy::jump()
{
	if (stateJumping == 0 && isOnPlatform)
	{
		stateJumping = 1;
	}
}

void Guy::bounce()
{
	if (stateJumping == 2)
	{
		velocity.y = -velocity.y / 2;
		stateJumping++;
	}
}

void Guy::keyboardSpecial(int key)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		isMovingLeft = 1;
		isFlippedX = 1;
		break;
	case GLUT_KEY_RIGHT:
		isMovingRight = 1;
		isFlippedX = 0;
		break;
	}
}

void Guy::keyboardSpecialUp(int key)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		isMovingLeft = 0;
		break;
	case GLUT_KEY_RIGHT:
		isMovingRight = 0;
		break;
	}
}

void Guy::keyboard(unsigned char key)
{
	switch (key)
	{
	case KEY_SPACE:
		
		if (jumpPressed == 0)
		{
			jump();
			jumpPressed = 1;
		}
		
		break;
	}
}

void Guy::keyboardUp(unsigned char key)
{
	switch (key)
	{
	case KEY_SPACE:
		jumpPressed = 0;
		break;
	}
}