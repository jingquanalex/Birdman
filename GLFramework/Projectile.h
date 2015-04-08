#pragma once

#include "Character.h"

class Projectile : public Character
{

private:

	static std::vector<Projectile*> listProjectiles;

public:

	static void updateProjectiles(float dt);
	static const std::vector<Projectile*>& getListProjectiles();

private:

	Timer* durationTimer;

public:

	Projectile(Projectile* projectile, Tilemap* tilemap, glm::vec3 position, bool flipped);
	Projectile();
	~Projectile();

	// === Functions ===

	void update(float dt);
	void destroy();

	// === Accessors ===



};