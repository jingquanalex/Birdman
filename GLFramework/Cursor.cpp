#include "Cursor.h"

using namespace glm;
using namespace std;

Cursor::Cursor(string imgPath, vec2 size) : Sprite(imgPath, globalPosition, size, true)
{
	glutSetCursor(GLUT_CURSOR_NONE);
	Sprite::setZOrder(1.0f);
	Sprite::setPositionOffset(vec3(getSize().x / 2, -getSize().y / 2, 0));
	Sprite::setupCollision(vec2(1, 1), vec3(-size.x / 2, size.y / 2, 0));
	//Sprite::setBoundingRectVisible(1);
}

void Cursor::update(float dt, Camera* camera, float mouseX, float mouseY)
{
	// Store local coords relative to window and world coords relative to the world.
	localPosition = vec3(mouseX, mouseY, 0.0f);
	globalPosition = vec3(mouseX - camera->getResolution().x / 2 + camera->getPosition().x,
		-(mouseY - camera->getResolution().y / 2 - camera->getPosition().y), 0.0f);

	Sprite::setPosition(globalPosition);

	/*ostringstream ss;
	ss << globalPosition.x << ", " << globalPosition.y;
	glutSetWindowTitle(ss.str().c_str());*/
}

void Cursor::draw()
{
	Sprite::draw();
}

vec3 Cursor::getGlobalPosition() const
{
	return globalPosition;
}

vec3 Cursor::getLocalPosition() const
{
	return localPosition;
}