#include "Projectile.h"

using namespace std;
using namespace glm;

vector<Projectile*> Projectile::listProjectiles;

void Projectile::updateProjectiles(float dt)
{
	for_each(listProjectiles.begin(), listProjectiles.end(), [dt](Projectile*& projectile) {
		if (projectile->getIsDead())
		{
			delete projectile;
			projectile = NULL;
		}
		else
		{
			projectile->update(dt);
		}
	});
	listProjectiles.erase(remove(listProjectiles.begin(), listProjectiles.end(), static_cast<Projectile*>(NULL)), listProjectiles.end());
}

const vector<Projectile*>& Projectile::getListProjectiles()
{
	return listProjectiles;
}

Projectile::Projectile(Projectile* projectile, Tilemap* tilemap, glm::vec3 position, bool flipped) : Character(projectile, position)
{
	durationTimer = new Timer(0.0f, 0.2f);
	durationTimer->start();
	listProjectiles.push_back(this);
	setupAnimation(vec2(64, 64), 0.0f, 51, 51);
	setupMapCollision(tilemap);
	setHasGravity(0);
	moveSpeed = 1550.0f;

	if (flipped)
	{
		this->position.x += -32;
		isFlippedX = 1;
		isMovingLeft = 1;
	}
	else
	{
		this->position.x += 32;
		isMovingRight = 1;
	}
}

Projectile::Projectile() : Character("media\\img\\char.png", vec3(), vec2(64, 64))
{
	setupAnimation(vec2(64, 64), 0.0f, 51, 51);
	setMoveSpeed(1600.0f);
	setSize(vec2(64, 64));
}

Projectile::~Projectile()
{
	durationTimer->destroy();
}

void Projectile::update(float dt)
{
	if (collidingX || !durationTimer->getIsRunning())
	{
		destroy();
	}

	Character::update(dt);
}

void Projectile::destroy()
{
	isDead = 1;
}