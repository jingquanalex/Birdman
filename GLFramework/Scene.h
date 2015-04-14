#pragma once

#include "Timer.h"
#include "Camera.h"
#include "Cursor.h"
#include "Sprite.h"
#include "Tilemap.h"
#include "Character.h"
#include "Guy.h"
#include "NPC.h"
#include "freetype.h"

// Manages the player, map, objects, score, game logic.
class Scene
{

private:

	bool devMode = 0;
	int stateLoaded = 0;
	int gameState = 1;
	int gameWon = 0;
	bool hardMode = 0;
	Camera* camera;
	Cursor* cursor;
	Sprite* bgGreen;
	Sprite* bgBrown;
	Tilemap* tilemap;
	Guy* guy;
	NPC* npc;
	NPC* npcBlue;
	Sprite* coin;
	std::vector<Sprite*> listCoins;
	glm::vec3 guyStartPosition = glm::vec3(545, -1859, 0);
	glm::vec3 camStartPosition = glm::vec3(565, -2100, 0);
	int spawnNpcNext = 0;
	Sprite* blanket;
	Timer* fadeTimer;
	Timer* spawnTimer;
	Timer* gameTimer;
	int secondsElasped = 0;

public:

	Scene(Camera* camera, Cursor* cursor);
	~Scene();

	// === Functions ===

	void load();
	void update(float dt);
	void draw();

	void resetScene();
	void gameOver();

	// === Callbacks ===

	void mouse(int button, int state);
	void keyboard(unsigned char key);
	void keyboardUp(unsigned char key);
	void keyboardSpecial(int key);
	void keyboardSpecialUp(int key);

	// === Accessors ===

	int getIsLoaded() const;

};