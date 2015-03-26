#pragma once
#include "globalIncludes.h"
#include "Sprite.h"
#include "Camera.h"
#include <sstream>
#include <string>

// Draws and manages the coordinates of the mouse cursor.
class Cursor : public Sprite
{

private:

	glm::vec3 globalPosition, localPosition;

public:

	Cursor(std::string imgPath = "media\\img\\cursor.png", glm::vec2 size = glm::vec2(32.0f, 32.0f));

	// === Functions ===

	void update(float dt, Camera* camera, float mouseX, float mouseY);
	void draw();
	void destroy();

	glm::vec3 getGlobalPosition() const;
	glm::vec3 getLocalPosition() const;

};