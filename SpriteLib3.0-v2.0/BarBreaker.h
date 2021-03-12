#pragma once
#include "Tone Fire/Tonefire.h"
#include "Scene.h"
#include "BarBreakerListener.h"
#include <vector>

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

	void ThrowBottle();

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


	void LightAttack();

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
	int heavyMoves = 0;
	int lightMoves = 0;
	float playerDistance;
	bool moveCam;

	Button smallJumpLButton;
	Button bigJumpLButton;
	Button smallJumpRButton;
	Button bigJumpRButton;
	Button lightAttackButton;
	Button heavyAttackButton;
	Button interactButton;

	std::vector<Button*> buttonVec;
	int buttonVecLen = 7;

	float boundaryDistanceRightActive; 
	float boundaryDistanceLeftActive; 
	float boundaryDistanceRightInactive; 
	float boundaryDistanceLeftInactive; 

	bool turnEnd = false;
	bool counting = false;
	double beginClk;

	ToneFire::CoreSound backgroundMusic{"bg1.mp3"};
};