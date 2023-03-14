#include "AstralBody.h"

#include "Gizmos.h"
#include <glm/ext.hpp>

AstralBody::AstralBody(glm::vec3 pos, float rotSpd, float rad, float ringWdt, float ringAngl)
{
	position = pos;
	rotSpeed = rotSpd;
	radius = rad;
	ringWidth = ringWdt;
	ringAngle = ringAngl;
	transform = glm::mat4(1);
}

void AstralBody::Update(float dt)
{
	transform = glm::mat4(1);
	transform = glm::translate(transform, glm::vec3(glm::sin(dt * rotSpeed) * position.x, 0, glm::cos(dt * rotSpeed) * position.z));
}

void AstralBody::Draw()
{
	aie::Gizmos::addSphere(glm::vec3(0), radius, 22, 22, glm::vec4(1, 0, 0, 1), &transform);
	transform = glm::rotate(transform, ringAngle, glm::vec3(1, 0, 0));
	aie::Gizmos::addRing(glm::vec3(0), radius + ringWidth, radius + ringWidth*2, 20, glm::vec4(0, 1, 0, 1), &transform);
}
