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
	isKnockedBack = false;
}

void Character::update(float dt)
{
	// Movement left and right
	if (isMovingLeft)
	{
		collidingX = 0; // Is the character colliding horizontally. -1 : left, 1 : right
		isFlippedX = 1; // Is the sprite flipped horizontally.
		velocity.x = -moveSpeed;
		
		// Check for collision at the topleft and bottomleft points of the sprite bounding box.
		int valueT = tilemap->getValueAtPos(position + vec3(-boundingRectSize.x / 2, boundingRectSize.y / 2 - 5, 0) + boundingRectPositionOffset);
		int valueB = tilemap->getValueAtPos(position + vec3(-boundingRectSize.x / 2, -boundingRectSize.y / 2 + 5, 0) + boundingRectPositionOffset);
		if (valueT != -1 || valueB != -1)
		{
			//position.x = tilemap->getPositionAtCoord(;
			collidingX = -1;
			velocity.x = 0.0f;
		}
	}

	if (isMovingRight)
	{
		collidingX = 0;
		isFlippedX = 0;
		velocity.x = moveSpeed;

		// Stop moving if tile at position (plus offset) is not empty.
		int valueT = tilemap->getValueAtPos(position + vec3(boundingRectSize.x / 2, boundingRectSize.y / 2 - 5, 0) + boundingRectPositionOffset);
		int valueB = tilemap->getValueAtPos(position + vec3(boundingRectSize.x / 2, -boundingRectSize.y / 2 + 5, 0) + boundingRectPositionOffset);
		if (valueT != -1 || valueB != -1)
		{
			//position = oldPosition;
			collidingX = 1;
			velocity.x = 0.0f;
		}
	}

	// Stop if not moving left or right.
	if (!(isMovingLeft || isMovingRight) || isMovingLeft && isMovingRight)
	{
		velocity.x = 0.0f;
	}

	// Always fall if not colliding.
	int valueL = tilemap->getValueAtPos(position + vec3(-boundingRectSize.x / 2 + 5, -boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
	int valueR = tilemap->getValueAtPos(position + vec3(boundingRectSize.x / 2 - 5, -boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
	if (valueL == -1 && valueR == -1)
	{
		velocity.y += -gravity;
		isOnPlatform = 0;
	}
	else
	{
		velocity.y = 0;
		isOnPlatform = 1;
	}
	
	// Celling collision
	valueL = tilemap->getValueAtPos(position + vec3(-boundingRectSize.x / 2 + 5, boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
	valueR = tilemap->getValueAtPos(position + vec3(boundingRectSize.x / 2 - 5, boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
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

	if (isKnockedBack)
	{
		if (isFlippedX)
		{
			velocity = vec3(800, 900, 0);
		}
		else
		{
			velocity = vec3(-800, 900, 0);
		}
		isKnockedBack = 0;
	}

	oldPosition = position;
	position += velocity * dt;

	Sprite::update(dt);
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
