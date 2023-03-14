#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Space.h"

class aieProject3D1App : public aie::Application {
public:

	aieProject3D1App();
	virtual ~aieProject3D1App();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	Space* space;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
};