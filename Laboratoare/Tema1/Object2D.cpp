#include "Object2D.h"

#include <Core/Engine.h>


// Creeaza arc cu centrul in (0, 0, 0) de raza "length"
Mesh* Object2D::CreateBow(std::string name, float length, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	int x = length, y = length;
	// Numarul de triunghiuri in care este impartit cercul
	int numTriangles = 100;
	// Unghiul dintr laturile care se intalnesc in centru
	float angle = 2*M_PI / numTriangles;
	// Numarul de puncte folosite
	int countIndices = 0;


	for (int i = 0; i <= numTriangles; i++)
	{
		/* Se calculeaza coordonatele punctului obtinut prin 
		   rotatia punctului (x, y) cu i * angle */
		float coordX = (x * cos(i * angle)) + (y * sin(i * angle));
		float coordY = (x * sin(i * angle)) - (y * cos(i * angle));

		// Se iau doar punctele care pe axa pozitiva a axei Ox pentru a creea semicerc
		if (coordX >= length/2) {
			vertices.push_back(VertexFormat(glm::vec3(coordX, coordY, 0), color));
			countIndices++;
		}
	}

	for (int i = 0; i < countIndices; i++)
		indices.push_back(i);


	Mesh* bow = new Mesh(name);
	bow->InitFromData(vertices, indices);
	bow->SetDrawMode(GL_LINE_LOOP);
	return bow;
}

// Creeaza sageata cu corner-ul in (0, 0, 0) de lungime "length"
Mesh* Object2D::CreateArrow(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	// Indicii formeaza o linie cu un triunghi in capat
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length / 8, 0), color),
		VertexFormat(corner + glm::vec3(length + length / 8, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, -length / 8, 0), color)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 2 };

	Mesh* arrow = new Mesh(name);
	arrow->InitFromData(vertices, indices);
	arrow->SetDrawMode(GL_LINE_STRIP);
	return arrow;
}

// Creeaza un suriken cu centrul in (0, 0, 0) de raza "length"
Mesh* Object2D::CreateSuriken(std::string name, glm::vec3 centerSuriken, float length, glm::vec3 color)
{
	glm::vec3 center = centerSuriken;

	// Se seteaza puncte care formeaza cele 4 triunghiuri ale suriken-ului
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(center, color),	//0
		VertexFormat(center + glm::vec3(length, 0, 0), color),	// 1
		VertexFormat(center + glm::vec3(length, length, 0), color),	// 2
		VertexFormat(center + glm::vec3(0, length, 0), color), // 3
		VertexFormat(center + glm::vec3(-length, length, 0), color), // 4
		VertexFormat(center + glm::vec3(-length, 0, 0), color), // 5
		VertexFormat(center + glm::vec3(-length, -length, 0), color), // 6
		VertexFormat(center + glm::vec3(0, -length, 0), color), // 7
		VertexFormat(center + glm::vec3(length, -length, 0), color) // 8
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,		// Primul triunghi 0, 1, 2
		0, 3, 4,		// Al 2lea triunghi 0, 3, 4
		0, 5, 6,		// Al 3lea triunghi 0, 5, 6
		0, 7, 8			// Al 4lea triunghi 0, 7, 8
	};

	Mesh* suriken = new Mesh(name);
	suriken->InitFromData(vertices, indices);
	suriken->SetDrawMode(GL_TRIANGLES);
	return suriken;
}


// Creeaza un balon cu centrul in (0, 0, 0) de raza "length"
Mesh* Object2D::CreateBalloon(std::string name, float length, glm::vec3 color)
{
	/* Se procedeaza la fel ca la arc prin creearea punctelor de pe cerc.
	   insa se ia de data aceasta tot cercul, nu doar un semiarc */
	int x = length, y = length;
	int numTriangles = 100;
	float angle = 2 * M_PI / numTriangles;

	std::vector<VertexFormat> vertices;
	for (int i = 0; i <= numTriangles; i++)
	{
		float coordX = (x * cos(i * angle)) + (y * sin(i * angle));
		float coordY = (x * sin(i * angle)) - (y * cos(i * angle));
		vertices.push_back(VertexFormat(glm::vec3(coordX, coordY, 0), color));
	}

	std::vector<unsigned short> indices;
	for (int i = 0; i < numTriangles; i++)
		indices.push_back(i);


	Mesh* balloon = new Mesh(name);
	balloon->InitFromData(vertices, indices);
	balloon->SetDrawMode(GL_TRIANGLE_FAN);
	return balloon;
}



// Creeaza codita de la balon cu centrul in (0, 0, 0)
Mesh* Object2D::CreateBalloonTail(std::string name, float length, glm::vec3 color)
{
	// Se seteaza puncte astfel incat codita sa apara in zig-zag
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0, -1.4f*length, 0), color),
		VertexFormat(glm::vec3(-length / 6, -1.65 * length, 0), color),
		VertexFormat(glm::vec3(length / 6, -1.95 * length, 0), color),
		VertexFormat(glm::vec3(-length / 6, -2.15f * length, 0), color)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4};

	Mesh* tail = new Mesh(name);
	tail->InitFromData(vertices, indices);
	tail->SetDrawMode(GL_LINE_STRIP);
	return tail;
}


// Creeaza un patrat cu corner-ul dar, de latura "length" 
Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	Mesh* square = new Mesh(name);
	square->InitFromData(vertices, indices);
	square->SetDrawMode(GL_TRIANGLE_FAN);
	return square;
}