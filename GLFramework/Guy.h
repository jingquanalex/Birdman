#pragma once

#include "Character.h"

#define KEY_SPACE 32

class Guy : public Character
{

private:

	bool jumpPressed = 0;
	int score = 0;
	Timer* tHurtFlash;

public:

	Guy(glm::vec3 position);
	~Guy();

	// === Functions ===

	void update(float dt);

	void jump();
	void bounce();
	void damageTaken();

	// === Callbacks ===

	void keyboardSpecial(int key);
	void keyboardSpecialUp(int key);
	void keyboard(unsigned char key);
	void keyboardUp(unsigned char key);

};