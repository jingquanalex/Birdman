#pragma once

#include "Character.h"

#define KEY_SPACE 32

class Guy : public Character
{

private:

	int score = 0;

public:

	Guy(glm::vec3 position);

	// === Functions ===

	void update(float dt);
	void destroy();

	// === Callbacks ===

	void keyboardSpecial(int key);
	void keyboardSpecialUp(int key);
	void keyboard(unsigned char key);
	void keyboardUp(unsigned char key);

};