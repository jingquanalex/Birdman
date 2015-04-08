#pragma once

#include "Character.h"

enum class NPCTYPE
{
	RED, BLUE
};

enum class NPCSTATE
{
	IDLE, MOVING, STOMPED, FADING
};

class NPC : public Character
{

private:

	static std::vector<NPC*> listNPCs;

public:

	static std::vector<NPC*>& getListNPCs();
	static void updateNPCs(float dt);

private:

	NPCTYPE type;
	NPCSTATE state;
	Timer* stompTimer;
	Timer* fadeTimer;
	bool isThreat = true;

public:

	NPC(NPC* npc, glm::vec3 position);
	NPC(glm::vec3 position, NPCTYPE type);
	~NPC();

	// === Functions ===

	void update(float dt);
	void stomped();
	void struck();
	void destroy();

	// === Accessors ===

	NPCSTATE getState() const;
	NPCTYPE getType() const;
	bool getIsThreat() const;

};