#pragma once
#include "Tone Fire/Tonefire.h"
#include "Scene.h"
#include "BarBreakerListener.h"

struct Button
{
	unsigned int entity;
	vec2 min;
	vec2 max;
	//ButtonState currentState;
};

enum ButtonState
{
	inactive, //0
	hover,	//1
	click	//2
};

class BarBreaker : public Scene
{
public:
	BarBreaker(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void UpdateCamera();

	void Update() override;

	//void GUI() override;
	//void GUIWindowUI();
	//void GUIWindowOne();
	//void GUIWindowTwo();

	void AdjustScrollOffset();

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

	void MouseMotion(SDL_MouseMotionEvent evnt) override;
	void MouseClick(SDL_MouseButtonEvent evnt) override;

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
	unsigned int boundaryRight;
	unsigned int boundaryLeft;
	unsigned int activePlayer;
	unsigned int inactivePlayer;
	int movesTaken;
	float playerDistance;
	float boundaryDistanceRightActive;
	float boundaryDistanceLeftActive;
	float boundaryDistanceRightInactive;
	float boundaryDistanceLeftInactive;

	unsigned int moveButtonSprite;

	Button moveButton;

	bool turnEnd = false;
	bool counting = false;
	double beginClk;

	ToneFire::CoreSound backgroundMusic{"bg1.mp3"};
};