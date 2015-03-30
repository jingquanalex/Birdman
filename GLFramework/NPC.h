#pragma once

#include "Character.h"

enum class NPCTYPE
{
	RED, BLUE
};

enum class NPCSTATE
{
	MOVING, STOMPED, FADING
};

class NPC : public Character
{

private:

	static std::vector<NPC*> listNPCs;

	NPCSTATE state;
	Timer* stompTimer;
	Timer* fadeTimer;

public:

	static std::vector<NPC*> getListNPCs();

	NPC(glm::vec3 position, NPCTYPE type);

	// === Functions ===

	void update(float dt);
	void destroy();

	void stomped();

};