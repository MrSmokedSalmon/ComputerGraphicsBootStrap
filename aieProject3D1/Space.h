#pragma once

#include "AstralBody.h"
#include <vector>

class Space
{
public:
	Space();
	~Space();

	void Update(float dt);
	void Draw();

	void AddPlanet(glm::vec3 pos, float rotSpeed, float rad, float ringWidth = 0, float ringAngle = 0) {
		AstralBody* newPlanet = new AstralBody(pos, rotSpeed, rad, ringWidth, ringAngle);
		objects.push_back(newPlanet);
	}

private:
	std::vector<AstralBody*> objects;
};

