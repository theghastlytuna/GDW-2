#pragma once
#include "Tone Fire/Tonefire.h"
#include "Scene.h"
#include "BarBreakerListener.h"

class BarBreaker : public Scene
{
public:
	BarBreaker(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void UpdateCamera();

	void Update() override;

	void AdjustScrollOffset();

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

	struct pos
	{
		float x = 0.f, y = 0.f;
	};

protected:
	BarBreakerListener listener;

	unsigned int player1;
	unsigned int player2;

	ToneFire::CoreSound backgroundMusic{"test2.mp3"};
};
