#pragma once

#include "Sprite.h"
#include "Tilemap.h"

// Common character states.
// All characters are affected by gravity and collide with the map.
class Character : public Sprite
{

private:

	
	
protected:

	Tilemap* tilemap = NULL;
	float gravity = 2995.0f;

	glm::vec3 oldPosition;
	glm::vec3 velocity;
	float moveSpeed;
	float jumpSpeed;
	bool isMovingLeft, isMovingRight, isOnPlatform, isInvuln, isIdle, isDead;
	int collidingX = 0, collidingY = 0;
	int stateKnockedBack = 0, stateJumping = 0;
	bool hasGravity = 1;

public:

	Character(Character* character, glm::vec3 position);
	Character(std::string texPath, glm::vec3 position, glm::vec2 size);
	~Character();

	// === Functions ===

	void update(float dt);

	void setupMapCollision(Tilemap* tilemap);
	void knockback(int direction);
	void jump();

	// === Accessors ===

	float getMoveSpeed() const;
	float getJumpHeight() const;
	glm::vec3 getVelocity() const;
	bool getIsOnPlatform() const;
	bool getIsDead() const;
	int getCollidingX() const;
	bool getIsInvuln() const;
	
	void setMoveSpeed(float speed);
	void setJumpHeight(float height);
	void setVelocity(glm::vec3 velocity);
	void setHasGravity(bool hasGravity);
	virtual void setIdle(bool idle);

};