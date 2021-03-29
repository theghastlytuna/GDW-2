#pragma once
#include <vector>
#include "Tone Fire/Tonefire.h"
#include "Scene.h"
#include "BarBreakerListener.h"
#include <vector>

struct Button
{
	unsigned int entity;
	unsigned int textImage;
	vec2 min;
	vec2 max;
	int startHoverTime;
	bool hovering;
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

	void ThrowChair();

	void CheckGame();

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

	void AnimationUpdate();

	void UIUpdate();

	void BoundaryUpdate();

	void MouseMotion(SDL_MouseMotionEvent evnt) override;
	void MouseClick(SDL_MouseButtonEvent evnt) override;

	void SmallMoveRight();

	void SmallMoveLeft();

	void BigMoveRight();

	void BigMoveLeft();

	void LightAttack();

	void HeavyAttack();

	void EndTurn();

	void SwitchPlayer();

	void BottleInteractable(float xCord, float yCord);

	void ChairInteractable(float xCord, float yCord);

	void EndGame();

	void PickupBottle();

	void PlayAttackSound();
	void PlayThrowSound();

	bool IsFinished() override;

	int GetWinner() override;

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

	unsigned int p1HealthBar;
	unsigned int p2HealthBar;
	unsigned int p1HealthBarOutline;
	unsigned int p2HealthBarOutline;
	unsigned int helpTextImage;

	std::vector<unsigned int> bottle;
	std::vector<unsigned int> chair;

	int movesTaken;
	int heavyMoves = 0;
	int lightMoves = 0;
	float playerDistance;
	bool moveCam;
	bool throwing = false;

	double p1Health = 200.f;
	double p2Health = 200.f;

	Button smallJumpLButton;
	Button bigJumpLButton;
	Button smallJumpRButton;
	Button bigJumpRButton;
	Button lightAttackButton;
	Button heavyAttackButton;
	Button interactButton;
	Button helpButton;

	std::vector<Button*> buttonVec;
	int buttonVecLen = 8;

	float boundaryDistanceRightActive;
	float boundaryDistanceLeftActive;
	float boundaryDistanceRightInactive;
	float boundaryDistanceLeftInactive;

	bool endGame = false;
	bool turnEnd = false;
	bool counting = false;
	double beginClk;

	ToneFire::CoreSound backgroundMusic{ "bg1.mp3" };
	ToneFire::CoreSound punch{ "punch.wav" };
	ToneFire::CoreSound kick{ "kick.mp3" };

	//First player voice clips
	ToneFire::CoreSound fpPunchGrunt1{ "fpPunchGrunt1.wav" };
	ToneFire::CoreSound fpPunchGrunt2{ "fpPunchGrunt2.wav" };
	ToneFire::CoreSound fpPunchGrunt3{ "fpPunchGrunt3.wav" };
	ToneFire::CoreSound fpPunchGrunt4{ "fpPunchGrunt4.wav" };
	ToneFire::CoreSound fpThrowGrunt1{ "fpThrowGrunt1.wav" };
	ToneFire::CoreSound fpThrowGrunt2{ "fpThrowGrunt2.wav" };

	//Second player voice clips
	ToneFire::CoreSound spPunchGrunt1{ "spPunchGrunt1.wav" };
	ToneFire::CoreSound spPunchGrunt2{ "spPunchGrunt2.wav" };
	ToneFire::CoreSound spPunchGrunt3{ "spPunchGrunt3.wav" };
	ToneFire::CoreSound spPunchGrunt4{ "spPunchGrunt4.wav" };
	ToneFire::CoreSound spThrowGrunt1{ "spThrowGrunt1.wav" };
	ToneFire::CoreSound spThrowGrunt2{ "spThrowGrunt2.wav" };

private:
	bool finished = false;
	int winner;
};