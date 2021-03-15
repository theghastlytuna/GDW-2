#pragma once
#include <vector>
#include <map>
#include <math.h>
#include "Tone Fire/Tonefire.h"
#include "Scene.h"
#include "BarBreakerListener.h"
#include "HasBottle.h"

class BarBreaker : public Scene
{
public:
	BarBreaker(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void ThrowBottle();

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

	void LightAttack();

	void EndTurn();

	void SwitchPlayer();

	void PickupBottle();

	struct pos
	{
		float x = 0.f, y = 0.f;
	};


protected:
	const int playerSize = 40;
	const int bottleSize = 20;

	bool m_firstWindow = false;
	bool m_secondWindow = false;

	BarBreakerListener listener;

	unsigned int player1;
	unsigned int player2;
	unsigned int boundaryRight;
	unsigned int boundaryLeft;
	unsigned int activePlayer;
	unsigned int inactivePlayer;
	std::vector<unsigned int> bottle;
	
	int movesTaken;
	int heavyMoves = 0;
	int lightMoves = 0;
	float playerDistance;
	bool moveCam;

	bool turnEnd = false;
	bool counting = false;
	double beginClk;

	ToneFire::CoreSound backgroundMusic{"bg1.mp3"};
};
