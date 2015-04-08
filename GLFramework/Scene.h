#pragma once

#include <FTGL/ftgl.h>
#include "Timer.h"
#include "Camera.h"
#include "Cursor.h"
#include "Sprite.h"
#include "Tilemap.h"
#include "Character.h"
#include "Guy.h"
#include "NPC.h"

// Manages the player, map, objects, score, game logic.
class Scene
{

private:

	int stateLoaded = 0;
	Camera* camera;
	Cursor* cursor;
	Sprite* background;
	Tilemap* tilemap;
	Guy* guy;
	NPC* npc;
	Sprite* coin;
	std::vector<Sprite*> listCoins;
	glm::vec3 guyStartPosition = glm::vec3(546, -1856, 0);
	glm::vec3 camStartPosition = glm::vec3(565, -2100, 0);

public:

	Scene(Camera* camera, Cursor* cursor);
	~Scene();

	// === Functions ===

	void load();
	void update(float dt);
	void draw();

	void resetScene();

	// === Callbacks ===

	void mouse(int button, int state);
	void keyboard(unsigned char key);
	void keyboardUp(unsigned char key);
	void keyboardSpecial(int key);
	void keyboardSpecialUp(int key);

	// === Accessors ===

	int getIsLoaded() const;

};