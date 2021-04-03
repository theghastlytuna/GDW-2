#include "Scene.h"
#pragma once

class OpenScreen : public Scene
{
public:
	OpenScreen(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	//void UpdateCamera();

	void Update() override;

	bool IsFinished() override;
	//void AdjustScrollOffset();

	void SetWinner(int) override;

	void KeyboardDown() override;

private:
	int winner;
	bool finished = false;
	float m_windowWidth;
	float m_windowHeight;
};

