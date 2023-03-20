#include "StationaryCamera.h"

void StationaryCamera::SetRotation(glm::vec3 rotation)
{
	m_theta = rotation.x;
	m_phi = rotation.y;
}