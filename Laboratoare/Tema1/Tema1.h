#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

using namespace std;

class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;
		float count = -1;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void RanderBow(float deltaTimeSeconds);
		void RanderArrow(float deltaTimeSeconds);
		void RanderSuriken(float deltaTimeSeconds);
		void RanderBalloon(float deltaTimeSeconds);
		void RenderPowerBar();
		void RanderScoreBar();
		void RanderLifeStatus();
		bool CheckCollisionBallonArrow();
		bool CheckCollisionBowSuriken();
		bool CheckCollisionArrowSuriken();
		void SetAngleArrow();


	protected:
		glm::mat3 modelMatrix;

		// Variabile pentru sageata
		bool releaseArrow;
		int arrowSpeed;
		float angleArrow, angleReleasedArrow;
		pair<float, float> arrowHead;
		pair<float, float> tArrow;
		bool collisionBalloon, collisionSurikenArrow, collisionSurikenBow;

		// Variabile pentru Suriken
		int currentS;
		float angleSuriken, scaleSuriken;
		pair<float, float> tSuriken;
		vector<pair<float, float>> centerS;

		// Variabile pentru balon
		int currentB;
		float scaleBalloon;
		pair<float, float> tBalloon;
		vector<pair<float, float>> centerB;

		// Variabile pentru arc
		pair<float, float> tBow;
		pair<float, float> centerBow;

		// Alte variabile
		float scalePowerBar;
		int scaleScoreBar;
		int lives;
		pair<int, int> coordMouse;
};
