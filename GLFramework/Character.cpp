#include "Character.h"

using namespace std;
using namespace glm;

Character::Character(string texPath, vec3 position, vec2 size) : Sprite(texPath, position, size)
{
	isMovingLeft = false;
	isMovingRight = false;
	isJumping = false;
	isHurt = false;
	isInvuln = false;
	isIdle = false;
	isOnPlatform = false;
}

void Character::update(float dt)
{
	Sprite::update(dt);

	isCollidingX = 0;

	// Movement left and right
	if (isMovingLeft)
	{
		isFlippedX = 1;
		velocity.x = -moveSpeed;

		// Collide at bounding box's edges.
		// Stop moving if tile at position (plus offset) is not empty.
		int valueT = tilemap->getValueAtPos(position + vec3(-boundingRectSize.x / 2 + boundingRectPositionOffset.x, -boundingRectSize.y / 2 + boundingRectPositionOffset.y + 5, 0));
		int valueB = tilemap->getValueAtPos(position + vec3(-boundingRectSize.x / 2 + boundingRectPositionOffset.x, boundingRectSize.y / 2 + boundingRectPositionOffset.y + 5, 0));
		if (valueT != -1 || valueB != -1)
		{
			velocity.x = 0.0f;
			isCollidingX = 1;
		}
	}

	if (isMovingRight)
	{
		isFlippedX = 0;
		velocity.x = moveSpeed;

		// Stop moving if tile at position (plus offset) is not empty.
		int valueT = tilemap->getValueAtPos(position + vec3(boundingRectSize.x / 2 + boundingRectPositionOffset.x, -boundingRectSize.y / 2 + boundingRectPositionOffset.y + 5, 0));
		int valueB = tilemap->getValueAtPos(position + vec3(boundingRectSize.x / 2 + boundingRectPositionOffset.x, boundingRectSize.y / 2 + boundingRectPositionOffset.y + 5, 0));
		if (valueT != -1 || valueB != -1)
		{
			velocity.x = 0.0f;
			isCollidingX = 1;
		}
	}

	// Stop if not moving.
	if (!(isMovingLeft || isMovingRight) || isMovingLeft && isMovingRight)
	{
		velocity.x = 0.0f;
	}

	// Always fall if not colliding.
	int valueL = tilemap->getValueAtPos(position + vec3(-boundingRectSize.x / 2 + boundingRectPositionOffset.x + 5, -boundingRectSize.y / 2 + boundingRectPositionOffset.y, 0));
	int valueR = tilemap->getValueAtPos(position + vec3(boundingRectSize.x / 2 + boundingRectPositionOffset.x - 5, -boundingRectSize.y / 2 + boundingRectPositionOffset.y, 0));
	if (valueL == -1 && valueR == -1)
	{
		velocity.y += -gravity;
		isOnPlatform = 0;
	}
	else
	{
		//position = oldPosition;
		velocity.y = 0;
		isOnPlatform = 1;
	}
	
	// Celling collision
	valueL = tilemap->getValueAtPos(position + vec3(-boundingRectSize.x / 2 + boundingRectPositionOffset.x + 5, boundingRectSize.y / 2 + boundingRectPositionOffset.y, 0));
	valueR = tilemap->getValueAtPos(position + vec3(boundingRectSize.x / 2 + boundingRectPositionOffset.x - 5, boundingRectSize.y / 2 + boundingRectPositionOffset.y, 0));
	if (valueL != -1 || valueR != -1)
	{
		position = oldPosition;
		velocity.y = -velocity.y / 2;
	}

	// Jumping
	if (isJumping && isOnPlatform)
	{
		velocity.y = jumpSpeed;
	}

	oldPosition = position;
	position += velocity * dt;
}

void Character::destroy()
{
	Sprite::destroy();
	delete this;
}

void Character::setupMapCollision(Tilemap* tilemap)
{
	this->tilemap = tilemap;
}

/*bool Character::getIsPhysicsAffected() const
{
	return isPhysicsAffected;
}

void Character::setIsPhysicsAffected(bool isPhysicsAffected)
{
	this->isPhysicsAffected = isPhysicsAffected;
}*/

float Character::getMoveSpeed() const
{
	return moveSpeed;
}

float Character::getJumpHeight() const
{
	return jumpSpeed;
}

vec3 Character::getVelocity() const
{
	return velocity;
}

void Character::setMoveSpeed(float speed)
{
	moveSpeed = speed;
}

void Character::setJumpHeight(float height)
{
	jumpSpeed = height;
}

void Character::setVelocity(vec3 velocity)
{
	this->velocity = velocity;
}
