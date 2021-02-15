#pragma once
#include "Tone Fire/Tonefire.h"
#include "Scene.h"

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
	ToneFire::CoreSound backgroundMusic{"test2.mp3"};
};
