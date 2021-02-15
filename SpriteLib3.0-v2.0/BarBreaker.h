#pragma once
#include "Tone Fire/Tonefire.h"
#include "Scene.h"
#include "BarBreakerListener.h"

class BarBreaker : public Scene
{
public:
	BarBreaker(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	void AdjustScrollOffset();

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	BarBreakerListener listener;

	int player1;
	int player2;
	ToneFire::CoreSound backgroundMusic{"test2.mp3"};
};
