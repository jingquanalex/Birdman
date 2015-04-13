#include "Character.h"

using namespace std;
using namespace glm;

Character::Character(Character* character, vec3 position) : Sprite(character, position)
{

}

Character::Character(string texPath, vec3 position, vec2 size) : Sprite(texPath, position, size)
{
	isMovingLeft = false;
	isMovingRight = false;
	isInvuln = false;
	isIdle = false;
	isOnPlatform = false;
	isDead = false;
}

Character::~Character()
{

}

void Character::update(float dt)
{
	if (tilemap)
	{
		vec3 nextPosition = position + velocity * dt;

		// Check for collision at the topleft and bottomleft points of the sprite bounding box.
		vec2 coordT = tilemap->getCoordAtPos(vec3(nextPosition.x, position.y, 0) + vec3(-boundingRectSize.x / 2, boundingRectSize.y / 2 - 5, 0) + boundingRectPositionOffset);
		vec2 coordB = tilemap->getCoordAtPos(vec3(nextPosition.x, position.y, 0) + vec3(-boundingRectSize.x / 2, -boundingRectSize.y / 2 + 5, 0) + boundingRectPositionOffset);
		int valueT = tilemap->getValueAtCoord(coordT);
		int valueB = tilemap->getValueAtCoord(coordB);
		if (valueT > 0 || valueB > 0)
		{
			// If colliding on the left side, set 0 velocity and position right next to the wall.
			collidingX = -1;
			if (velocity.x < 0.0f) velocity.x = 0.0f;
			position.x = tilemap->getPositionAtCoord(coordT).x + tilemap->getMapTileSize().x + boundingRectSize.x / 2 + boundingRectPositionOffset.x - 1;
		}
		else if (isMovingLeft && !stateKnockedBack)
		{
			collidingX = 0; // Is the character colliding horizontally. -1 : left, 1 : right
			velocity.x = -moveSpeed;
		}

		// Check for collision at the topright and bottomright points of the sprite bounding box.
		coordT = tilemap->getCoordAtPos(vec3(nextPosition.x, position.y, 0) + vec3(boundingRectSize.x / 2, boundingRectSize.y / 2 - 5, 0) + boundingRectPositionOffset);
		coordB = tilemap->getCoordAtPos(vec3(nextPosition.x, position.y, 0) + vec3(boundingRectSize.x / 2, -boundingRectSize.y / 2 + 5, 0) + boundingRectPositionOffset);
		valueT = tilemap->getValueAtCoord(coordT);
		valueB = tilemap->getValueAtCoord(coordB);
		if (valueT > 0 || valueB > 0)
		{
			collidingX = 1;
			if (velocity.x > 0.0f) velocity.x = 0.0f;
			position.x = tilemap->getPositionAtCoord(coordT).x - boundingRectSize.x / 2 + boundingRectPositionOffset.x;
		}
		else if (isMovingRight && !stateKnockedBack)
		{
			collidingX = 0;
			velocity.x = moveSpeed;
		}

		// Stop if not moving left or right.
		if (!stateKnockedBack && (!(isMovingLeft || isMovingRight) || isMovingLeft && isMovingRight))
		{
			velocity.x = 0.0f;
		}

		// Celling collision
		// Check for collision at the topleft and topright points of the sprite bounding box.
		vec2 coordL = tilemap->getCoordAtPos(vec3(position.x, nextPosition.y, 0) + vec3(-boundingRectSize.x / 2 + 5, boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
		vec2 coordR = tilemap->getCoordAtPos(vec3(position.x, nextPosition.y, 0) + vec3(boundingRectSize.x / 2 - 5, boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
		int valueL = tilemap->getValueAtCoord(coordL);
		int valueR = tilemap->getValueAtCoord(coordR);
		if (valueL > 0 || valueR > 0)
		{
			velocity.y = -velocity.y;
			position.y = tilemap->getPositionAtCoord(coordL).y - tilemap->getMapTileSize().y - boundingRectSize.y / 2 - boundingRectPositionOffset.y;
		}

		// Always fall if not colliding.
		// Check for collision at the bottomleft and bottomright points of the sprite bounding box.
		coordL = tilemap->getCoordAtPos(vec3(position.x, nextPosition.y, 0) + vec3(-boundingRectSize.x / 2 + 5, -boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
		coordR = tilemap->getCoordAtPos(vec3(position.x, nextPosition.y, 0) + vec3(boundingRectSize.x / 2 - 5, -boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
		valueL = tilemap->getValueAtCoord(coordL);
		valueR = tilemap->getValueAtCoord(coordR);
		if ((valueL > 0 || valueR > 0) && velocity.y <= 0.0f)
		{
			isOnPlatform = 1;
			velocity.y = 0;
			position.y = tilemap->getPositionAtCoord(coordL).y + boundingRectSize.y / 2 - boundingRectPositionOffset.y;
		}
		else
		{
			isOnPlatform = 0;
			if (hasGravity)
			{
				velocity.y += -gravity * dt;
			}
		}

		//printf("%f, %f \n", position.x, position.y);
	}
	else
	{
		if (hasGravity)
		{
			velocity.y += -gravity * dt;
		}
	}

	// Jumping fall back on platform
	if (stateJumping >= 2 && isOnPlatform)
	{
		stateJumping = 0;
	}

	// Jumping start
	if (stateJumping == 1)
	{
		stateJumping = 2;
		velocity.y = jumpSpeed;
	}

	// Knocked back states 1 - knock left, 2 - knock right
	if (stateKnockedBack == 3 && isOnPlatform)
	{
		stateKnockedBack = 0;
	}

	if (stateKnockedBack == 1)
	{
		velocity = vec3(-400, 400, 0);
		stateKnockedBack = 3;
	}
	else if (stateKnockedBack == 2)
	{
		velocity = vec3(400, 400, 0);
		stateKnockedBack = 3;
	}

	oldPosition = position;
	position += velocity * dt;

	Sprite::update(dt);
}

void Character::setupMapCollision(Tilemap* tilemap)
{
	this->tilemap = tilemap;
}

// 1 - left, 2 - right
void Character::knockback(int direction)
{
	if (stateKnockedBack == 0)
	{
		stateKnockedBack = direction;
	}
}

void Character::jump()
{
	if (stateJumping == 0 && isOnPlatform)
	{
		stateJumping = 1;
	}
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

bool Character::getIsOnPlatform() const
{
	return isOnPlatform;
}

bool Character::getIsDead() const
{
	return isDead;
}

int Character::getCollidingX() const
{
	return collidingX;
}

bool Character::getIsInvuln() const
{
	return isInvuln;
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

void Character::setHasGravity(bool hasGravity)
{
	this->hasGravity = hasGravity;
}

void Character::setIdle(bool idle)
{
	this->isIdle = idle;
}