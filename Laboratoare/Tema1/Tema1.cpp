#include "Tema1.h"

#include <vector>
#include <iostream>
#include <cmath> 

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;
#define WINDOW_Y 720
#define WINDOW_X 1280
#define LENGTH 100
#define OBJECT_SPEED 300
#define ARROW_INIT_X 40
#define SURIKEN_INIT_Y 100
#define BALLOON_INIT_X 400
#define SCALE_FACTOR 0.1f

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glLineWidth(2.5f);

	/* Initializare variabile pentru sageata */
	releaseArrow = false;
	angleArrow = 0;
	collisionBalloon = false;
	arrowSpeed = OBJECT_SPEED;
	tArrow = std::make_pair(0, 0);
	arrowHead = std::make_pair(0, 0);


	/* Initializare variabile pentru balon */
	tBalloon = std::make_pair(0, 0);
	currentB = rand() % 5;
	/* Se initializeaza centrele pentru 5 baloane */
	for (int i = 0; i < 5; i++) {
		centerB.push_back(std::make_pair(0, 0));
	}


	/* Initializare variabile pentru suriken */
	angleSuriken = 0;
	scaleSuriken = 1;
	currentS = rand() % 6;
	collisionSurikenArrow = false;
	collisionSurikenBow = false;
	tSuriken = std::make_pair(0, 0);
	/* Se initializeaza centrele pentru 6 suriken */
	for (int i = 0; i <= 5; i++) {
		centerS.push_back(std::make_pair(WINDOW_X, 0));
	}


	/* Initializare variabile pentru arc */
	tBow = std::make_pair(0, 0);
	centerBow = std::make_pair(0, 0);


	/* Initializare alte variabile */
	scalePowerBar = 0;
	scaleScoreBar = 1;
	lives = 3;
	coordMouse = std::make_pair(0, 0);


	glm::vec3 center = glm::vec3(0, 0, 0);
	
	/* Crearea obictelor din joc*/
	Mesh* bow = Object2D::CreateBow("bow", 0.75f * LENGTH, glm::vec3(1, 0, 0));
	AddMeshToList(bow);

	Mesh* arrow = Object2D::CreateArrow("arrow", glm::vec3(ARROW_INIT_X, 0, 0), LENGTH, glm::vec3(0, 1, 0));
	AddMeshToList(arrow);

	Mesh* suriken = Object2D::CreateSuriken("suriken", center, LENGTH / 2, glm::vec3(0, 0.5f, 1));
	AddMeshToList(suriken);

	Mesh* balloonRed = Object2D::CreateBalloon("balloonRed", LENGTH/3, glm::vec3(1, 0, 0));
	Mesh* balloonTailRed = Object2D::CreateBalloonTail("balloonTailRed", LENGTH/3, glm::vec3(1, 0, 0));
	AddMeshToList(balloonRed);
	AddMeshToList(balloonTailRed);

	Mesh* balloonYellow = Object2D::CreateBalloon("balloonYellow", LENGTH / 3, glm::vec3(1, 1, 0));
	Mesh* balloonTailYellow = Object2D::CreateBalloonTail("balloonTailYellow", LENGTH / 3, glm::vec3(1, 1, 0));
	AddMeshToList(balloonYellow);
	AddMeshToList(balloonTailYellow);

	Mesh* bar = Object2D::CreateSquare("bar", center, 10, glm::vec3(0, 1, 0), true);
	AddMeshToList(bar);

	Mesh* life = Object2D::CreateSquare("life", center, 15, glm::vec3(1, 0, 0), true);
	AddMeshToList(life);
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


/* Calculeaza unghiul cu care trebuie sa fie rotita sageata
   pentru a arata spre pozitia mouse-ului */
void Tema1::SetAngleArrow()
{
	float dx = coordMouse.first - centerBow.first;
	float dy = WINDOW_Y / 2 - tBow.second - coordMouse.second;
	float tg = dy / dx;

	/* Unghiul este arctg de raportul dintre laturile triunghiului
	   dreptunghic format de mouse si sageata */
	angleArrow = atan(tg);
}


/* Verifica daca s-a facut coliziune balon-sageata */
bool Tema1::CheckCollisionBallonArrow()
{
	/* Coliziune punct-cerc intre varful sagetii si balon */
	int dx = abs(centerB[currentB].first - arrowHead.first);
	int dy = abs(centerB[currentB].second - arrowHead.second);
	float distance = sqrt(dx * dx + dy * dy);

	if (distance < 1.25f * LENGTH)
		return true;
	return false;
}


/* Verifica daca s-a facut coliziune suriken-sageata */
bool Tema1::CheckCollisionArrowSuriken()
{
	/* Coliziune punct-cerc intre varful sagetii si suriken */
	if (releaseArrow == true) {
		int dx = abs(centerS[currentS].first - arrowHead.first);
		int dy = abs(centerS[currentS].second - arrowHead.second);
		float distance = sqrt(dx * dx + dy * dy);

		if (distance < 1.4f * LENGTH)
			return true;
	}
	return false;
}


/* Verifica daca s-a facut coliziune suriken-arc */
bool Tema1::CheckCollisionBowSuriken()
{
	/* Coliziune cerc-cerc intre arc si suriken*/
	int dx = abs(centerBow.first - centerS[currentS].first);
	int dy = abs(centerBow.second - centerS[currentS].second);
	float distance = sqrt(dx * dx + dy * dy);

	if (distance < 1.75f * LENGTH)
		return true;
	return false;
}


/* Randeaza arcul */
void Tema1::RanderBow(float deltaTimeSeconds)
{
	/* Actualizare centru arc*/
	centerBow.second = WINDOW_Y / 2 + tBow.second;
	 
	/* Rotatia arcului dupa mouse si apoi translatie*/
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, centerBow.second);
	modelMatrix *= Transform2D::Rotate(angleArrow);
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);
}


/* Randeaza sageata */
void Tema1::RanderArrow(float deltaTimeSeconds)
{
	/* Variabila temporara in care retinem ty pentru sageata*/
	float tArrowY = tArrow.second;

	/* Cazul in care sageata este eliberata*/
	if (releaseArrow)
	{
		/* Calculam noile variabile de translatie */
		tArrow.first += deltaTimeSeconds * arrowSpeed;
		tArrowY = tArrow.second + tArrow.first * tan(angleReleasedArrow);

		/* Rotatie cu unghiul obtinut anterior si apoi translatie */
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(tArrow.first, tArrowY);
		modelMatrix *= Transform2D::Rotate(angleReleasedArrow);
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);

		/* Daca sageata a atins marginile ferestrei sau s-a intamplat vreo 
		   coliziune se reseteaza tot la valorile initiale */
		if (tArrow.first >= WINDOW_X || tArrowY >= WINDOW_Y || tArrowY <= 0 
									 || collisionBalloon || collisionSurikenArrow)
		{
			tArrow.first = ARROW_INIT_X;
			tArrow.second = centerBow.second;
			arrowSpeed = OBJECT_SPEED;
			releaseArrow = false;
		}
		/* Pentru ca scalarea PowerBar-ului sa se reseteze cand sageata e trimisa */
		scalePowerBar = 0;
	}

	/* Cazul in care sageata NU este eliberata*/
	else {
		angleReleasedArrow = angleArrow;
		tArrow.first = ARROW_INIT_X;
		tArrow.second = centerBow.second;

		/* Rotatie cu unghiul curent si apoi translatie in centrul arcului */
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(0, centerBow.second);
		modelMatrix *= Transform2D::Rotate(angleArrow);
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
	}

	/* Actualizarea coordonatelor varfului sagetii */
	arrowHead.first = tArrow.first + LENGTH;
	arrowHead.second = tArrowY;
}


/* Randeaza suriken-ul*/
void Tema1::RanderSuriken(float deltaTimeSeconds)
{
	/* Unghiul de rotire al suriken-ului */
	angleSuriken += deltaTimeSeconds * 200;

	/* Cazul in care suriken-ul este inca in cadru si nu se intampla o coliziune */
	if (tSuriken.first <= WINDOW_X && !collisionSurikenBow && !collisionSurikenArrow)
	{
		/* Factorul de translatie al suriken-ului */
		tSuriken.first += deltaTimeSeconds * OBJECT_SPEED;

		/* Rotatie in jurul centrului propriu si translatie de la dreapta ecranului*/
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(WINDOW_X - tSuriken.first, 
											  centerS[currentS].second);
		modelMatrix *= Transform2D::Rotate(RADIANS(angleSuriken));
		RenderMesh2D(meshes["suriken"], shaders["VertexColor"], modelMatrix);
	}

	/* Cazul in care suriken-ul a iesit din ecran, a lovit arcul 
	   sau trebuie sa dispara dupa ce animatie de lovire s-a terminat*/
	else if (collisionSurikenBow || tSuriken.first >= WINDOW_X 
								 || scaleSuriken < SCALE_FACTOR) {
		/* Actualizare variabile si trecem la un alt suriken ales random */
		scaleSuriken = 1;
		tSuriken.first = 0;
		currentS = rand() % 6;
	}

	/* Cazul in care sageata loveste suriken-ul */
	else {
		/* Rotire suriken, scalare pana dispare si apoi translatie */
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(WINDOW_X - tSuriken.first, 
											  centerS[currentS].second);
		modelMatrix *= Transform2D::Scale(scaleSuriken, scaleSuriken);
		modelMatrix *= Transform2D::Rotate(RADIANS(angleSuriken));
		RenderMesh2D(meshes["suriken"], shaders["VertexColor"], modelMatrix);

		/* Factorul de scalare scade constant pana la o limita*/
		scaleSuriken -= SCALE_FACTOR;
		collisionSurikenArrow = false;
	}

	/* Actualizam centrul suriken-ului curent */
	centerS[currentS].first = WINDOW_X - tSuriken.first;
	centerS[currentS].second = SURIKEN_INIT_Y + currentS * 100;
}


/*Randeaza balonul*/
void Tema1::RanderBalloon(float deltaTimeSeconds)
{
	/* Cazul in care suriken-ul este inca in cadru si nu se intampla o coliziune */
	if (centerB[currentB].second <= WINDOW_Y && !collisionBalloon)
	{
		/* Factorul de translatie al balonului */
		tBalloon.second += deltaTimeSeconds * (OBJECT_SPEED / 2);

		/* Scalare pentru ca balonul sa fie un cerc alungit si apoi translatie */
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(centerB[currentB].first,
											  centerB[currentB].second);
		modelMatrix *= Transform2D::Scale(1, 1.25f);

		/* Pentru indici pari se randeaza baloane galbene, pentru restul baloane rosii */
		if (currentB % 2 == 1) 
		{
			RenderMesh2D(meshes["balloonYellow"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["balloonTailYellow"], shaders["VertexColor"], modelMatrix);
		}
		else {
			RenderMesh2D(meshes["balloonRed"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["balloonTailRed"], shaders["VertexColor"], modelMatrix);
		}
	}

	/* Cazul in care balonul a iesit din ecran sau trebuie sa 
	   dispara dupa ce animatie de lovire s-a terminat*/
	else if (centerB[currentB].second >= WINDOW_Y || scaleBalloon < SCALE_FACTOR){
		/* Actualizare variabile si trecem la un alt balon ales random */
		tBalloon.second = 0;
		scaleBalloon = 1;
		currentB = rand() % 5;
	}

	/* Cazul in care sageata loveste balonul */
	else {
		/* Scalare pentru disparitia balonului cand a fost lovit si apoi translatie */
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(centerB[currentB].first, 
											  centerB[currentB].second);
		modelMatrix *= Transform2D::Scale(scaleBalloon, 1.25f * scaleBalloon);

		/* Pentru indici pari se randeaza baloane galbene, iar pt cei impari baloane rosii */
		if (currentB % 2 == 1)
		{
			RenderMesh2D(meshes["balloonYellow"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["balloonTailYellow"], shaders["VertexColor"], modelMatrix);
		}
		else {
			RenderMesh2D(meshes["balloonRed"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["balloonTailRed"], shaders["VertexColor"], modelMatrix);
		}

		/* Factorul de scalare scade constant pana la o limita*/
		scaleBalloon -= SCALE_FACTOR;
		collisionBalloon = false;
	}

	/* Actualizam centrul balonului curent */
	centerB[currentB].first = BALLOON_INIT_X + currentB * 200;
	centerB[currentB].second = tBalloon.second;
}


/* Randeaza PowerBar-ul */
void Tema1::RenderPowerBar()
{
	/* Scalare cu scalePoweBar care tine si scorul si translatie in sus-ul ecranului*/
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(ARROW_INIT_X, 
										  WINDOW_Y / 2 + tBow.second - 1.25f * LENGTH);
	modelMatrix *= Transform2D::Scale(scalePowerBar, 1);
	RenderMesh2D(meshes["bar"], shaders["VertexColor"], modelMatrix);
}


/* Randeaza ScoreBar-ul */
void Tema1::RanderScoreBar()
{
	/* Cresterea scorului cand se loveste un suriken*/
	if (collisionSurikenArrow && scaleSuriken == 1)
		scaleScoreBar++;

	/* Cresterea/Scaderea scorului cand se loveste un balon*/
	if (collisionBalloon && scaleBalloon == 1)
	{
		/* Baloanele galbene scad scorul */
		if (currentB == 1 || currentB == 3) 
		{
			if (scaleScoreBar > 1)
				scaleScoreBar--;
		}
		/*Baloanele rosii cresc scorul */
		else
			scaleScoreBar++;
	}

	/*Scalare pentru cresterea scorului si translatie in susul scenei */
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(10, 700);
	modelMatrix *= Transform2D::Scale(scaleScoreBar, 1.25f);
	RenderMesh2D(meshes["bar"], shaders["VertexColor"], modelMatrix);
}


/* Randeaza romburile care semnifica viata ramasa */
void Tema1::RanderLifeStatus()
{
	/* Se afiseaza romburi in coltul dreapta sus, reprezentand vietile ramase*/
	for (int i = 0; i < lives; i++) 
	{
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1260 - i * 30, 670);
		modelMatrix *= Transform2D::Scale(1, 1.5f);
		modelMatrix *= Transform2D::Rotate(RADIANS(45));
		RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
	}
}


void Tema1::Update(float deltaTimeSeconds)
{
	/* Se testeaza toate coliziunile*/
	collisionSurikenBow = CheckCollisionBowSuriken();
	collisionSurikenArrow = CheckCollisionArrowSuriken();
	collisionBalloon = CheckCollisionBallonArrow();

	/* Se pierde o viata daca un suriken loveste arcul */
	if (collisionSurikenBow == true)
		lives--;

	/* Daca toate vietile s-au pierdut, fereastra se inchide si se
       afiseaza "GAME OVER" si scorul obtinut in consola */
	if (lives == 0) {
		printf("GAME OVER\n");
		printf("Score: %d\n", (scaleScoreBar - 1) * 10);
		window->Close();
	}

	/* Se randeaza ScoreBar-ul, PowerBar-ul si statusul de viata */
	RanderScoreBar();
	RanderLifeStatus();
	RenderPowerBar();

	/* Aflam unghiul de rotire al sagetii dupa mouse*/
	SetAngleArrow();

	/* Se randeaza obiectele jocului */
	RanderSuriken(deltaTimeSeconds);
	RanderBow(deltaTimeSeconds);
	RanderBalloon(deltaTimeSeconds);
	RanderArrow(deltaTimeSeconds);
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	/* Deplasarea arcului in sus la apasarea tastei W */
	int limit = WINDOW_Y / 2 - LENGTH - 25;
	if (window->KeyHold(GLFW_KEY_W) && tBow.second <= limit) {
		tBow.second += deltaTime * OBJECT_SPEED;
	}

	/* Deplasarea arcului in jos la apasarea tastei S */
	if (window->KeyHold(GLFW_KEY_S) && tBow.second >= -limit) {
		tBow.second -= deltaTime * OBJECT_SPEED;
	}

	/* Scalarea PowerBar-ului si cresterea vitezei de deplasare a
	   sagetii la apasarea continua pe mouse */
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		if (scalePowerBar < 10 && releaseArrow == false) {
			scalePowerBar += 0.15f;
			arrowSpeed += 5;
		}
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	/* Retinerea coordonatelor mouse-ului */
	coordMouse.first = mouseX;
	coordMouse.second = mouseY;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event

}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	/* Se elibereaza sageata cand se da drumul la butonul mouse-ului */
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
	{
		releaseArrow = true;
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
