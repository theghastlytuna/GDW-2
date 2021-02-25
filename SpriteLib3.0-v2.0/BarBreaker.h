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

	void GUI() override;

	void GUIWindowUI();
	void GUIWindowOne();
	void GUIWindowTwo();

	void AdjustScrollOffset();

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

	void SmallMoveRight();

	void SmallMoveLeft();

	void BigMoveRight();

	void BigMoveLeft();


	void Punch();

	void EndTurn();

	void SwitchPlayer();

	struct pos
	{
		float x = 0.f, y = 0.f;
	};

protected:

	bool m_firstWindow = false;
	bool m_secondWindow = false;

	BarBreakerListener listener;

	unsigned int player1;
	unsigned int player2;
	unsigned int activePlayer;
	unsigned int inactivePlayer;
	int movesTaken;
	float playerDistance;

	ToneFire::CoreSound backgroundMusic{"bg1.mp3"};
};
