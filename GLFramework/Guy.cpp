#include "Guy.h"

using namespace std;
using namespace glm;

Guy::Guy(vec3 position) : Character("media\\img\\char.png", position, vec2(64, 64))
{
	jumpSpeed = 1200.0f;
	moveSpeed = 550.0f;
	setupAnimation(vec2(64, 64), 0.2f, 0, 2);
	startAnimation();
	setupCollision(vec2(38, 50), vec3(0, -5, 0));
	//setBoundingRectVisible(1);
}

void Guy::update(float dt)
{
	Character::update(dt);

	if (isCollidingX)
	{
		setFrame(48);
	}
	else if (!isOnPlatform)
	{
		setFrameRange(15, 16);
		setFrameInterval(0.1f);
	}
	else if (isMovingLeft || isMovingRight)
	{
		setFrameRange(12, 14);
		setFrameInterval(0.1f);
	}
	else
	{
		setFrameRange(0, 0);
	}
}

void Guy::destroy()
{
	Character::destroy();
	delete this;
}

void Guy::keyboardSpecial(int key)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		isMovingLeft = 1;
		break;
	case GLUT_KEY_RIGHT:
		isMovingRight = 1;
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
		isJumping = 1;
		break;
	}
}

void Guy::keyboardUp(unsigned char key)
{
	switch (key)
	{
	case KEY_SPACE:
		isJumping = 0;
		break;
	}
}