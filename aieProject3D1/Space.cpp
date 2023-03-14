#include "Space.h"

Space::Space()
{
	float radii[9] = { 1.f, 0.1f, 0.17f, 0.18f, 0.15f, 0.35f, 0.28f, 0.21f, 0.21f };
	float edgePoint = -1.f;

	float spacing = 0.25f;

	//for (int i = 0; i < 9; i++)
	//{
	//	edgePoint += radii[i];
	//	AddPlanet(glm::vec3(edgePoint,0.f, edgePoint), 1, radii[i]);
	//	edgePoint += radii[i] + spacing;
	//}

	// SUN
	edgePoint += radii[0];
	AddPlanet(glm::vec3(edgePoint, 0.f, edgePoint), 1, radii[0]);
	edgePoint += radii[0] + spacing;
	// MERCURY
	edgePoint += radii[1];
	AddPlanet(glm::vec3(edgePoint, 0.f, edgePoint), 4.15f, radii[1]);
	edgePoint += radii[1] + spacing;
	// VENUS
	edgePoint += radii[2];
	AddPlanet(glm::vec3(edgePoint, 0.f, edgePoint), 1.62f, radii[2]);
	edgePoint += radii[2] + spacing;
	// EARTH
	edgePoint += radii[3];
	AddPlanet(glm::vec3(edgePoint, 0.f, edgePoint), 1, radii[3]);
	edgePoint += radii[3] + spacing;
	// MARS
	edgePoint += radii[4];
	AddPlanet(glm::vec3(edgePoint, 0.f, edgePoint), 0.53f, radii[4]);
	edgePoint += radii[4] + spacing;
	// JUPITER
	edgePoint += radii[5];
	AddPlanet(glm::vec3(edgePoint, 0.f, edgePoint), 0.08f, radii[5]);
	edgePoint += radii[5] + spacing;
	// SATURN
	edgePoint += radii[6];
	AddPlanet(glm::vec3(edgePoint, 0.f, edgePoint), 0.03f, radii[6], 0.07f);
	edgePoint += radii[6] + spacing;
	// NEPTUN
	edgePoint += radii[7];
	AddPlanet(glm::vec3(edgePoint, 0.f, edgePoint), 0.01f, radii[7], 0.05f);
	edgePoint += radii[7] + spacing;
	// URANUS
	edgePoint += radii[8];
	AddPlanet(glm::vec3(edgePoint, 0.f, edgePoint), 0.02f, radii[8]);
	edgePoint += radii[8] + spacing;
}

void Space::Update(float dt)
{
	int count = objects.size();
	for (int i = 0; i < count; i++)
	{
		objects[i]->Update(dt);
	}
}

void Space::Draw()
{
	int count = objects.size();
	for (int i = 0; i < count; i++)
	{
		objects[i]->Draw();
	}
}
