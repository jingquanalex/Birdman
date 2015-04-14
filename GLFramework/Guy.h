#pragma once

#include "Character.h"
#include "Projectile.h"

#define KEY_SPACE 32

class Guy : public Character
{

private:

	Projectile* projectile;
	Timer* tHurtFlash;

	bool jumpPressed = 0, punchPressed = 0, kickPressed = 0;
	float oldMoveSpeed;
	int statePunching = 0, stateKicking = 0;
	int kickCharges = 1;

	int score = 0;
	int coinsCollected = 0, npcsKilled = 0;
	int frozen = 0;

public:

	Guy(glm::vec3 position);
	~Guy();

	// === Functions ===

	void update(float dt);

	void bounce();
	void damageTaken();
	void punch();
	void kick();
	void collectCoin();
	void killedNpc();
	void reset();
	void freeze();
	void addScore(int score);

	// === Callbacks ===

	void keyboardSpecial(int key);
	void keyboardSpecialUp(int key);
	void keyboard(unsigned char key);
	void keyboardUp(unsigned char key);

	// === Accessors ===

	int getCoinsCollected() const;
	int getNpcsKilled() const;
	int getScore() const;

};