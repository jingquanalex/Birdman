#include "Camera.h"

using namespace glm;
using namespace std;

extern float g_content_width;
extern float g_content_height;

// Input: width and height of the window as int.
Camera::Camera(int width, int height)
{
	position = vec3(0.0f, 0.0f, z);
	direction = vec3(0.0f, 0.0f, -1.0f);
	up = vec3(0.0f, 1.0f, 0.0f);
	panMaxSpeed = 1800.0f;
	panAcceleration = 5500.0f;

	setResolution(width, height);
	updateProjectionMatrix();
	updateViewMatrix();
}

// Input: width and height of the window as int.
// Calculate aspect ratio of window, call on window reshape.
void Camera::setResolution(int width, int height)
{
	if (height == 0) height = 1;
	resolution = vec2((float)width, (float)height);
	aspectRatio = resolution.x / resolution.y;
	updateProjectionMatrix();
}

// Set Camera max movement speed.
void Camera::setPanMaxSpeed(float speed)
{
	panMaxSpeed = speed;
}

// Scroll the camera to a specified position
void Camera::moveTo(vec3 position)
{
	isMovingToDest = true;
	destPosition = vec3(position.x, position.y, getPosition().z);
}

// Input: The frame delta time.
// Call in main update loop.
void Camera::update(float dt)
{
	updateViewMatrix();

	// Move camera to specified position smoothly
	if (isMovingToDest)
	{
		vec3 dir = destPosition - position;
		float distSqr = dot(dir, dir);
		vec3 dirNorm = normalize(dir);
		if (distSqr > 5.0f && dt < 0.1f)
		{
			velocity += dir * 22 * dt;
		}
		else
		{
			isMovingToDest = false;
		}
	}

	// Update velocity and cap to maxspeed
	if (isMovingUp)
	{
		velocity.y += panAcceleration * dt;
		if (velocity.y > panMaxSpeed) velocity.y = panMaxSpeed;
	}
	if (isMovingDown)
	{
		velocity.y -= panAcceleration * dt;
		if (fabs(velocity.y) > panMaxSpeed) velocity.y = -panMaxSpeed;
	}
	if (isMovingLeft)
	{
		velocity.x -= panAcceleration * dt;
		if (fabs(velocity.x) > panMaxSpeed) velocity.x = -panMaxSpeed;
	}
	if (isMovingRight)
	{
		velocity.x += panAcceleration * dt;
		if (velocity.x > panMaxSpeed) velocity.x = panMaxSpeed;
	}

	// If not moving, decelerate.
	if (!(isMovingUp || isMovingDown) || isMovingUp && isMovingDown)
	{
		velocity += -velocity * 6 * dt;
	}

	// If velocity is minimum, set to zero
	if (dot(velocity, velocity) < 1.0f)
	{
		velocity = vec3(0.0f);
	}

	//cout << glm::dot(velocity, velocity) << endl;

	position += velocity * dt;
}

// Create ortho2d matrix, maintain viewport aspect ratio.
void Camera::updateProjectionMatrix()
{
	// For now, design game scenes at 1280x800, and map clipping to window resolution.
	matProjection = glm::ortho(-resolution.x / 2, resolution.x / 2, -resolution.y / 2, resolution.y / 2, 1.0f, 1000.0f);

	//matProjection = glm::ortho(-g_content_width , g_content_width , -g_content_height, g_content_height);

	/*if (resolution.x >= resolution.y)
	{
		matProjection = glm::ortho(-g_content_width * aspectRatio, g_content_width * aspectRatio, -g_content_height, g_content_height);
	}
	else
	{
		matProjection = glm::ortho(-g_content_width, g_content_width, -g_content_height / aspectRatio, g_content_height / aspectRatio);
	}*/
}

// Create view matrix based on position.
void Camera::updateViewMatrix()
{
	matView = glm::lookAt(position, position + direction, up);
}

// Special keys callback
void Camera::keyboardSpecial(int key)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		isMovingUp = 1;
		isMovingToDest = false;
		break;
	case GLUT_KEY_DOWN:
		isMovingDown = 1;
		isMovingToDest = false;
		break;
	case GLUT_KEY_LEFT:
		isMovingLeft = 1;
		isMovingToDest = false;
		break;
	case GLUT_KEY_RIGHT:
		isMovingRight = 1;
		isMovingToDest = false;
		break;
	}
}

// Special keys up callback
void Camera::keyboardSpecialUp(int key)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		isMovingUp = 0;
		break;
	case GLUT_KEY_DOWN:
		isMovingDown = 0;
		break;
	case GLUT_KEY_LEFT:
		isMovingLeft = 0;
		break;
	case GLUT_KEY_RIGHT:
		isMovingRight = 0;
		break;
	}
}

// Accessors definitions

void Camera::setPosition(glm::vec3 position)
{
	this->position = vec3(position.x, position.y, z);
}

mat4 Camera::getMatViewProjection() const
{
	return matProjection * matView;
}

mat4 Camera::getMatView() const
{
	return matView;
}

mat4 Camera::getMatProjection() const
{
	return matProjection;
}

vec3 Camera::getPosition() const
{
	return position;
}

vec2 Camera::getResolution() const
{
	return resolution;
}