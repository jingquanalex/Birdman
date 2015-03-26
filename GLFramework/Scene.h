#pragma once

#include "Timer.h"
#include "Camera.h"
#include "Cursor.h"
#include "Sprite.h"
#include "Tilemap.h"
#include "Character.h"
#include "Guy.h"

// Manages the player, map, objects, score, game logic.
class Scene
{

private:

	Camera* camera;
	Cursor* cursor;

public:

	Scene(Camera* camera, Cursor* cursor);

	// === Functions ===

	void load();
	void update(float dt);
	void draw();
	void destroy();

	// === Callbacks ===

	void mouse(int button, int state);
	void keyboard(unsigned char key);
	void keyboardUp(unsigned char key);
	void keyboardSpecial(int key);
	void keyboardSpecialUp(int key);
};