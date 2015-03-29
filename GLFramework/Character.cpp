#include "Character.h"

using namespace std;
using namespace glm;

Character::Character(string texPath, vec3 position, vec2 size) : Sprite(texPath, position, size)
{
	isMovingLeft = false;
	isMovingRight = false;
	isJumping = false;
	isInvuln = false;
	isIdle = false;
	isOnPlatform = false;
}

void Character::update(float dt)
{
	// Movement left and right
	
	// Check for collision at the topleft and bottomleft points of the sprite bounding box.
	vec2 coordT = tilemap->getCoordAtPos(position + vec3(-boundingRectSize.x / 2, boundingRectSize.y / 2 - 5, 0) + boundingRectPositionOffset);
	vec2 coordB = tilemap->getCoordAtPos(position + vec3(-boundingRectSize.x / 2, -boundingRectSize.y / 2 + 5, 0) + boundingRectPositionOffset);
	int valueT = tilemap->getValueAtCoord(coordT);
	int valueB = tilemap->getValueAtCoord(coordB);
	if (valueT != -1 || valueB != -1)
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
	coordT = tilemap->getCoordAtPos(position + vec3(boundingRectSize.x / 2, boundingRectSize.y / 2 - 5, 0) + boundingRectPositionOffset);
	coordB = tilemap->getCoordAtPos(position + vec3(boundingRectSize.x / 2, -boundingRectSize.y / 2 + 5, 0) + boundingRectPositionOffset);
	valueT = tilemap->getValueAtCoord(coordT);
	valueB = tilemap->getValueAtCoord(coordB);
	if (valueT != -1 || valueB != -1)
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

	// Always fall if not colliding.
	// Check for collision at the bottomleft and bottomright points of the sprite bounding box.
	vec2 coordL = tilemap->getCoordAtPos(position + vec3(-boundingRectSize.x / 2 + 5, -boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
	vec2 coordR = tilemap->getCoordAtPos(position + vec3(boundingRectSize.x / 2 - 5, -boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
	int valueL = tilemap->getValueAtCoord(coordL);
	int valueR = tilemap->getValueAtCoord(coordR);
	if (valueL != -1 || valueR != -1)
	{
		isOnPlatform = 1;
		velocity.y = 0;
		position.y = tilemap->getPositionAtCoord(coordL).y + boundingRectSize.y / 2 - boundingRectPositionOffset.y;
	}
	else
	{
		isOnPlatform = 0;
		velocity.y += -gravity;
	}

	printf("%f, %f \n", position.x, position.y);
	
	// Celling collision
	// Check for collision at the topleft and topright points of the sprite bounding box.
	coordL = tilemap->getCoordAtPos(position + vec3(-boundingRectSize.x / 2 + 5, boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
	coordR = tilemap->getCoordAtPos(position + vec3(boundingRectSize.x / 2 - 5, boundingRectSize.y / 2, 0) + boundingRectPositionOffset);
	valueL = tilemap->getValueAtCoord(coordL);
	valueR = tilemap->getValueAtCoord(coordR);
	if (valueL != -1 || valueR != -1)
	{
		velocity.y = -velocity.y / 2;
		position.y = tilemap->getPositionAtCoord(coordL).y - tilemap->getMapTileSize().y - boundingRectSize.y / 2 + boundingRectPositionOffset.y;
	}

	// Jumping
	if (isJumping && isOnPlatform)
	{
		velocity.y = jumpSpeed;
	}

	// Knocked back states

	if (stateKnockedBack == 2 && isOnPlatform)
	{
		stateKnockedBack = 0;
	}

	if (stateKnockedBack == 1)
	{
		if (isFlippedX)
		{
			velocity = vec3(400, 400, 0);
		}
		else
		{
			velocity = vec3(-400, 400, 0);
		}
		stateKnockedBack = 2;
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
