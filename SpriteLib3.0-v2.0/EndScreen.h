#include "Scene.h"
#pragma once

class EndScreen : public Scene
{
public:
	EndScreen(std::string name, int in_winner);

	void InitScene(float windowWidth, float windowHeight) override;

	//void UpdateCamera();

	void Update() override;

	bool IsFinished() override;
	//void AdjustScrollOffset();

private:
	int winner;
	float m_windowWidth;
	float m_windowHeight;
};

