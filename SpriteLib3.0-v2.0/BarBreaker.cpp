#include "BarBreaker.h"
#include "Utilities.h"
#include "Timer.h"
#include "Tone Fire/Tonefire.h"

BarBreaker::BarBreaker(std::string name)
	:Scene(name)
{
	//no gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -9.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}

void BarBreaker::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	Scene::CreateCameraEntity(true, windowWidth, windowHeight, -75.f, 75.f, -75.f, 75.f, -100.f, 100.f, aspectRatio, true, true);
	ECS::GetComponent<Camera>(MainEntities::MainCamera()).Zoom(-20);

	Scene::CreatePlatform("boxSprite.jpg", 800.f, 20, 0, -30.f, 0.f, 0.f, 0.f);

	//ToneFire::CoreSound testSound{ "test2.mp3" };
	//backgroundMusic = testSound;
	backgroundMusic.Play();

	//Setup new Entity
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		player1 = entity;
		activePlayer = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "LinkStandby.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-70.f, 50.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-70.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 1000.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);

	}

	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		player2 = entity;
		inactivePlayer = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "LinkStandby.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(70.f, 50.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(70.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 1000.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(activePlayer));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(activePlayer));
	moveCam = false;
}

void BarBreaker::UpdateCamera() {
	/*lerps camera to active player rather than jumping from player to player*/

	auto& cameraOffset = ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera());
	static float offsetVal = 0;
	if (moveCam) {
		if (activePlayer == player1) {
			if (offsetVal < 79 /*&& offsetVal < 81*/) moveCam = false;
			else offsetVal -= 0.1 * Timer::time;
		}
		if (activePlayer == player2) {
			if (offsetVal > -49 /*&& offsetVal > -51*/) moveCam = false;
			else offsetVal += 0.1 * Timer::time;
		}
		cameraOffset.SetOffset(offsetVal);
	}
	else {
		//reset offsetting values
		if (activePlayer == player1) {
			if (ECS::GetComponent<Transform>(player2).GetPositionX() - ECS::GetComponent<Transform>(player1).GetPositionX() > 210)
				offsetVal = -200;
			else offsetVal = -55;
		}
		if (activePlayer == player2) offsetVal = 200;
	}

}

void BarBreaker::Update()
{
	BarBreaker::UpdateCamera();

	BarBreaker::AdjustScrollOffset();

	playerDistance = (ECS::GetComponent<PhysicsBody>(activePlayer).GetPosition().x - ECS::GetComponent<PhysicsBody>(inactivePlayer).GetPosition().x);

	if (!backgroundMusic.IsPlaying())
	{
		backgroundMusic.Play();
	}

	//UpdateCamera();
}

void BarBreaker::AdjustScrollOffset()
{
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(activePlayer));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(activePlayer));

}

void BarBreaker::KeyboardHold()
{
	float speed = 1.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 5.f;
	}

	//Change physics body size for circle
	if (Input::GetKey(Key::O))
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).ScaleBody(1.3 * Timer::deltaTime, 0);
	}
	else if (Input::GetKey(Key::I))
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).ScaleBody(-1.3 * Timer::deltaTime, 0);
	}

}

void BarBreaker::KeyboardDown()
{
	if(!moveCam){
		if (Input::GetKeyDown(Key::E))
		{
			BigMoveRight();
		}
		if (Input::GetKeyDown(Key::Q))
		{
			BigMoveLeft();
		}
		if (Input::GetKeyDown(Key::A))
		{
			SmallMoveLeft();
		}
		if (Input::GetKeyDown(Key::D))
		{
			SmallMoveRight();
		}

		if (Input::GetKeyDown(Key::W))
		{
			Punch();
		}

		if (Input::GetKeyDown(Key::L))
		{
			PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
		}

		if (Input::GetKeyDown(Key::K))
		{
			BarBreaker::SwitchPlayer();
		}
	}
}

void BarBreaker::KeyboardUp()
{
}

void BarBreaker::SmallMoveRight()
{
	ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(60000.f, 50000.f), true);
	movesTaken++;
	if (movesTaken == 2)
	{
		EndTurn();
	}
}

void BarBreaker::SmallMoveLeft()
{
	ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-60000.f, 50000.f), true);
	movesTaken++;
	if (movesTaken == 2)
	{
		EndTurn();
	}
}

void BarBreaker::BigMoveRight()
{
	ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(100000.f, 90000.f), true);
	movesTaken++;
	if (movesTaken == 2)
	{
		EndTurn();
	}
}

void BarBreaker::BigMoveLeft()
{
	ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-100000.f, 90000.f), true);
	movesTaken++;
	if (movesTaken == 2)
	{
		EndTurn();
	}
}

void BarBreaker::Punch()
{
	ToneFire::CoreSound testSound{ "punch.wav" };

	if (abs(playerDistance) <= 60)
	{
		testSound.Play();
		if (playerDistance < 0)
		{
			ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(60000.f, 80000.f), true);
			EndTurn();

		}
		else if (playerDistance > 0)
		{
			ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-60000.f, 80000.f), true);
			EndTurn();

		}
	}
}

void BarBreaker::EndTurn()
{
	movesTaken = 0;
	SwitchPlayer();
	moveCam = true;
}

void BarBreaker::SwitchPlayer()
{
	moveCam = true;
	if (activePlayer == player1)
	{
		activePlayer = player2;
		inactivePlayer = player1;
	}
	else if (activePlayer == player2)
	{
		activePlayer = player1;
		inactivePlayer = player2;
	}
}

