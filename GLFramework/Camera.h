#pragma once

#include "globalIncludes.h"
#include <iostream>

// Camera class that builds the projection and view matrices.
class Camera
{

private:

	const float z = 5.0f;

	glm::mat4 matProjection, matView;
	glm::vec3 position, direction, up, velocity;
	float aspectRatio, panMaxSpeed, panAcceleration;
	glm::vec2 resolution;
	bool isMovingUp = 0, isMovingDown = 0, isMovingLeft = 0, isMovingRight = 0;
	glm::vec3 destPosition;
	bool isMovingToDest = false;

	// === Private Functions ===

	void updateProjectionMatrix();
	void updateViewMatrix();

public:

	Camera(int width, int height);

	// === Functions ===

	void update(float dt);
	void setResolution(int width, int height);
	void setPanMaxSpeed(float speed);
	void moveTo(glm::vec3 position);

	void keyboardSpecial(int key);
	void keyboardSpecialUp(int key);

	// === Accessors ===

	void setPosition(glm::vec3 position);

	glm::mat4 getMatViewProjection() const;
	glm::mat4 getMatView() const;
	glm::mat4 getMatProjection() const;
	glm::vec3 getPosition() const;
	glm::vec2 getResolution() const;
	
};