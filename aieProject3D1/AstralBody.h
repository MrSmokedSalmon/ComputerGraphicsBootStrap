#pragma once
#include "glm/glm.hpp"
#include <vector>

class AstralBody
{
public:
	AstralBody(glm::vec3 pos, float rotSpd, float rad, float ringWdt, float ringAngl);
	~AstralBody();

	void Update(float dt);
	void Draw();

private:
	glm::vec3 position;
	float radius;
	float ringWidth;
	float ringAngle;

	glm::mat4 transform;

	float rotSpeed;
};

