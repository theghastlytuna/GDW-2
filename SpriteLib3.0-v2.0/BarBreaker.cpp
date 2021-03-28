#include "BarBreaker.h"
#include "Utilities.h"
#include "Timer.h"
#include "Tone Fire/Tonefire.h"

BarBreaker::BarBreaker(std::string name)
	:Scene(name)
{
	//no gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -15.f);
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

	Scene::CreateTransparentPlatform("boxSprite.jpg", 600.f, 20, 0, -60.f, 0.f, 0.f, 0.f);

	backgroundMusic.Play();
	backgroundMusic.SetVolume(0.1);

	//ui
	{
		//Help text
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "helpText.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 110);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

			helpTextImage = entity;
		}

		//Health bar entities
		{
			//P1 health bar
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "healthBar.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 10);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

				p1HealthBar = entity;
			}

			//P2 health bar
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "healthBar.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 10);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

				p2HealthBar = entity;
			}

			//P1 health bar outline
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "healthBacking.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 70, 20);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 9.f));

				p1HealthBarOutline = entity;
			}

			//P2 health bar outline
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "healthBacking.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 70, 20);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 9.f));

				p2HealthBarOutline = entity;
			}
		}

		//Button entities
		{
			//Small jump left button
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<AnimationController>(entity);

				auto animations = File::LoadJSON("buttonAnims.json");
				std::string fileName = "spritesheets/buttonAnimSheet.png";
				auto& animController = ECS::GetComponent<AnimationController>(entity);
				animController.InitUVs(fileName);
				animController.AddAnimation(animations["smallJumpLeftHover"]);
				animController.AddAnimation(animations["smallJumpLeftDefault"]);
				animController.AddAnimation(animations["smallJumpLeftClick"]);
				animController.SetActiveAnim(1);
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15, true, &animController);
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
				ECS::AttachComponent<AnimationController>(entity);

				auto animations = File::LoadJSON("buttonAnims.json");
				std::string fileName = "spritesheets/buttonAnimSheet.png";
				auto& animController = ECS::GetComponent<AnimationController>(entity);
				animController.InitUVs(fileName);
				animController.AddAnimation(animations["smallJumpRightHover"]);
				animController.AddAnimation(animations["smallJumpRightDefault"]);
				animController.AddAnimation(animations["smallJumpRightClick"]);
				animController.SetActiveAnim(1);
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15, true, &animController);
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
				ECS::AttachComponent<AnimationController>(entity);

				auto animations = File::LoadJSON("buttonAnims.json");
				std::string fileName = "spritesheets/buttonAnimSheet.png";
				auto& animController = ECS::GetComponent<AnimationController>(entity);
				animController.InitUVs(fileName);
				animController.AddAnimation(animations["lightAttackHover"]);
				animController.AddAnimation(animations["lightAttackDefault"]);
				animController.AddAnimation(animations["lightAttackClick"]);
				animController.SetActiveAnim(1);
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15, true, &animController);
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
				ECS::AttachComponent<AnimationController>(entity);

				auto animations = File::LoadJSON("buttonAnims.json");
				std::string fileName = "spritesheets/buttonAnimSheet.png";
				auto& animController = ECS::GetComponent<AnimationController>(entity);
				animController.InitUVs(fileName);
				animController.AddAnimation(animations["largeJumpLeftHover"]);
				animController.AddAnimation(animations["largeJumpLeftDefault"]);
				animController.AddAnimation(animations["largeJumpLeftClick"]);
				animController.SetActiveAnim(1);
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15, true, &animController);
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
				ECS::AttachComponent<AnimationController>(entity);

				auto animations = File::LoadJSON("buttonAnims.json");
				std::string fileName = "spritesheets/buttonAnimSheet.png";
				auto& animController = ECS::GetComponent<AnimationController>(entity);
				animController.InitUVs(fileName);
				animController.AddAnimation(animations["largeJumpRightHover"]);
				animController.AddAnimation(animations["largeJumpRightDefault"]);
				animController.AddAnimation(animations["largeJumpRightClick"]);
				animController.SetActiveAnim(1);
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15, true, &animController);
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
				ECS::AttachComponent<AnimationController>(entity);

				auto animations = File::LoadJSON("buttonAnims.json");
				std::string fileName = "spritesheets/buttonAnimSheet.png";
				auto& animController = ECS::GetComponent<AnimationController>(entity);
				animController.InitUVs(fileName);
				animController.AddAnimation(animations["heavyAttackHover"]);
				animController.AddAnimation(animations["heavyAttackDefault"]);
				animController.AddAnimation(animations["heavyAttackClick"]);
				animController.SetActiveAnim(1);
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15, true, &animController);
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
				ECS::AttachComponent<AnimationController>(entity);

				auto animations = File::LoadJSON("buttonAnims.json");
				std::string fileName = "spritesheets/buttonAnimSheet.png";
				auto& animController = ECS::GetComponent<AnimationController>(entity);
				animController.InitUVs(fileName);
				animController.AddAnimation(animations["interactHover"]);
				animController.AddAnimation(animations["interactDefault"]);
				animController.AddAnimation(animations["interactClick"]);
				animController.SetActiveAnim(1);
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15, true, &animController);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

				interactButton.entity = entity;

				buttonVec.push_back(&interactButton);
			}

			//Help button
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<AnimationController>(entity);

				auto animations = File::LoadJSON("buttonAnims.json");
				std::string fileName = "spritesheets/buttonAnimSheet.png";
				auto& animController = ECS::GetComponent<AnimationController>(entity);
				animController.InitUVs(fileName);
				animController.AddAnimation(animations["helpHover"]);
				animController.AddAnimation(animations["helpDefault"]);
				animController.AddAnimation(animations["helpClick"]);
				animController.SetActiveAnim(1);
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 15, true, &animController);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

				helpButton.entity = entity;

				buttonVec.push_back(&helpButton);
			}
		}

		//Button textboxes
		{
			//Small jump left text
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "smallJumpLText.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 20);
				ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

				smallJumpLButton.textImage = entity;
			}

			//Small jump right text
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "smallJumpRText.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 20);
				ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

				smallJumpRButton.textImage = entity;
			}

			//Light attack text
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "lightAttackText.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 22);
				ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

				lightAttackButton.textImage = entity;
			}

			//Big jump left text
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "bigJumpLText.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 20);
				ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

				bigJumpLButton.textImage = entity;
			}

			//Big jump right text
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "smallJumpLtext.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 20);
				ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

				bigJumpRButton.textImage = entity;
			}

			//Heavy attack text
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "heavyAttackText.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 22);
				ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

				heavyAttackButton.textImage = entity;
			}

			//Interact text
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "interactText.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 45);
				ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

				interactButton.textImage = entity;
			}

			//Help hover text
			{
				auto entity = ECS::CreateEntity();

				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "helpHelpText.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 8);
				ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

				helpButton.textImage = entity;
			}
		}
	}

	//background
	{
		auto entity = ECS::CreateEntity();
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "BarBreakerBackground4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 800, 250);
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
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components 
		auto animations = File::LoadJSON("stageHazardRight.json");
		std::string fileName = "spritesheets/stageHazardRight.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		animController.AddAnimation(animations["RightIdle"]);
		animController.AddAnimation(animations["RightPush"]);
		animController.SetActiveAnim(0);
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 60, true, &animController);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(300.f), float32(-20.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, BOUNDARY, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.f, 3.f);

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
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components 
		auto animations = File::LoadJSON("stageHazardLeft.json");
		std::string fileName = "spritesheets/stageHazardLeft.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		animController.AddAnimation(animations["LeftIdle"]);
		animController.AddAnimation(animations["LeftPush"]);
		animController.SetActiveAnim(0);
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 70, true, &animController);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-300.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, BOUNDARY, ENEMY | OBJECTS | PICKUP | TRIGGER | BOUNDARY, 0.f, 3.f);

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
		auto animations = File::LoadJSON("barBreakerAnims.json");
		std::string fileName = "spritesheets/playerAnimations.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		animController.AddAnimation(animations["fpIdle"]);//idle
		animController.AddAnimation(animations["fpJump"]);//jump
		animController.AddAnimation(animations["fpAttack"]);//attack
		animController.AddAnimation(animations["fpDamage"]); //damage
		animController.AddAnimation(animations["fpKick"]); //kick
		animController.AddAnimation(animations["fpThrow"]); //throw
		animController.SetActiveAnim(0);
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 90, 40, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-50.f, -15.f, 2.f));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;
		ECS::GetComponent<CanJump>(entity).m_canJump = false;
		ECS::GetComponent<Health>(entity).qPosition = -5;//qPosition is basically where the entity SHOULD be, in game units
		ECS::GetComponent<Health>(entity).playerNum = 1;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 60.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-50.f), float32(-15.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | PICKUP | TRIGGER | BOUNDARY, 1000.f, 3.f);

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
		auto animations = File::LoadJSON("barBreakerAnims.json");
		std::string fileName = "spritesheets/playerAnimations.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		animController.AddAnimation(animations["spIdle"]);//idle
		animController.AddAnimation(animations["spJump"]);//jump
		animController.AddAnimation(animations["spAttack"]);//attack
		animController.AddAnimation(animations["spDamage"]); //damage
		animController.AddAnimation(animations["spKick"]); //kick
		animController.AddAnimation(animations["spThrow"]); //throw
		animController.SetActiveAnim(0);
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 90, 40, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, -15.f, 2.f));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;
		ECS::GetComponent<CanJump>(entity).m_canJump = false;
		ECS::GetComponent<Health>(entity).qPosition = 5;//qPosition is basically where the entity SHOULD be, in game units
		ECS::GetComponent<Health>(entity).playerNum = 2;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 60.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(50.f), float32(-15.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | PICKUP | TRIGGER | BOUNDARY, 1000.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}

	//interactables
	{
		BarBreaker::BottleInteractable(-50.f, 10.f);
		BarBreaker::BottleInteractable(-15.f, 10.f);
		BarBreaker::BottleInteractable(-200.f, 10.f);
		BarBreaker::BottleInteractable(-230.f, 10.f);
		BarBreaker::BottleInteractable(15.f, 10.f);
		BarBreaker::BottleInteractable(50.f, 10.f);
		BarBreaker::BottleInteractable(200.f, 10.f);
		BarBreaker::BottleInteractable(230.f, 10.f);

		BarBreaker::ChairInteractable(-190.f, -20.f);
		BarBreaker::ChairInteractable(-220.f, -20.f);
		BarBreaker::ChairInteractable(190.f, -20.f);
		BarBreaker::ChairInteractable(220.f, -20.f);

	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(activePlayer));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(activePlayer));
	moveCam = false;
}


void BarBreaker::Update()
{
	BarBreaker::AnimationUpdate();

	BarBreaker::UpdateCamera();

	BarBreaker::AdjustScrollOffset();

	playerDistance = (ECS::GetComponent<PhysicsBody>(activePlayer).GetPosition().x - ECS::GetComponent<PhysicsBody>(inactivePlayer).GetPosition().x);

	BarBreaker::UIUpdate();

	BarBreaker::BoundaryUpdate();

	//If the music has stopped playing, loop it
	if (!backgroundMusic.IsPlaying())
	{
		backgroundMusic.Play();
		backgroundMusic.SetVolume(0.1);
	}

	if (endGame == false)
	{
		BarBreaker::CheckGame();
	}

	//If an item has been thrown in the last 2 seconds, set throwing to true
	// (this causes both players to be unable to move or attack)
	if ((time(0) - beginClk) < 2.5)
	{
		throwing = true;	
	}

	//Else, the throwing has finished so set throwing to false
	else
	{
		throwing = false;
	}

	//End the scene if a player's health reaches 0
	if (ECS::GetComponent<Health>(player1).GetHealth() <= 0)
	{
		winner = 2;
		finished = true;
	}

	else if (ECS::GetComponent<Health>(player2).GetHealth() <= 0)
	{
		winner = 1;
		finished = true;
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
	//Debug keys
	if (Input::GetKeyDown(Key::M))
	{
		ECS::GetComponent<Health>(player1).reduceHealth(10);
		//std::cout << "P1 Health: " << ECS::GetComponent<Health>(player1).GetHealth() << std::endl;
	}

	if (Input::GetKeyDown(Key::N))
	{
		ECS::GetComponent<Health>(player2).reduceHealth(10);
		//std::cout << "P2 Health: " << ECS::GetComponent<Health>(player2).GetHealth() << std::endl;
	}
	
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
		BarBreaker::ThrowChair();
	}

	if (Input::GetKeyDown(Key::L))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
}

void BarBreaker::KeyboardUp()
{
}

void BarBreaker::UpdateCamera() {
	/*lerps camera to active player rather than jumping from player to player*/

	auto& cameraOffset = ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera());
	static float offsetVal = 0;
	if (moveCam) {
		if (activePlayer == player1) {
			if (offsetVal < 79 /*&& offsetVal < 81*/) moveCam = false;
			else offsetVal -= 0.05 * Timer::time;
		}
		if (activePlayer == player2) {
			if (offsetVal > -49 /*&& offsetVal > -51*/) moveCam = false;
			else offsetVal += 0.05 * Timer::time;
		}
		cameraOffset.SetOffset(offsetVal);
	}
	else {
		//reset offsetting values
		if (activePlayer == player1) {
			if (ECS::GetComponent<Transform>(player2).GetPositionX() - ECS::GetComponent<Transform>(player1).GetPositionX() > 210)
				offsetVal = -200;
			else offsetVal = -100;
		}
		if (activePlayer == player2) offsetVal = 200;
	}

}

void BarBreaker::AnimationUpdate()
{
	//if(ECS::GetComponent<CanJump>(player1).m_canJump && ECS::GetComponent<AnimationController>(player1).GetActiveAnim() == 3)
	//	ECS::GetComponent<AnimationController>(player1).SetActiveAnim(0); //set animation to idle

	//if (ECS::GetComponent<CanJump>(player2).m_canJump && ECS::GetComponent<AnimationController>(player2).GetActiveAnim() == 3)
	//	ECS::GetComponent<AnimationController>(player2).SetActiveAnim(0); //set animation to idle

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

	if (ECS::GetComponent<AnimationController>(boundaryLeft).GetAnimation(ECS::GetComponent<AnimationController>(boundaryLeft).GetActiveAnim()).GetAnimationDone()) //check if animation is finished
	{
		ECS::GetComponent<AnimationController>(boundaryLeft).GetAnimation(ECS::GetComponent<AnimationController>(boundaryLeft).GetActiveAnim()).Reset(); // reset attack animation
		ECS::GetComponent<AnimationController>(boundaryLeft).SetActiveAnim(0); //set animation to idle
	}

	if (ECS::GetComponent<AnimationController>(boundaryRight).GetAnimation(ECS::GetComponent<AnimationController>(boundaryRight).GetActiveAnim()).GetAnimationDone()) //check if animation is finished
	{
		ECS::GetComponent<AnimationController>(boundaryRight).GetAnimation(ECS::GetComponent<AnimationController>(boundaryRight).GetActiveAnim()).Reset(); // reset attack animation
		ECS::GetComponent<AnimationController>(boundaryRight).SetActiveAnim(0); //set animation to idle
	}

}

void BarBreaker::UIUpdate()
{
	//Find the camera x and y in order to set the buttons' x and y values relative to it
	auto newX = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPositionX();
	auto newY = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPositionY();

	//Use Lerp to reduce the width of the health bars based on player health
	ECS::GetComponent<Sprite>(p1HealthBar).SetWidth(Util::Lerp(1, 60, ECS::GetComponent<Health>(player1).GetHealth() / 100));
	ECS::GetComponent<Sprite>(p2HealthBar).SetWidth(Util::Lerp(1, 60, ECS::GetComponent<Health>(player2).GetHealth() / 100));

	//Get the currenth health bar widths
	auto p1HealthWidth = ECS::GetComponent<Sprite>(p1HealthBar).GetWidth();
	auto p2HealthWidth = ECS::GetComponent<Sprite>(p2HealthBar).GetWidth();

	//Set the health bar outline positions relative to the camera
	ECS::GetComponent<Transform>(p1HealthBarOutline).SetPosition(vec3(newX - 125, newY + 70, 9.f));
	ECS::GetComponent<Transform>(p2HealthBarOutline).SetPosition(vec3(newX + 125, newY + 70, 9.f));

	//Set the health bar positions relative to the camera (and adjust for the changing size)
	ECS::GetComponent<Transform>(p1HealthBar).SetPosition(vec3(newX - 125 - ((60 - p1HealthWidth) / 2), newY + 70, 10.f));
	ECS::GetComponent<Transform>(p2HealthBar).SetPosition(vec3(newX + 125 + ((60 - p2HealthWidth) / 2), newY + 70, 10.f));

	ECS::GetComponent<Transform>(helpTextImage).SetPosition(vec3(newX, newY + 3, 100.f));

	//Set each button's location relative to the camera
	ECS::GetComponent<Transform>(smallJumpLButton.entity).SetPosition(vec3(newX - 50, newY - 70, 10.f));
	ECS::GetComponent<Transform>(smallJumpRButton.entity).SetPosition(vec3(newX + 50, newY - 70, 10.f));
	ECS::GetComponent<Transform>(lightAttackButton.entity).SetPosition(vec3(newX - 15, newY - 80, 10.f));
	ECS::GetComponent<Transform>(bigJumpLButton.entity).SetPosition(vec3(newX - 80, newY - 70, 10.f));
	ECS::GetComponent<Transform>(bigJumpRButton.entity).SetPosition(vec3(newX + 80, newY - 70, 10.f));
	ECS::GetComponent<Transform>(heavyAttackButton.entity).SetPosition(vec3(newX + 15, newY - 80, 10.f));
	ECS::GetComponent<Transform>(interactButton.entity).SetPosition(vec3(newX + 0, newY - 60, 10.f));
	ECS::GetComponent<Transform>(helpButton.entity).SetPosition(vec3(newX + 145, newY - 85, 10.f));

	//ECS::GetComponent<Transform>(smallJumpLButton.textImage).SetPosition(vec3(newX - 50, newY - 55, 10.f));

	//Iterate through each button
	for (int i = 0; i < buttonVecLen; i++)
	{
		//Set each of new min and max location
		int buttonX = ECS::GetComponent<Transform>(buttonVec[i]->entity).GetPosition().x;
		int buttonY = ECS::GetComponent<Transform>(buttonVec[i]->entity).GetPosition().y;
		int buttonWidth = ECS::GetComponent<Sprite>(buttonVec[i]->entity).GetWidth();
		int buttonHeight = ECS::GetComponent<Sprite>(buttonVec[i]->entity).GetHeight();

		buttonVec[i]->max = vec2(buttonX + buttonWidth / 2, buttonY + buttonHeight / 2);
		buttonVec[i]->min = vec2(buttonX - buttonWidth / 2, buttonY - buttonHeight / 2);

		//Set each text location
		ECS::GetComponent<Transform>(buttonVec[i]->textImage).SetPosition(vec3(buttonX, buttonY + 30, 100.f));

		//If the button has been hovered over for long enough, show text
		if (buttonVec[i]->hovering && (time(0) - buttonVec[i]->startHoverTime) > 1)
		{
			ECS::GetComponent<Sprite>(buttonVec[i]->textImage).SetTransparency(1.f);
		}

		//Else don't show text
		else
		{
			ECS::GetComponent<Sprite>(buttonVec[i]->textImage).SetTransparency(0.f);
		}
	}
}

void BarBreaker::BoundaryUpdate()
{
	boundaryDistanceRightActive = (ECS::GetComponent<PhysicsBody>(boundaryRight).GetPosition().x - ECS::GetComponent<PhysicsBody>(activePlayer).GetPosition().x);
	boundaryDistanceLeftActive = ECS::GetComponent<PhysicsBody>(activePlayer).GetPosition().x - (ECS::GetComponent<PhysicsBody>(boundaryLeft).GetPosition().x);

	boundaryDistanceRightInactive = (ECS::GetComponent<PhysicsBody>(boundaryRight).GetPosition().x - ECS::GetComponent<PhysicsBody>(inactivePlayer).GetPosition().x);
	boundaryDistanceLeftInactive = ECS::GetComponent<PhysicsBody>(inactivePlayer).GetPosition().x - (ECS::GetComponent<PhysicsBody>(boundaryLeft).GetPosition().x);

	//If the turn end sequence has begun, and all players are not moving
	if (turnEnd && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true
		&& ECS::GetComponent<CanJump>(inactivePlayer).m_canJump == true
		&& ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->GetLinearVelocity().x == 0
		&& ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->GetLinearVelocity().x == 0)
	{
		//If the active player is in range of a boundary, push them away and set counting to true (so that they aren't hit again)
		if (boundaryDistanceRightActive <= 50 && !counting)
		{
			ECS::GetComponent<AnimationController>(boundaryRight).SetActiveAnim(1);
			ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-80000.f, 80000.f), true);
			ECS::GetComponent<Health>(activePlayer).qPosition -= 7;
			counting = true;
		}

		else if (boundaryDistanceLeftActive <= 50 && !counting)
		{
			ECS::GetComponent<AnimationController>(boundaryLeft).SetActiveAnim(1);
			ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(80000.f, 80000.f), true);
			ECS::GetComponent<Health>(activePlayer).qPosition += 7;
			counting = true;
		}

		//Else, since neither player is moving and the active player is out of moves, end the turn
		else EndTurn();
	}
}

void BarBreaker::SmallMoveRight()
{
	if (!turnEnd && !throwing && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true)
	{
		ECS::GetComponent<AnimationController>(activePlayer).SetActiveAnim(1);
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(80000.f, 60000.f), true);
		ECS::GetComponent<CanJump>(activePlayer).m_canJump = false;
		ECS::GetComponent<Health>(activePlayer).qPosition += 5;
		lightMoves++;
	}
}

void BarBreaker::SmallMoveLeft()
{
	if (!turnEnd && !throwing && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true)
	{
		ECS::GetComponent<AnimationController>(activePlayer).SetActiveAnim(1);
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-80000.f, 60000.f), true);
		ECS::GetComponent<CanJump>(activePlayer).m_canJump = false;
		ECS::GetComponent<Health>(activePlayer).qPosition -= 5;
		lightMoves++;
	}
}

void BarBreaker::BigMoveRight()
{
	if (heavyMoves < 1 && !turnEnd && !throwing && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true)
	{
		ECS::GetComponent<AnimationController>(activePlayer).SetActiveAnim(1);
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(100000.f, 100000.f), true);
		ECS::GetComponent<CanJump>(activePlayer).m_canJump = false;
		ECS::GetComponent<Health>(activePlayer).qPosition += 10;
		heavyMoves++;
	}
}

void BarBreaker::BigMoveLeft()
{
	if (heavyMoves < 1 && !turnEnd && !throwing && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true)
	{
		ECS::GetComponent<AnimationController>(activePlayer).SetActiveAnim(1);
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-100000.f, 100000.f), true);
		ECS::GetComponent<CanJump>(activePlayer).m_canJump = false;
		ECS::GetComponent<Health>(activePlayer).qPosition -= 10;
		heavyMoves++;
	}
}

void BarBreaker::LightAttack()
{
	//Only count the button press if the turn hasnt ended and both players aren't moving
	if (!turnEnd && !throwing
		&& ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->GetLinearVelocity().x == 0
		&& ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->GetLinearVelocity().x == 0)
	{
		ToneFire::CoreSound testSound{ "punch.wav" };

		if (abs(playerDistance) <= 70)
		{
			ECS::GetComponent<AnimationController>(activePlayer).SetActiveAnim(2);
			testSound.Play();
			testSound.SetVolume(0.5);

			ECS::GetComponent<Health>(inactivePlayer).reduceHealth(10);

			if (playerDistance < 0)
			{
				ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(65000.f, 80000.f), true);
				ECS::GetComponent<Health>(inactivePlayer).qPosition += 5;
			}
			else if (playerDistance > 0)
			{
				ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-65000.f, 80000.f), true);
				ECS::GetComponent<Health>(inactivePlayer).qPosition -= 5;
			}
			ECS::GetComponent<Health>(inactivePlayer).reduceHealth(10);
			ECS::GetComponent<AnimationController>(inactivePlayer).SetActiveAnim(3);
			ECS::GetComponent<CanJump>(inactivePlayer).m_canJump = false;
			lightMoves++;
		}
	}
}

void BarBreaker::HeavyAttack()
{
	//Only count the button press if the turn hasnt ended and both players aren't moving
	if (heavyMoves < 1 && !turnEnd && !throwing
		&& ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->GetLinearVelocity().x == 0
		&& ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->GetLinearVelocity().x == 0)
	{
		ToneFire::CoreSound testSound{ "punch.wav" };

		if (abs(playerDistance) <= 70)
		{
			ECS::GetComponent<AnimationController>(activePlayer).SetActiveAnim(4);
			testSound.Play();
			testSound.SetVolume(0.5);

			if (playerDistance < 0)
			{
				ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(100000.f, 100000.f), true);
				ECS::GetComponent<Health>(inactivePlayer).qPosition += 10;
			}

			else if (playerDistance > 0)
			{
				ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-100000.f, 100000.f), true);
				ECS::GetComponent<Health>(inactivePlayer).qPosition -= 10;
			}
			ECS::GetComponent<Health>(inactivePlayer).reduceHealth(15);
			ECS::GetComponent<AnimationController>(inactivePlayer).SetActiveAnim(3);
			ECS::GetComponent<CanJump>(inactivePlayer).m_canJump = false;
			heavyMoves++;
		}
	}
}

void BarBreaker::PickupBottle()
{
	//Only count the button press if the turn hasnt ended and both players aren't moving
	if (!turnEnd && !throwing && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true
		&& ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->GetLinearVelocity().x == 0
		&& ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->GetLinearVelocity().x == 0)
	{
		pos playerPos; //position of active player
		playerPos.x = ECS::GetComponent<Transform>(activePlayer).GetPositionX();
		playerPos.y = ECS::GetComponent<Transform>(activePlayer).GetPositionY();

		for (int i = 0; i < bottle.size(); i++) {
			//check if active player's x pos is within bottle range
			float bottlex = ECS::GetComponent<Transform>(bottle[i]).GetPositionX();
			float dist = bottlex - playerPos.x;

			if (abs(dist) <= 30) {
				//player is within range
				PhysicsBody::m_bodiesToDelete.push_back(bottle[i]);
				bottle.erase(bottle.begin() + i);

				std::cout << "\nPicked up bottle\n";

				ThrowBottle();
				return;
			}
		}

		for (int i = 0; i < chair.size(); i++) {
			//check if active player's x pos is within bottle range
			float chairx = ECS::GetComponent<Transform>(chair[i]).GetPositionX();
			float dist = chairx - playerPos.x;

			if (abs(dist) <= 30) {
				//player is within range
				PhysicsBody::m_bodiesToDelete.push_back(chair[i]);
				chair.erase(chair.begin() + i);

				std::cout << "\nPicked up chair\n";

				ThrowChair();
				return;
			}
		}
		std::cout << "\nNo bottles\n";
	}
}

void BarBreaker::ThrowBottle()
{
	//Only count the button press if the turn hasnt ended and both players aren't moving
	if (!turnEnd && !throwing && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true
		&& ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->GetLinearVelocity().x == 0
		&& ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->GetLinearVelocity().x == 0)
	{
		ECS::GetComponent<AnimationController>(activePlayer).SetActiveAnim(5);


		auto entity = ECS::CreateEntity();
		vec3 playerPos = ECS::GetComponent<Transform>(activePlayer).GetPosition();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<EntityNumber>(entity);

		std::string fileName = "Bottle.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 7, 15);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(playerPos.x + 32, playerPos.y + 32.f, playerPos.z + 5));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		if (playerDistance < 0)
		{
			tempDef.position.Set(float32(playerPos.x + 40), float32(playerPos.y + 10));
		}
		else if (playerDistance > 0)
		{
			tempDef.position.Set(float32(playerPos.x - 40), float32(playerPos.y + 10));
		}
		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, PICKUP, PLAYER | ENEMY | OBJECTS | PICKUP | TRIGGER, 1000.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(false);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.2);

		if (playerDistance < 0)
		{
			tempBody->ApplyLinearImpulseToCenter(b2Vec2(11000, 7000), true);
			tempBody->ApplyTorque(-1000000, true);
		}
		else if (playerDistance > 0)
		{
			tempBody->ApplyLinearImpulseToCenter(b2Vec2(-11000, 7000), true);
			tempBody->ApplyTorque(1000000, true);
		}


		lightMoves++;
		
		//Start the clock after throwing the object
		beginClk = time(0);
	}
}

void BarBreaker::ThrowChair()
{
	//Only count the button press if the turn hasnt ended and both players aren't moving
	if (!turnEnd && ECS::GetComponent<CanJump>(activePlayer).m_canJump == true
		&& ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->GetLinearVelocity().x == 0
		&& ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->GetLinearVelocity().x == 0)
	{
		ECS::GetComponent<AnimationController>(activePlayer).SetActiveAnim(5);

		auto entity = ECS::CreateEntity();
		vec3 playerPos = ECS::GetComponent<Transform>(activePlayer).GetPosition();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<EntityNumber>(entity);

		std::string fileName = "Chair.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 12, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(playerPos.x, playerPos.y, playerPos.z + 5));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		if (playerDistance < 0)
		{
			tempDef.position.Set(float32(playerPos.x + 40), float32(playerPos.y + 20));
		}
		else if (playerDistance > 0)
		{
			tempDef.position.Set(float32(playerPos.x - 40), float32(playerPos.y + 20));
		}
		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, ENEMY, PLAYER | ENEMY | OBJECTS | PICKUP | TRIGGER, 1000.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(false);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.4);

		if (playerDistance < 0)
		{
			tempBody->ApplyLinearImpulseToCenter(b2Vec2(30000, 30000), true);
			tempBody->ApplyTorque(-16000000, true);
		}
		else if (playerDistance > 0)
		{
			tempBody->ApplyLinearImpulseToCenter(b2Vec2(-35000, 30000), true);
			tempBody->ApplyTorque(16000000, true);
		}

		heavyMoves++;
	}
}

void BarBreaker::CheckGame()
{
	//If the player has reached the move limit, start the turn ending sequence
	if (lightMoves + heavyMoves >= 2)
	{
		turnEnd = true;
	}

	if (ECS::GetComponent<Health>(player1).GetHealth() <= 0 || ECS::GetComponent<Health>(player2).GetHealth() <= 0)
	{
		endGame = true;
		BarBreaker::EndGame();
	}
}

void BarBreaker::EndGame()
{
	if (ECS::GetComponent<Health>(player1).GetHealth() <= 0)
	{
		std::cout << "Player 2 wins" << std::endl;
	}
	else if (ECS::GetComponent<Health>(player2).GetHealth() <= 0)
	{
		std::cout << "Player 1 wins" << std::endl;
	}
}

void BarBreaker::EndTurn()
{
	lightMoves = 0;
	heavyMoves = 0;
	turnEnd = false;
	counting = false;

	//If the inactive player is within range of either boundary, push them away
	if (boundaryDistanceRightInactive <= 50)
	{
		ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-80000.f, 80000.f), true);
		ECS::GetComponent<Health>(inactivePlayer).qPosition -= 7;
		ECS::GetComponent<AnimationController>(boundaryRight).SetActiveAnim(1);
	}

	else if (boundaryDistanceLeftInactive <= 50)
	{
		ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(80000.f, 80000.f), true);
		ECS::GetComponent<Health>(inactivePlayer).qPosition += 7;
		ECS::GetComponent<AnimationController>(boundaryLeft).SetActiveAnim(1);
	}
	
	SwitchPlayer();
	moveCam = true;
}

void BarBreaker::SwitchPlayer()
{
	moveCam = true;
	
	//Switch the player
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

void BarBreaker::BottleInteractable(float xCord, float yCord)
{
	//creates entity
	auto entity = ECS::CreateEntity();
	bottle.push_back(entity);

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<EntityNumber>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Bottle.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 7, 15);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xCord, yCord, 2.f));
	ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(xCord), float32(yCord));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, OBJECTS, ENVIRONMENT, 1000.f, 3.f);
	tempPhsBody.SetGravityScale(0.f);
}

void BarBreaker::ChairInteractable(float xCord, float yCord)
{
	//creates entity
	auto entity = ECS::CreateEntity();
	chair.push_back(entity);

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<EntityNumber>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Chair.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 15, 30);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xCord, yCord, 2.f));
	ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(xCord), float32(yCord));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, ENEMY, ENVIRONMENT, 1000.f, 3.f);
	tempPhsBody.SetGravityScale(0.f);

}

void BarBreaker::MouseMotion(SDL_MouseMotionEvent evnt)
{
	vec2 cursorRelative(Util::ConvertToGL(m_sceneReg, vec2(evnt.x, evnt.y)));

	/*
		***Functions to find if the player has moved the cursor over a button***
	*/
	if (evnt.type == SDL_MOUSEMOTION && (cursorRelative.x >= smallJumpLButton.min.x && cursorRelative.y >= smallJumpLButton.min.y) &&
		(cursorRelative.x <= smallJumpLButton.max.x && cursorRelative.y <= smallJumpLButton.max.y))
	{
		//If the player wasn't hovering before, set the button hover to true and start counting
		if (!smallJumpLButton.hovering)
		{
			smallJumpLButton.hovering = true;
			ECS::GetComponent<AnimationController>(smallJumpLButton.entity).SetActiveAnim(0);
			smallJumpLButton.startHoverTime = time(0);
		}

		//Look through each button, and set hovering to false if it isn't the current button
		for (int i = 0; i < buttonVecLen; i++)
		{
			if (buttonVec[i]->entity != smallJumpLButton.entity)
			{
				buttonVec[i]->hovering = false;
			}
		}
	}

	else if (evnt.type == SDL_MOUSEMOTION && (cursorRelative.x >= bigJumpLButton.min.x && cursorRelative.y >= bigJumpLButton.min.y) &&
		(cursorRelative.x <= bigJumpLButton.max.x && cursorRelative.y <= bigJumpLButton.max.y))
	{
		if (!bigJumpLButton.hovering)
		{
			bigJumpLButton.hovering = true;
			ECS::GetComponent<AnimationController>(bigJumpLButton.entity).SetActiveAnim(0);
			bigJumpLButton.startHoverTime = time(0);
		}

		for (int i = 0; i < buttonVecLen; i++)
		{
			if (buttonVec[i]->entity != bigJumpLButton.entity)
			{
				buttonVec[i]->hovering = false;
			}
		}
	}

	else if (evnt.type == SDL_MOUSEMOTION && (cursorRelative.x >= smallJumpRButton.min.x && cursorRelative.y >= smallJumpRButton.min.y) &&
		(cursorRelative.x <= smallJumpRButton.max.x && cursorRelative.y <= smallJumpRButton.max.y))
	{
		if (!smallJumpRButton.hovering)
		{
			smallJumpRButton.hovering = true;
			ECS::GetComponent<AnimationController>(smallJumpRButton.entity).SetActiveAnim(0);
			smallJumpRButton.startHoverTime = time(0);
		}

		for (int i = 0; i < buttonVecLen; i++)
		{
			if (buttonVec[i]->entity != smallJumpRButton.entity)
			{
				buttonVec[i]->hovering = false;
			}
		}
	}

	else if (evnt.type == SDL_MOUSEMOTION && (cursorRelative.x >= bigJumpRButton.min.x && cursorRelative.y >= bigJumpRButton.min.y) &&
		(cursorRelative.x <= bigJumpRButton.max.x && cursorRelative.y <= bigJumpRButton.max.y))
	{
		if (!bigJumpRButton.hovering)
		{
			bigJumpRButton.hovering = true;
			ECS::GetComponent<AnimationController>(bigJumpRButton.entity).SetActiveAnim(0);
			bigJumpRButton.startHoverTime = time(0);
		}

		for (int i = 0; i < buttonVecLen; i++)
		{
			if (buttonVec[i]->entity != bigJumpRButton.entity)
			{
				buttonVec[i]->hovering = false;
			}
		}
	}

	else if (evnt.type == SDL_MOUSEMOTION && (cursorRelative.x >= lightAttackButton.min.x && cursorRelative.y >= lightAttackButton.min.y) &&
		(cursorRelative.x <= lightAttackButton.max.x && cursorRelative.y <= lightAttackButton.max.y))
	{
		if (!lightAttackButton.hovering)
		{
			lightAttackButton.hovering = true;
			ECS::GetComponent<AnimationController>(lightAttackButton.entity).SetActiveAnim(0);
			lightAttackButton.startHoverTime = time(0);
		}

		for (int i = 0; i < buttonVecLen; i++)
		{
			if (buttonVec[i]->entity != lightAttackButton.entity)
			{
				buttonVec[i]->hovering = false;
			}
		}
	}

	else if (evnt.type == SDL_MOUSEMOTION && (cursorRelative.x >= heavyAttackButton.min.x && cursorRelative.y >= heavyAttackButton.min.y) &&
		(cursorRelative.x <= heavyAttackButton.max.x && cursorRelative.y <= heavyAttackButton.max.y))
	{
		if (!heavyAttackButton.hovering)
		{
			heavyAttackButton.hovering = true;
			ECS::GetComponent<AnimationController>(heavyAttackButton.entity).SetActiveAnim(0);
			heavyAttackButton.startHoverTime = time(0);
		}

		for (int i = 0; i < buttonVecLen; i++)
		{
			if (buttonVec[i]->entity != heavyAttackButton.entity)
			{
				buttonVec[i]->hovering = false;
			}
		}
	}

	else if (evnt.type == SDL_MOUSEMOTION && (cursorRelative.x >= interactButton.min.x && cursorRelative.y >= interactButton.min.y) &&
	(cursorRelative.x <= interactButton.max.x && cursorRelative.y <= interactButton.max.y))
	{
		if (!interactButton.hovering)
		{
			interactButton.hovering = true;
			ECS::GetComponent<AnimationController>(interactButton.entity).SetActiveAnim(0);
			interactButton.startHoverTime = time(0);
		}

		for (int i = 0; i < buttonVecLen; i++)
		{
			if (buttonVec[i]->entity != interactButton.entity)
			{
				buttonVec[i]->hovering = false;
			}
		}
	}

	else if (evnt.type == SDL_MOUSEMOTION && (cursorRelative.x >= helpButton.min.x && cursorRelative.y >= helpButton.min.y) &&
	(cursorRelative.x <= helpButton.max.x && cursorRelative.y <= helpButton.max.y))
	{
		if (!helpButton.hovering)
		{
			helpButton.hovering = true;
			ECS::GetComponent<AnimationController>(helpButton.entity).SetActiveAnim(0);
			helpButton.startHoverTime = time(0);
		}

		for (int i = 0; i < buttonVecLen; i++)
		{
			if (buttonVec[i]->entity != helpButton.entity)
			{
				buttonVec[i]->hovering = false;
			}
		}
	}

	else
	{
		for (int i = 0; i < buttonVecLen; i++)
		{
			buttonVec[i]->hovering = false;
			ECS::GetComponent<AnimationController>(buttonVec[i]->entity).SetActiveAnim(1);
		}
	}
	
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

	//if the player has clicked on a button, change animation state to clicked
	if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= smallJumpLButton.min.x && cursorRelative.y >= smallJumpLButton.min.y) &&
		(cursorRelative.x <= smallJumpLButton.max.x && cursorRelative.y <= smallJumpLButton.max.y))
	{
		ECS::GetComponent<AnimationController>(smallJumpLButton.entity).SetActiveAnim(2);
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= bigJumpLButton.min.x && cursorRelative.y >= bigJumpLButton.min.y) &&
		(cursorRelative.x <= bigJumpLButton.max.x && cursorRelative.y <= bigJumpLButton.max.y))
	{
		ECS::GetComponent<AnimationController>(bigJumpLButton.entity).SetActiveAnim(2);
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= smallJumpRButton.min.x && cursorRelative.y >= smallJumpRButton.min.y) &&
		(cursorRelative.x <= smallJumpRButton.max.x && cursorRelative.y <= smallJumpRButton.max.y))
	{
		ECS::GetComponent<AnimationController>(smallJumpRButton.entity).SetActiveAnim(2);
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= bigJumpRButton.min.x && cursorRelative.y >= bigJumpRButton.min.y) &&
		(cursorRelative.x <= bigJumpRButton.max.x && cursorRelative.y <= bigJumpRButton.max.y))
	{
		ECS::GetComponent<AnimationController>(bigJumpRButton.entity).SetActiveAnim(2);
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= lightAttackButton.min.x && cursorRelative.y >= lightAttackButton.min.y) &&
		(cursorRelative.x <= lightAttackButton.max.x && cursorRelative.y <= lightAttackButton.max.y))
	{
		ECS::GetComponent<AnimationController>(lightAttackButton.entity).SetActiveAnim(2);
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= heavyAttackButton.min.x && cursorRelative.y >= heavyAttackButton.min.y) &&
		(cursorRelative.x <= heavyAttackButton.max.x && cursorRelative.y <= heavyAttackButton.max.y))
	{
		ECS::GetComponent<AnimationController>(heavyAttackButton.entity).SetActiveAnim(2);
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= interactButton.min.x && cursorRelative.y >= interactButton.min.y) &&
		(cursorRelative.x <= interactButton.max.x && cursorRelative.y <= interactButton.max.y))
	{
		ECS::GetComponent<AnimationController>(interactButton.entity).SetActiveAnim(2);
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= helpButton.min.x && cursorRelative.y >= helpButton.min.y) &&
		(cursorRelative.x <= helpButton.max.x && cursorRelative.y <= helpButton.max.y))
	{
		ECS::GetComponent<AnimationController>(helpButton.entity).SetActiveAnim(2);
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(1.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONDOWN)
	{
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}


	//if the player has clicked on a button, change animation state back to hover
	if (evnt.type == SDL_MOUSEBUTTONUP && (cursorRelative.x >= smallJumpLButton.min.x && cursorRelative.y >= smallJumpLButton.min.y) &&
		(cursorRelative.x <= smallJumpLButton.max.x && cursorRelative.y <= smallJumpLButton.max.y))
	{
		ECS::GetComponent<AnimationController>(smallJumpLButton.entity).SetActiveAnim(0);
		SmallMoveLeft();
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONUP && (cursorRelative.x >= bigJumpLButton.min.x && cursorRelative.y >= bigJumpLButton.min.y) &&
		(cursorRelative.x <= bigJumpLButton.max.x && cursorRelative.y <= bigJumpLButton.max.y))
	{
		ECS::GetComponent<AnimationController>(bigJumpLButton.entity).SetActiveAnim(0);
		BigMoveLeft();
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONUP && (cursorRelative.x >= smallJumpRButton.min.x && cursorRelative.y >= smallJumpRButton.min.y) &&
		(cursorRelative.x <= smallJumpRButton.max.x && cursorRelative.y <= smallJumpRButton.max.y))
	{
		ECS::GetComponent<AnimationController>(smallJumpRButton.entity).SetActiveAnim(0);
		SmallMoveRight();
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONUP && (cursorRelative.x >= bigJumpRButton.min.x && cursorRelative.y >= bigJumpRButton.min.y) &&
		(cursorRelative.x <= bigJumpRButton.max.x && cursorRelative.y <= bigJumpRButton.max.y))
	{
		ECS::GetComponent<AnimationController>(bigJumpRButton.entity).SetActiveAnim(0);
		BigMoveRight();
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONUP && (cursorRelative.x >= lightAttackButton.min.x && cursorRelative.y >= lightAttackButton.min.y) &&
		(cursorRelative.x <= lightAttackButton.max.x && cursorRelative.y <= lightAttackButton.max.y))
	{
		ECS::GetComponent<AnimationController>(lightAttackButton.entity).SetActiveAnim(0);
		LightAttack();
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONUP && (cursorRelative.x >= heavyAttackButton.min.x && cursorRelative.y >= heavyAttackButton.min.y) &&
		(cursorRelative.x <= heavyAttackButton.max.x && cursorRelative.y <= heavyAttackButton.max.y))
	{
		ECS::GetComponent<AnimationController>(heavyAttackButton.entity).SetActiveAnim(0);
		HeavyAttack();
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONUP && (cursorRelative.x >= interactButton.min.x && cursorRelative.y >= interactButton.min.y) &&
		(cursorRelative.x <= interactButton.max.x && cursorRelative.y <= interactButton.max.y))
	{
		ECS::GetComponent<AnimationController>(interactButton.entity).SetActiveAnim(0);
		PickupBottle();
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(0.f);
	}

	else if (evnt.type == SDL_MOUSEBUTTONUP && (cursorRelative.x >= helpButton.min.x && cursorRelative.y >= helpButton.min.y) &&
		(cursorRelative.x <= helpButton.max.x && cursorRelative.y <= helpButton.max.y))
	{
		ECS::GetComponent<AnimationController>(helpButton.entity).SetActiveAnim(0);
		ECS::GetComponent<Sprite>(helpTextImage).SetTransparency(1.f);
	}

}

bool BarBreaker::IsFinished()
{
	return finished;
}

int BarBreaker::GetWinner()
{
	return winner;
}