#pragma once

#include "globalIncludes.h"
#include "Timer.h"
#include "Camera.h"
#include <SOIL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

// Every object in the game is an instance of the sprite class.
// Handles rendering, position tracking, and collision.
class Sprite
{

private:

	// Create a draw list so that any sprite instance is added to it and later drawn,
	// from back to front based on zOrder.
	static std::vector<Sprite*> listSprites;

	// Assign a decrementing zOrder for each sprite if one is not provided.
	static float g_zOrder;

public:

	static void sortToZOrder();
	static void drawSprites();

protected:

	std::string name = "";

	float* quadVert;
	float* quadCoord;
	int quadCount;

	glm::mat4 matTransform;
	glm::vec3 position;
	glm::vec3 positionOffset;
	glm::vec2 texSize;
	glm::vec2 size;

	Sprite* attachedSprite = NULL;
	Camera* attachedCamera = NULL;

	int texid = 0;
	std::string texPath = "";
	float zOrder = 0.0f;
	float alpha = 1.0f;
	float alphaFadeTime = 0.1f;
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 colorBorder = glm::vec3(0.0f, 1.0f, 0.0f);
	bool isVisible = true;
	bool isFlippedX = false;

	bool isCollidable = false;
	bool isBoundingRectVisible = false;
	glm::vec3 boundingRectPositionOffset = glm::vec3(0.0f);
	glm::vec2 boundingRectSize = glm::vec2(0.0f);
	glm::vec4 boundingRect = glm::vec4(0.0f);

	Timer* animTimer = NULL;
	bool hasAnimation = false;
	bool isAnimating = false;
	int startFrame, endFrame;
	int currentFrame;
	float animFrameInterval;
	glm::vec2 animFramesCount;
	glm::vec2 animFramesDelta;


public:

	Sprite();
	Sprite(Sprite* sprite, glm::vec3 position);
	Sprite(std::string texPath, glm::vec3 position, glm::vec2 size = glm::vec2());
	~Sprite();

	// === Functions ===

	bool load();
	virtual void update(float dt);
	virtual void draw();

	void setupAnimation(glm::vec2 frameSize, float frameInterval, int startFrame = 0, int endFrame = 0);
	void setFrameInterval(float interval);
	void setFrameRange(int startFrame, int endFrame);
	void setFrame(int frame);
	void startAnimation();
	void stopAnimation();
	void setupCollision(glm::vec2 boundingRectSize = glm::vec2(), glm::vec3 boundingRectPositionOffset = glm::vec3());
	bool isCollidingWith(Sprite* sprite);

	// === Accessors ===

	std::string getName() const;
	int getTexID() const;
	float getZOrder() const;
	glm::vec3 getPosition() const;
	glm::vec3 getPositionOffset() const;
	glm::vec2 getSize() const;
	glm::vec2 getTexSize() const;
	bool getIsCollidable() const;
	glm::vec3 getBoundingRectPositionOffset() const;
	glm::vec2 getBoundingRectSize() const;
	glm::vec4 getBoundingRect() const;
	bool getHasAnimation() const;
	bool getVisibie() const;
	
	void setName(std::string name);
	void setTexID(GLuint texid);
	void setZOrder(float zOrder);
	void setPosition(glm::vec3 position);
	void setPositionOffset(glm::vec3 offset);
	void setSize(glm::vec2 size);
	void setIsCollidable(bool isCollidable);
	void setBoundingRectSize(glm::vec2 size);
	void setBoundingRectPositionOffset(glm::vec3 offset);
	void setBoundingRectVisible(bool isVisible);
	void setColor(glm::vec3 color);
	void setColorBorder(glm::vec3 color);
	void setVisible(bool isVisible);
	void attachTo(Sprite* sprite);
	void attachTo(Camera* camera);
	void flipX(bool isFlipX);
	
};