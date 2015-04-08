#include "Sprite.h"

using namespace glm;
using namespace std;

// === Static Declarations ===

vector<Sprite*> Sprite::listSprites;
float Sprite::g_zOrder = -1.0f;

void Sprite::resetZOrder()
{
	g_zOrder = -1.0f;
}

void Sprite::sortToZOrder()
{
	sort(listSprites.begin(), listSprites.end(), [](Sprite* sprite1, Sprite* sprite2) {
		return sprite1->getZOrder() < sprite2->getZOrder();
	});
}

void Sprite::drawSprites()
{
	for_each(listSprites.begin(), listSprites.end(), [](Sprite*& sprite) {
		if (sprite->getVisibie()) sprite->draw();
	});
}

Sprite::Sprite()
{
	this->position = vec3();
	sortToZOrder();
}

// Make a copy of a sprite at a specified location.
Sprite::Sprite(Sprite* sprite, vec3 position, bool excludeDrawList)
{
	this->texid = sprite->getTexID();
	this->position = position;
	this->texSize = sprite->getTexSize();
	this->size = sprite->getSize();

	setupCollision();
	load(excludeDrawList);
}

// Initialize a sprite with texture path, position and size, wrap bounding rectangle around its size and load the texture.
// Size is the dimension of the sprite, if not provided, the dimensions of the texture will be used.
Sprite::Sprite(string texPath, vec3 position, vec2 size, bool excludeDrawList)
{
	this->texPath = texPath;
	this->position = position;
	this->size = size;

	setupCollision();
	load(excludeDrawList);
}

Sprite::~Sprite()
{
	listSprites.erase(remove(listSprites.begin(), listSprites.end(), this), listSprites.end());
	delete quadVert;
	delete quadCoord;
	if (animTimer) animTimer->destroy();
}

bool Sprite::load(bool excludeDrawList)
{
	// Add sprite to draw list
	if (!excludeDrawList)
	{
		listSprites.push_back(this);
	}

	// If no zOrder is provided, assign it automatically.
	if (zOrder == 0.0f)
	{
		zOrder = g_zOrder;
		g_zOrder -= 0.1f;
	}

	sortToZOrder();

	// Define quad vertices and texcoords.
	quadVert = new float[12] {
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	quadCoord = new float[8] {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	quadCount = 4;

	// If no texture is assigned to object, load one. It will be assigned when cloning a sprite.
	if (texid == 0)
	{
		// Get texture size
		unsigned char* image = NULL;
		int texWidth, texHeight;
		image = SOIL_load_image(texPath.c_str(), &texWidth, &texHeight, 0, 0);
		if (image == NULL)
		{
			cout << "SOIL error: " << SOIL_last_result() << " \"" << texPath << "\"" << endl;
			return false;
		}

		SOIL_free_image_data(image);
		texSize = vec2(texWidth, texHeight);

		// Set sprite size to texture's size, if size not specified.
		if (size == vec2())
		{
			size = texSize;
		}

		// Load texture
		texid = SOIL_load_OGL_texture(texPath.c_str(), 0, 0, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
		if (texid == 0)
		{
			cout << "SOIL error: " << SOIL_last_result() << " \"" << texPath << "\"" << endl;
			return false;
		}

		// Set default texture parameters for sprites
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	return true;
}

void Sprite::update(float dt)
{
	// Set this sprite's position to its attached object's position.
	if (attachedSprite != NULL)
	{
		position = attachedSprite->getPosition();
	}

	if (attachedCamera != NULL)
	{
		position = attachedCamera->getPosition();
	}

	// Animations
	if (hasAnimation)
	{
		if (isAnimating)
		{
			if (animTimer->hasTicked())
			{
				currentFrame = currentFrame == endFrame ? startFrame : ++currentFrame;
				//printf("frame: %i\n", currentFrame);
			}
		}

		// Set texcoord based on frame number. Note: currentFrame starts at 0.
		quadCoord = new float[8] {
			animFramesDelta.x * (currentFrame % (int)animFramesCount.x), 1 - animFramesDelta.y * (glm::floor(currentFrame / animFramesCount.x) + 1),
			animFramesDelta.x * (currentFrame % (int)animFramesCount.x), 1 - animFramesDelta.y * glm::floor(currentFrame / animFramesCount.x),
			animFramesDelta.x * (currentFrame % (int)animFramesCount.x + 1), 1 - animFramesDelta.y * glm::floor(currentFrame / animFramesCount.x),
			animFramesDelta.x * (currentFrame % (int)animFramesCount.x + 1), 1 - animFramesDelta.y * (glm::floor(currentFrame / animFramesCount.x) + 1),
		};
	}
}

void Sprite::draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();
	glTranslatef(positionOffset.x, positionOffset.y, 0.0f);
	glTranslatef(position.x, position.y, zOrder);
	if (isFlippedX) glScalef(-1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texid);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(color.r, color.g, color.b, alpha);

	glPushMatrix();
	glScalef(size.x, size.y, 1.0f);
	
	glVertexPointer(3, GL_FLOAT, 0, quadVert);
	glTexCoordPointer(2, GL_FLOAT, 0, quadCoord);
	glDrawArrays(GL_QUADS, 0, quadCount);

	/*glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.0f);
	glEnd();*/

	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	// Draw bounding rectangle, wrapped around the sprite, unless a boundingRectPositionOffset is specified.
	if (isBoundingRectVisible)
	{
		glColor3f(colorBorder.r, colorBorder.g, colorBorder.b);
		glLineWidth(3.0f);

		glPushMatrix();
		glTranslatef(boundingRectPositionOffset.x, boundingRectPositionOffset.y, zOrder);
		glScalef(boundingRectSize.x, boundingRectSize.y, 1.0f);

		glDrawArrays(GL_LINE_LOOP, 0, 4);

		/*glBegin(GL_LINE_LOOP);
		glVertex3f(-0.5f, -0.5f, 0.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);
		glVertex3f(0.5f, 0.5f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);
		glEnd();*/
		
		glPopMatrix();
	}

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// Setup spritesheet animation. Framesize takes in a vec 2 that defines the area of the spritesheet to
// be used as a frame. frameInterval defines how long to wait before changing to the next frame.
// start and end Frame defines the range of frame indices to play before looping back.
void Sprite::setupAnimation(vec2 frameSize, float frameInterval, int startFrame, int endFrame)
{
	hasAnimation = true;
	animTimer = new Timer(frameInterval);
	animFrameInterval = frameInterval;
	this->startFrame = startFrame;
	this->endFrame = endFrame;
	this->currentFrame = startFrame;

	// Check how many frames are in the sprite sheet.
	animFramesCount.x = glm::floor(texSize.x / frameSize.x);
	animFramesCount.y = glm::floor(texSize.y / frameSize.y);

	// If frames is unspecified, use all possible frames in the spritesheet.
	if (endFrame == 0)
	{
		this->endFrame = (int)(animFramesCount.x * animFramesCount.y) - 1;
	}

	animFramesDelta.x = 1 / animFramesCount.x;
	animFramesDelta.y = 1 / animFramesCount.y;

	quadCoord = new float[8] {
		0.0f, 1 - animFramesDelta.y,
		0.0f, 1.0f,
		animFramesDelta.x, 1.0f,
		animFramesDelta.x, 1 - animFramesDelta.y
	};
}

// Set the range of frame indices to loop.
void Sprite::setFrameRange(int startFrame, int endFrame)
{
	if (this->startFrame != startFrame || this->endFrame != endFrame)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->currentFrame = startFrame;
	}
}

// Create a bounding rectangle with defined size and its optional offset location from the object's position.
// Bounding rect stores lower left and upper right coord in a vec4.
void Sprite::setupCollision(vec2 boundingRectSize, vec3 boundingRectPositionOffset)
{
	this->isCollidable = true;
	this->boundingRectSize = boundingRectSize;
	this->boundingRectPositionOffset = boundingRectPositionOffset;

	if (boundingRectSize == vec2())
	{
		this->boundingRectSize = this->size;
	}

	boundingRect = vec4(-boundingRectSize.x / 2 + positionOffset.x + boundingRectPositionOffset.x,
		-boundingRectSize.y / 2 + positionOffset.y + boundingRectPositionOffset.y,
		boundingRectSize.x / 2 + positionOffset.x + boundingRectPositionOffset.x,
		boundingRectSize.y / 2 + positionOffset.y + boundingRectPositionOffset.y);
}

// Check if sprite is colliding with another sprite, based on their bounding rectangles.
bool Sprite::isCollidingWith(Sprite* sprite)
{
	bool isColliding = false;

	// First check if objects has collision enabled.
	if (isCollidable && sprite->getIsCollidable())
	{
		vec3 obj1Pos = getPosition() + getPositionOffset() + getBoundingRectPositionOffset();
		vec3 obj2Pos = sprite->getPosition() + sprite->getPositionOffset() + sprite->getBoundingRectPositionOffset();
		isColliding = glm::abs(obj1Pos.x - obj2Pos.x) * 2 < getBoundingRectSize().x + sprite->getBoundingRectSize().x &&
			glm::abs(obj1Pos.y - obj2Pos.y) * 2 < getBoundingRectSize().y + sprite->getBoundingRectSize().y;
	}

	return isColliding;
}

bool Sprite::isInRangeOf(Sprite* sprite, float distance) const
{
	return glm::distance2(position, sprite->getPosition()) <= distance * distance;
}

string Sprite::getName() const
{
	return name;
}

int Sprite::getTexID() const
{
	return texid;
}

float Sprite::getZOrder() const
{
	return zOrder;
}

vec3 Sprite::getPosition() const
{
	return position;
}

vec3 Sprite::getPositionOffset() const
{
	return positionOffset;
}

vec2 Sprite::getSize() const
{
	return size;
}

vec2 Sprite::getTexSize() const
{
	return texSize;
}

// Check if the object is collidable.
bool Sprite::getIsCollidable() const
{
	return isCollidable;
}

vec2 Sprite::getBoundingRectSize() const
{
	return boundingRectSize;
}

vec3 Sprite::getBoundingRectPositionOffset() const
{
	return boundingRectPositionOffset;
}

vec4 Sprite::getBoundingRect() const
{
	return boundingRect;
}

void Sprite::setTexID(GLuint texid)
{
	texid = texid;
}

void Sprite::setPosition(vec3 position)
{
	this->position = position;
}

void Sprite::setPositionOffset(vec3 offset)
{
	this->positionOffset = offset;
}

void Sprite::setSize(vec2 size)
{
	this->size = size;
}

// Turn on/off collision for this object
void Sprite::setIsCollidable(bool isCollidable)
{
	this->isCollidable = isCollidable;
}

// Give this sprite a unique name, great to compare special object for collision.
void Sprite::setName(string name)
{
	this->name = name;
}

// Draw the bounding rect too?
void Sprite::setBoundingRectVisible(bool isVisible)
{
	isBoundingRectVisible = isVisible;
}

// Note: camera near-far is set to 1f - 100f! z-axis goes into the screen.
void Sprite::setZOrder(float zOrder)
{
	this->zOrder = zOrder;
}

void Sprite::setBoundingRectSize(glm::vec2 size)
{
	boundingRectSize = size;
}

void Sprite::setBoundingRectPositionOffset(glm::vec3 offset)
{
	boundingRectPositionOffset = offset;
}

void Sprite::setColor(vec3 color)
{
	color = color;
}

void Sprite::setColorBorder(vec3 color)
{
	colorBorder = color;
}

bool Sprite::getHasAnimation() const
{
	return hasAnimation;
}

void Sprite::setFrame(int frame)
{
	currentFrame = frame;
	if (currentFrame < startFrame || currentFrame > endFrame)
	{
		setFrameRange(currentFrame, currentFrame);
	}
}

void Sprite::setFrameInterval(float interval)
{
	animTimer->setTickInterval(interval);
}

void Sprite::startAnimation()
{
	animTimer->start();
	isAnimating = true;
}

void Sprite::stopAnimation()
{
	animTimer->stop();
	isAnimating = false;
}

bool Sprite::getVisibie() const
{
	return isVisible;
}

bool Sprite::getIsFlippedX() const
{
	return isFlippedX;
}

void Sprite::setVisible(bool isVisible)
{
	this->isVisible = isVisible;
}

void Sprite::attachTo(Sprite* sprite)
{
	attachedSprite = sprite;
}

void Sprite::attachTo(Camera* camera)
{
	attachedCamera = camera;
}

void Sprite::flipX(bool isFlipX)
{
	isFlippedX = isFlipX;
}