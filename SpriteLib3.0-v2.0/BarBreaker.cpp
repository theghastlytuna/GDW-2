#include "BarBreaker.h"
#include "Utilities.h"
#include "Timer.h"
#include "DestroyTrigger.h"
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

	Scene::CreatePlatform("boxSprite.jpg", 600.f, 20, 0, -30.f, 0.f, 0.f, 0.f);

	//ToneFire::CoreSound testSound{ "test2.mp3" };
	//backgroundMusic = testSound;
	backgroundMusic.Play();
	backgroundMusic.SetVolume(0.1);

		//background
	{
		auto entity = ECS::CreateEntity();
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "BarBreakerBackground.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 800, 200);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, -20.f));
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

		//Sets up components
		std::string fileName = "FacingRight.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-70.f, 50.f, 2.f));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-70.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | PICKUP | TRIGGER, 1000.f, 3.f);

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

		//Sets up components
		std::string fileName = "FacingLeft.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(70.f, 50.f, 2.f));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(70.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | PICKUP | TRIGGER, 1000.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}

	//bottle
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
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-135.f, 0.f, 2.f));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-135.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, OBJECTS, ENVIRONMENT, 1000.f, 3.f);
		tempPhsBody.SetGravityScale(0.f);

		//std::map<float, float> tempCoord;
		//tempCoord.insert(std::pair<float, float>(0.f, 10.f));

	}

	//bottle 2
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
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(135.f, 0.f, 2.f));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(135.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, OBJECTS, ENVIRONMENT, 1000.f, 3.f);
		tempPhsBody.SetGravityScale(0.f);
	}

	//bottle 3
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
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-200.f, 0.f, 2.f));
		ECS::GetComponent<EntityNumber>(entity).entityNumber = entity;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-200.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, OBJECTS, ENVIRONMENT, 1000.f, 3.f);
		tempPhsBody.SetGravityScale(0.f);
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

	//Once the system has begun counting, end the turn only after five seconds have passed
	else if (turnEnd && (time(0) - beginClk >= 3))
	{
		EndTurn();
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
		BarBreaker::PickupBottle();
	}
	if (Input::GetKeyDown(Key::L))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	/*if (Input::GetKeyDown(Key::G)) {
		PickupBottle();
		{
			std::cout << "X: " << ECS::GetComponent<Transform>(activePlayer).GetPositionX() << " Y: " << ECS::GetComponent<Transform>(activePlayer).GetPositionY() << std::endl;
		}
	}*/
}

void BarBreaker::KeyboardUp()
{
}

void BarBreaker::SmallMoveRight()
{
	if (!counting)
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(60000.f, 50000.f), true);
		lightMoves++;
	}
}

void BarBreaker::SmallMoveLeft()
{
	if (!counting)
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-60000.f, 50000.f), true);
		lightMoves++;
	}
}

void BarBreaker::BigMoveRight()
{
	if (heavyMoves < 1 && !counting)
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(100000.f, 90000.f), true);
		heavyMoves++;
	}
}

void BarBreaker::BigMoveLeft()
{
	if (heavyMoves < 1 && !counting)
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-100000.f, 90000.f), true);
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
			testSound.Play();
			testSound.SetVolume(0.5);
			if (playerDistance < 0)
			{
				ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(60000.f, 80000.f), true);
				lightMoves++;

			}
			else if (playerDistance > 0)
			{
				ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-60000.f, 80000.f), true);
				lightMoves++;

			}
		}
	}
}

void BarBreaker::PickupBottle() {
	pos playerPos; //position of active player
	playerPos.x = ECS::GetComponent<Transform>(activePlayer).GetPositionX();
	playerPos.y = ECS::GetComponent<Transform>(activePlayer).GetPositionY();

	for (int i = 0; i < bottle.size(); i++) {
		//check if active player's x pos is within bottle range
		float bottlex = ECS::GetComponent<Transform>(bottle[i]).GetPositionX();
		float dist = bottlex - playerPos.x;
		if (dist < 0) dist = -dist;

		if (dist <= bottleSize) {
			//player is within range
			PhysicsBody::m_bodiesToDelete.push_back(bottle[i]);
			//ECS::GetComponent<PhysicsBody>(bottle[i]).DeleteBody();
			bottle.erase(bottle.begin() + i);

			std::cout << "\nPicked up bottle\n";

			ThrowBottle();
			return;
		}
	}
	std::cout << "\nNo bottles\n";
}

void BarBreaker::ThrowBottle()
{
//	if (ECS::GetComponent<HasBottle>(activePlayer).hasBottle)
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
		//ECS::GetComponent<HasBottle>(activePlayer).hasBottle = false;
	}
	//else std::cout << "no bottles\n";
}

void BarBreaker::EndTurn()
{
	lightMoves = 0;
	heavyMoves = 0;
	turnEnd = false;
	counting = false;

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

void BarBreaker::GUI()
{
	GUIWindowUI();

	/*if (m_firstWindow)
	{
		GUIWindowOne();
	}

	if (m_secondWindow)
	{
		GUIWindowTwo();
	}*/

	GUIWindowOne();
	GUIWindowTwo();
}

void BarBreaker::GUIWindowUI()
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

	/*
	ImGui::Begin("Healthbar");

	static std::string imageLoad = "HealthTest.png";

	ImGui::Image((void*)(intptr_t)TextureManager::FindTexture(imageLoad)->GetID(), ImVec2(150.f, 150.f), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
	*/
}

void BarBreaker::GUIWindowOne()
{
	/*//Window begin
	ImGui::Begin("Side Docked Window");
	//is the buffer initialized
	static bool init = false;

	//Buffers
	const unsigned int BUF_LEN = 512;
	static char buf[BUF_LEN];
	//Image load
	static std::string imageLoad = "LinkStandby.png";
	//World gravity slider
	float gravity[2] = { m_physicsWorld->GetGravity().x, m_physicsWorld->GetGravity().y };

	if (!init)
	{
		memset(buf, 0, BUF_LEN);
		init = true;
	}
	m_physicsWorld->SetAllowSleeping(true);

	//ImGui content
	if (ImGui::TreeNode("Vignette Effect"))
	{
		if (EffectManager::GetVignetteHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
				float innerRad = vig->GetInnerRadius();
				float outerRad = vig->GetOuterRadius();
				float opacity = vig->GetOpacity();

				if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.f, outerRad))
				{
					vig->SetInnerRadius(innerRad);
				}
				if (ImGui::SliderFloat("Outer Radius", &outerRad, innerRad, 1.f))
				{
					vig->SetOuterRadius(outerRad);
				}
				if (ImGui::SliderFloat("Opacity", &opacity, 0.f, 1.f))
				{
					vig->SetOpacity(opacity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetVignetteHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sepia Effect"))
	{
		if (EffectManager::GetSepiaHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				SepiaEffect* sep = (SepiaEffect*)EffectManager::GetEffect(EffectManager::GetSepiaHandle());
				float intensity = sep->GetIntensity();

				if (ImGui::SliderFloat("Inner Radius", &intensity, 0.f, 1.f))
				{
					sep->SetIntensity(intensity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetSepiaHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Sepia, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	ImGui::Separator();

	ImGui::Text("Editing World Variables!");
	if (ImGui::SliderFloat2("World Gravity", gravity, -1000.f, 1000.f, "%.2f"))
	{
		m_physicsWorld->SetGravity(b2Vec2(gravity[0], gravity[1]));
		m_physicsWorld->SetAllowSleeping(false);
	}

	ImGui::Separator();

	ImGui::Text("Displaying image dynamically");
	ImGui::InputText("Input filename for image", buf, BUF_LEN, ImGuiInputTextFlags_CharsNoBlank);

	if (ImGui::Button("Confirm File", ImVec2(100.f, 30.f)))
	{
		imageLoad = buf;
	}

	ImGui::Image((void*)(intptr_t)TextureManager::FindTexture(imageLoad)->GetID(), ImVec2(150.f, 150.f), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();

	ImGui::Text("Editing Colors");
	ImGui::ColorPicker4("Scene Background Color", &m_clearColor.x);

	ImGui::End();*/
}

void BarBreaker::GUIWindowTwo()
{
	/*//Second docked window
	ImGui::Begin("Second Window");
	//Image load
	static std::string imageLoad1 = "FunnyButton.png";
	static std::string imageLoad2 = "LikeButton.png";
	static std::string hahaPressed = "";
	static std::string likePressed = "";

	ImGui::BeginTabBar("Tab Bar Test");

	if (ImGui::BeginTabItem("Tab 1"))
	{
		ImGui::Text("You are within Tab 1");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad1)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
		{
			hahaPressed = "You shouldn't have pressed that...";
		}

		ImGui::Text("%s", hahaPressed.c_str());

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 2"))
	{
		ImGui::Text("You are within Tab 2");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad2)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
		{
			likePressed = "LIKED!!!";
		}

		ImGui::Text("%s", likePressed.c_str());

		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();

	ImGui::End();*/
}