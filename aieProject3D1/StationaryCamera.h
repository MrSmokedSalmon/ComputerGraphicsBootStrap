#pragma once
#include "BaseCamera.h"
class StationaryCamera : public BaseCamera
{
public:
	void SetRotation(glm::vec3 rotation);
};

