#include "Scene.h"
#pragma once

class EndScreen : public Scene
{
public:
	EndScreen(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	//void UpdateCamera();

	void Update() override;

	bool IsFinished() override;
	//void AdjustScrollOffset();

	void SetWinner(int) override;

private:
	int winner;
	float m_windowWidth;
	float m_windowHeight;
};

