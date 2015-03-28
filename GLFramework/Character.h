#pragma once

#include "Sprite.h"
#include "Tilemap.h"

// Common character states.
// All characters are affected by gravity and collide with the map.
class Character : public Sprite
{

private:

	glm::vec3 oldPosition;
	Tilemap* tilemap;
	float gravity = 5.0f;

protected:

	glm::vec3 velocity;
	float moveSpeed;
	float jumpSpeed;
	bool isMovingLeft, isMovingRight, isJumping, isOnPlatform, isHurt, isInvuln, isIdle,
		isKnockedBack;
	int collidingX = 0, collidingY = 0;

public:

	Character(std::string texPath, glm::vec3 position, glm::vec2 size);

	// === Functions ===

	void update(float dt);
	void destroy();

	void setupMapCollision(Tilemap* tilemap);

	// === Accessors ===

	float getMoveSpeed() const;
	float getJumpHeight() const;
	glm::vec3 getVelocity() const;

	void setMoveSpeed(float speed);
	void setJumpHeight(float height);
	void setVelocity(glm::vec3 velocity);

};