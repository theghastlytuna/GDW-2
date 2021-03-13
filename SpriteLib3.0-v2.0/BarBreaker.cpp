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

	Scene::CreateTransparentPlatform("boxSprite.jpg", 600.f, 20, 0, -40.f, 0.f, 0.f, 0.f);

	backgroundMusic.Play();
	backgroundMusic.SetVolume(0.1);

	/*{
		auto entity = ECS::CreateEntity();
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		std::string fileName = "spritesheets/buttonSheet.png";
		std::string animations = "buttonAnimations.json";
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 49, 59, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
	}*/
	//Buttons
	{
		//Small jump left button
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "smallJumpLButton.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

			smallJumpLButton.entity = entity;

			buttonVec.push_back(&smallJumpLButton);

		}

		//Small jump right button
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "smallJumpRButton.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

			smallJumpRButton.entity = entity;

			buttonVec.push_back(&smallJumpRButton);

		}

		//Light attack button
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "lightAttackButton.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

			lightAttackButton.entity = entity;

			buttonVec.push_back(&lightAttackButton);

			
		}
		//Big jump left button
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "bigJumpLButton.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

			bigJumpLButton.entity = entity;

			buttonVec.push_back(&bigJumpLButton);

		}

		//Big jump right button
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "bigJumpRButton.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

			bigJumpRButton.entity = entity;

			buttonVec.push_back(&bigJumpRButton);

		}

		//Heavy attack button
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "heavyAttackButton.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

			heavyAttackButton.entity = entity;

			buttonVec.push_back(&heavyAttackButton);

		}

		//Interact button
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "interactButton.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

			interactButton.entity = entity;

			buttonVec.push_back(&interactButton);
		}
	}
	
	//Setup background
	{
		auto entity = ECS::CreateEntity();
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "BarBreakerBackground.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 400, 100);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, -20.f));
	}

	//Right boundary entity 
	{
		//Creates entity 
		auto entity = ECS::CreateEntity();
		boundaryRight = entity;

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components 
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 40);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(290.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, ENEMY | OBJECTS | PICKUP | TRIGGER, 1000.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}

	//Left boundary entity 
	{
		//Creates entity 
		auto entity = ECS::CreateEntity();
		boundaryLeft = entity;

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components 
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 40);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-290.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, ENEMY | OBJECTS | PICKUP | TRIGGER, 1000.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}

	//Setup Player 1
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		player1 = entity;
		activePlayer = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Health>(entity);//health component
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<EntityNumber>(entity);//stores entity number for easy access anywhere
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		auto animations = File::LoadJSON("linkAnimations.json");
		std::string fileName = "spritesheets/Link.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		animController.AddAnimation(animations["IdleRight"]);
		animController.AddAnimation(animations["AttackRight"]);
		animController.SetActiveAnim(0);
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-70.f, 50.f, 2.f));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;
		ECS::GetComponent<CanJump>(entity).m_canJump = false;
		ECS::GetComponent<Health>(entity).qPosition = -7;//qPosition is basically where the entity SHOULD be, in game units

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
	//player 2
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		player2 = entity;
		inactivePlayer = entity;
		
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Health>(entity);
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<EntityNumber>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		auto animations = File::LoadJSON("linkAnimations.json");
		std::string fileName = "spritesheets/Link.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		animController.AddAnimation(animations["IdleLeft"]);
		animController.AddAnimation(animations["AttackLeft"]);
		animController.SetActiveAnim(0);
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(70.f, 50.f, 2.f));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;
		ECS::GetComponent<CanJump>(entity).m_canJump = false;
		ECS::GetComponent<Health>(entity).qPosition = 7;//qPosition is basically where the entity SHOULD be, in game units

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
	if (ECS::GetComponent<AnimationController>(player1).GetAnimation(ECS::GetComponent<AnimationController>(player1).GetActiveAnim()).GetAnimationDone()) //check if animation is finished
	{
		ECS::GetComponent<AnimationController>(player1).GetAnimation(ECS::GetComponent<AnimationController>(player1).GetActiveAnim()).Reset(); // reset attack animation
		ECS::GetComponent<AnimationController>(player1).SetActiveAnim(0); //set animation to idle
	}

	if (ECS::GetComponent<AnimationController>(player2).GetAnimation(ECS::GetComponent<AnimationController>(player2).GetActiveAnim()).GetAnimationDone()) //check if animation is finished
	{
		ECS::GetComponent<AnimationController>(player2).GetAnimation(ECS::GetComponent<AnimationController>(player2).GetActiveAnim()).Reset(); // reset attack animation
		ECS::GetComponent<AnimationController>(player2).SetActiveAnim(0); //set animation to idle
	}

	BarBreaker::UpdateCamera();

	BarBreaker::AdjustScrollOffset();

	playerDistance = (ECS::GetComponent<PhysicsBody>(activePlayer).GetPosition().x - ECS::GetComponent<PhysicsBody>(inactivePlayer).GetPosition().x);

	boundaryDistanceRightActive = (ECS::GetComponent<PhysicsBody>(boundaryRight).GetPosition().x - ECS::GetComponent<PhysicsBody>(activePlayer).GetPosition().x);
	boundaryDistanceLeftActive = ECS::GetComponent<PhysicsBody>(activePlayer).GetPosition().x - (ECS::GetComponent<PhysicsBody>(boundaryLeft).GetPosition().x);

	boundaryDistanceRightInactive = (ECS::GetComponent<PhysicsBody>(boundaryRight).GetPosition().x - ECS::GetComponent<PhysicsBody>(inactivePlayer).GetPosition().x);
	boundaryDistanceLeftInactive = ECS::GetComponent<PhysicsBody>(inactivePlayer).GetPosition().x - (ECS::GetComponent<PhysicsBody>(boundaryLeft).GetPosition().x);

	if (!backgroundMusic.IsPlaying())
	{
		backgroundMusic.Play();
		backgroundMusic.SetVolume(0.1);
	}

	if (lightMoves + heavyMoves == 2)
	{
		turnEnd = true;
	}

	//If the player ended their turn, and the system hasnt begun counting yet
	if (turnEnd && !counting)
	{
		counting = true;
		beginClk = time(0);
	}

	//Once the system has begun counting, end the turn only after two seconds have passed
	else if (turnEnd && (time(0) - beginClk >= 2))
	{
		if (boundaryDistanceRightActive <= 30)
		{
			ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-60000.f, 80000.f), true);
			beginClk = time(0);
		}

		else if (boundaryDistanceLeftActive <= 30)
		{
			ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(60000.f, 80000.f), true);
			beginClk = time(0);
		}

		else EndTurn();
	}

	//Find the camera x and y in order to set the buttons' x and y values relative to it
	auto newX = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPositionX();
	auto newY = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPositionY();

	//Set each button's location relative to the camera
	ECS::GetComponent<Transform>(smallJumpLButton.entity).SetPosition(vec3(newX - 50, newY - 70, 10.f));
	ECS::GetComponent<Transform>(smallJumpRButton.entity).SetPosition(vec3(newX + 50, newY - 70, 10.f));
	ECS::GetComponent<Transform>(lightAttackButton.entity).SetPosition(vec3(newX - 15, newY - 80, 10.f));
	ECS::GetComponent<Transform>(bigJumpLButton.entity).SetPosition(vec3(newX - 80, newY - 70, 10.f));
	ECS::GetComponent<Transform>(bigJumpRButton.entity).SetPosition(vec3(newX + 80, newY - 70, 10.f));
	ECS::GetComponent<Transform>(heavyAttackButton.entity).SetPosition(vec3(newX + 15, newY - 80, 10.f));
	ECS::GetComponent<Transform>(interactButton.entity).SetPosition(vec3(newX + 0, newY - 60, 10.f));

	//Iterate through each button, setting each of its new min and max locations
	for (int i = 0; i < buttonVecLen; i++)
	{
		int buttonX = ECS::GetComponent<Transform>(buttonVec[i]->entity).GetPosition().x;
		int buttonY = ECS::GetComponent<Transform>(buttonVec[i]->entity).GetPosition().y;
		int buttonWidth = ECS::GetComponent<Sprite>(buttonVec[i]->entity).GetWidth();
		int buttonHeight = ECS::GetComponent<Sprite>(buttonVec[i]->entity).GetHeight();

		buttonVec[i]->max = vec2(buttonX + buttonWidth / 2, buttonY + buttonHeight / 2);
		buttonVec[i]->min = vec2(buttonX - buttonWidth / 2, buttonY - buttonHeight / 2);
	}

}

void BarBreaker::AdjustScrollOffset()
{
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(activePlayer));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(activePlayer));
}

void BarBreaker::KeyboardHold()
{
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
		LightAttack();
	}
	if (Input::GetKeyDown(Key::F))
	{
		BarBreaker::ThrowBottle();
	}
	if (Input::GetKeyDown(Key::L))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
}

void BarBreaker::KeyboardUp()
{
}

void BarBreaker::SmallMoveRight()
{
	if (!counting && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true)
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(65000.f, 60000.f), true);
		ECS::GetComponent<CanJump>(activePlayer).m_canJump = false;
		ECS::GetComponent<Health>(activePlayer).qPosition += 5;
		lightMoves++;
	}
}

void BarBreaker::SmallMoveLeft()
{
	if (!counting && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true)
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-65000.f, 60000.f), true);
		ECS::GetComponent<CanJump>(activePlayer).m_canJump = false;
		ECS::GetComponent<Health>(activePlayer).qPosition -= 5;
		lightMoves++;
	}
}

void BarBreaker::BigMoveRight()
{
	if (heavyMoves < 1 && !counting && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true)
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(90000.f, 100000.f), true);
		ECS::GetComponent<CanJump>(activePlayer).m_canJump = false;
		ECS::GetComponent<Health>(activePlayer).qPosition += 10;
		heavyMoves++;
	}
}

void BarBreaker::BigMoveLeft()
{
	if (heavyMoves < 1 && !counting && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true)
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-90000.f, 100000.f), true);
		ECS::GetComponent<CanJump>(activePlayer).m_canJump = false;
		ECS::GetComponent<Health>(activePlayer).qPosition -= 10;
		heavyMoves++;
	}
}

void BarBreaker::LightAttack()
{
	if (!counting)
	{
		ToneFire::CoreSound testSound{ "punch.wav" };

		if (abs(playerDistance) <= 60)
		{
			ECS::GetComponent<AnimationController>(activePlayer).SetActiveAnim(1);
			testSound.Play();
			testSound.SetVolume(0.5);
			if (playerDistance < 0)
			{
				ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(60000.f, 80000.f), true);
				ECS::GetComponent<Health>(inactivePlayer).qPosition += 5;
			}
			else if (playerDistance > 0)
			{
				ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-60000.f, 80000.f), true);
				ECS::GetComponent<Health>(inactivePlayer).qPosition -= 5;
			}
			ECS::GetComponent<Health>(inactivePlayer).reduceHealth(10);
			lightMoves++;
		}
	}
}

void BarBreaker::HeavyAttack()
{
	if (!counting)
	{
		ToneFire::CoreSound testSound{ "punch.wav" };

		if (abs(playerDistance) <= 60)
		{
			testSound.Play();
			testSound.SetVolume(0.5);
			if (playerDistance < 0)
			{
				ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(100000.f, 100000.f), true);
				ECS::GetComponent<Health>(inactivePlayer).qPosition += 20;
			}
			else if (playerDistance > 0)
			{
				ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-100000.f, 100000.f), true);
				ECS::GetComponent<Health>(inactivePlayer).qPosition -= 20;
			}
			ECS::GetComponent<Health>(inactivePlayer).reduceHealth(15);
			heavyMoves++;
		}
	}
}

void BarBreaker::ThrowBottle()
{
	if (!counting)
	{
		auto entity = ECS::CreateEntity();
		vec3 playerPos = ECS::GetComponent<Transform>(activePlayer).GetPosition();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<EntityNumber>(entity);

		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 15, 15);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(playerPos.x + 32, playerPos.y + 32.f, playerPos.z));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(playerPos.x), float32(playerPos.y + 30));
		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, PICKUP, PLAYER | ENEMY | OBJECTS | PICKUP | TRIGGER, 1000.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(false);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(0.8);//slightly reduced effect of gravity

		if (playerDistance < 0)
		{
			tempBody->ApplyLinearImpulseToCenter(b2Vec2(10000, 19000), true);
		}
		else if (playerDistance > 0)
		{
			tempBody->ApplyLinearImpulseToCenter(b2Vec2(-10000, 19000), true);
		}
		lightMoves++;
	}
}

void BarBreaker::EndTurn()
{
	lightMoves = 0;
	heavyMoves = 0;
	turnEnd = false;
	counting = false;

	if (boundaryDistanceRightInactive <= 30)
	{
		ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-60000.f, 80000.f), true);
	}

	else if (boundaryDistanceLeftInactive <= 30)
	{
		ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(60000.f, 80000.f), true);
	}
	
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

void BarBreaker::MouseMotion(SDL_MouseMotionEvent evnt)
{
	
}

void BarBreaker::MouseClick(SDL_MouseButtonEvent evnt)
{
	//Convert the cursor location to GL
	vec2 cursorRelative(Util::ConvertToGL(m_sceneReg, vec2(evnt.x, evnt.y)));

	/*std::cout << "Button min X: " << smallJumpLButton.min.x << std::endl << "Button min Y: " << smallJumpLButton.min.y << std::endl;
	std::cout << "Button max X: " << smallJumpLButton.max.x << std::endl << "Button max Y: " << smallJumpLButton.max.y << std::endl;
	std::cout << "CursorX: " << cursorRelative.x << std::endl << "CursorY: " << cursorRelative.y << std::endl;*/

	/*
		***Functions to find if the player has clicked a button***
	*/
	if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= smallJumpLButton.min.x && cursorRelative.y >= smallJumpLButton.min.y) &&
		(cursorRelative.x <= smallJumpLButton.max.x && cursorRelative.y <= smallJumpLButton.max.y))
	{
		SmallMoveLeft();
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= bigJumpLButton.min.x && cursorRelative.y >= bigJumpLButton.min.y) &&
		(cursorRelative.x <= bigJumpLButton.max.x && cursorRelative.y <= bigJumpLButton.max.y))
	{
		BigMoveLeft();
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= smallJumpRButton.min.x && cursorRelative.y >= smallJumpRButton.min.y) &&
		(cursorRelative.x <= smallJumpRButton.max.x && cursorRelative.y <= smallJumpRButton.max.y))
	{
		SmallMoveRight();
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= bigJumpRButton.min.x && cursorRelative.y >= bigJumpRButton.min.y) &&
		(cursorRelative.x <= bigJumpRButton.max.x && cursorRelative.y <= bigJumpRButton.max.y))
	{
		BigMoveRight();
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= lightAttackButton.min.x && cursorRelative.y >= lightAttackButton.min.y) &&
		(cursorRelative.x <= lightAttackButton.max.x && cursorRelative.y <= lightAttackButton.max.y))
	{
		LightAttack();
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= heavyAttackButton.min.x && cursorRelative.y >= heavyAttackButton.min.y) &&
		(cursorRelative.x <= heavyAttackButton.max.x && cursorRelative.y <= heavyAttackButton.max.y))
	{
		HeavyAttack();
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= interactButton.min.x && cursorRelative.y >= interactButton.min.y) &&
		(cursorRelative.x <= interactButton.max.x && cursorRelative.y <= interactButton.max.y))
	{
		ThrowBottle();
	}
}



/*void BarBreaker::GUI()
{
	GUIWindowUI();

	if (m_firstWindow)
	{
		GUIWindowOne();
	}

	if (m_secondWindow)
	{
		GUIWindowTwo();
	}

	GUIWindowOne();
	GUIWindowTwo();
}*/

/*void BarBreaker::GUIWindowUI()
{
	ImGui::Begin("Moveset", 0, 63); 

	ImGui::SetWindowFontScale(2.5);

	//is the buffer initialized
	static bool init = false;

	//Buffers
	const unsigned int BUF_LEN = 512;
	static char buf[BUF_LEN];

	if (!init)
	{
		memset(buf, 0, BUF_LEN);
		init = true;
	}
	m_physicsWorld->SetAllowSleeping(true);

	
	//ImGui::Checkbox("Enable First Window", &m_firstWindow);
	ImGui::SameLine(ImGui::GetWindowWidth() / 16);
	if (ImGui::Button("Short jump left", ImVec2(300.f, 45.f)))
	{
		SmallMoveLeft();
	}

	ImGui::SameLine(ImGui::GetWindowWidth() / 2.5);
	if (ImGui::Button("Short jump right", ImVec2(300.f, 45.f)))
	{
		SmallMoveRight();
	}


	ImGui::SameLine(ImGui::GetWindowWidth() / 1.3);
	if (ImGui::Button("Attack", ImVec2(300.f, 45.f)))
	{
		LightAttack();
	}

	ImGui::NewLine();
	ImGui::SameLine(ImGui::GetWindowWidth() / 16);
	if (ImGui::Button("Long jump left", ImVec2(300.f, 45.f)))
	{
		BigMoveLeft();
	}

	ImGui::SameLine(ImGui::GetWindowWidth() / 2.5);
	if (ImGui::Button("Long jump right", ImVec2(300.f, 45.f)))
	{
		BigMoveRight();
	}

	ImGui::SameLine(ImGui::GetWindowWidth() / 1.3);
	if (ImGui::Button("Grab Item", ImVec2(300.f, 45.f)))
	{
	}

	ImGui::End();

	ImGui::Begin("Healthbar");

	static std::string imageLoad = "HealthTest.png";

	ImGui::Image((void*)(intptr_t)TextureManager::FindTexture(imageLoad)->GetID(), ImVec2(150.f, 150.f), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
	
}*/