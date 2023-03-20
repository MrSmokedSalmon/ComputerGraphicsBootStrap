#include "StationaryCamera.h"

void StationaryCamera::SetRotation(glm::vec3 rotation)
{
	float angle = glm::length(rotation);
	glm::vec3 normal = glm::normalize(rotation);

	glm::rotate(m_worldTransform, angle, normal);
}