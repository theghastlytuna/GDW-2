#pragma once
#include "Tone Fire/Tonefire.h"
#include "Scene.h"

class BarBreaker : public Scene
{
public:
	BarBreaker(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	void GUI() override;

	void GUIWindowUI();
	void GUIWindowOne();
	void GUIWindowTwo();

	void AdjustScrollOffset();

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:

	bool m_firstWindow = false;
	bool m_secondWindow = false;

	ToneFire::CoreSound backgroundMusic{"test2.mp3"};
};
