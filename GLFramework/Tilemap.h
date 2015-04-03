#pragma once

#include "globalIncludes.h"
#include "Sprite.h"
#include <iostream>
#include <fstream>

struct Item
{
	glm::vec3 position;
	int type;
};

class Tilemap : public Sprite
{

private:

	glm::vec2 tileSize;
	glm::vec2 mapTileSize;
	std::vector<float> vertices;
	std::vector<float> texcoords;
	std::vector<std::vector<int>> mapValues;
	std::vector<Item> listItems;

	// === Private Functions ===

	void addQuadData(int posx, int posy, int value);
	void addItemData(int posx, int posy, int value);

public:

	Tilemap(std::string tileTexture, glm::vec2 tileSize, std::string mapData, glm::vec2 mapTileSize);
	~Tilemap();

	// === Functions ===

	void load(std::string mapData);
	void update(float dt);
	void draw();

	void loadMap(std::string mapData);
	glm::vec2 getCoordAtPos(glm::vec3 position) const;
	int getValueAtCoord(int x, int y) const;
	int getValueAtCoord(glm::vec2 coord) const;
	int getValueAtPos(glm::vec3 position) const;
	glm::vec3 getPositionAtCoord(int x, int y) const;
	glm::vec3 getPositionAtCoord(glm::vec2 coord) const;
	glm::vec2 getMapTileSize() const;

	// === Accessors ===

	const std::vector<std::vector<int>>& getMapValues() const;
	const std::vector<Item>& getListItems() const;

};