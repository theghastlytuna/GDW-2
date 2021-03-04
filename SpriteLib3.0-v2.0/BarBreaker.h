#pragma once
#include "Tone Fire/Tonefire.h"
#include "Scene.h"
#include "BarBreakerListener.h"

class BarBreaker : public Scene
{
public:
	BarBreaker(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void ThrowBottle();

	void Update() override;

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

	struct pos
	{
		float x = 0.f, y = 0.f;
	};

protected:
	BarBreakerListener listener;

	unsigned int player1;
	unsigned int player2;
	unsigned int activePlayer;
	unsigned int inactivePlayer;
	int movesTaken;
	int heavyMoves = 0;
	int lightMoves = 0;
	float playerDistance;

	ToneFire::CoreSound backgroundMusic{"test2.mp3"};
};
