#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{
	// Creeaza arc cu centrul in (0, 0, 0) de raza "length"
	Mesh* CreateBow(std::string name, float length, glm::vec3 color);

	// Creeaza sageata cu corner-ul in (0, 0, 0) de lungime "length"
	Mesh* CreateArrow(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);

	// Creeaza un suriken cu centrul in (0, 0, 0) de raza "length"
	Mesh* CreateSuriken(std::string name, glm::vec3 centerSuriken, float length, glm::vec3 color);

	// Creeaza un balon cu centrul in (0, 0, 0) de raza "length"
	Mesh* CreateBalloon(std::string name, float length, glm::vec3 color);

	// Creeaza codita de la balon cu centrul in (0, 0, 0)
	Mesh* CreateBalloonTail(std::string name, float length, glm::vec3 color);

	// Creeaza un patrat cu corner-ul dar, de latura "length" 
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill);
}

