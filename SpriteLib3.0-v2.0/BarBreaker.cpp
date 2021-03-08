#include "BarBreaker.h"
#include "Utilities.h"
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

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "SquareMask.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

		moveButton.entity = entity;
		moveButton.min = vec2(BackEnd::GetWindowWidth() / 2 - 4 * ((ECS::GetComponent<Sprite>(moveButton.entity).GetWidth() / 2)),
			BackEnd::GetWindowHeight() / 2 - 4 * ((ECS::GetComponent<Sprite>(moveButton.entity).GetHeight()) / 2));

		moveButton.max = vec2(BackEnd::GetWindowWidth() / 2 + 4 * ((ECS::GetComponent<Sprite>(moveButton.entity).GetWidth() / 2)),
			BackEnd::GetWindowHeight() / 2 + 4 * ((ECS::GetComponent<Sprite>(moveButton.entity).GetHeight()) / 2));

	}

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
		std::string fileName = "FacingRight.png";
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
		std::string fileName = "FacingLeft.png";
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
		tempDef.position.Set(float32(350.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 1000.f, 3.f);

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
		tempDef.position.Set(float32(-350.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 1000.f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(activePlayer));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(activePlayer));
}

void BarBreaker::UpdateCamera() {
	auto& dummy = ECS::GetComponent<Transform>(MainEntities::MainPlayer());
	auto& camera = ECS::GetComponent<Camera>(MainEntities::MainCamera());
	static bool f = false;
	//player1 = first player's physics body 
	//player2 = second player's physics body 

	pos p1Pos, p2Pos;
	p1Pos.x = ECS::GetComponent<PhysicsBody>(player1).GetBody()->GetPosition().x;
	p1Pos.y = ECS::GetComponent<PhysicsBody>(player1).GetBody()->GetPosition().y;

	p2Pos.x = ECS::GetComponent<PhysicsBody>(player2).GetBody()->GetPosition().x;
	p2Pos.y = ECS::GetComponent<PhysicsBody>(player2).GetBody()->GetPosition().y;

	static float averageX, averageY;
	averageX = (p2Pos.x + p1Pos.x) / 2.0;
	averageY = (p1Pos.y + p2Pos.y) / 2.0;

	//dummy.SetPositionX(averageX);
	//dummy.SetPositionY(averageY);

	static float dist, old = 0;
	pos xy;
	xy.x = pow((p1Pos.x - p2Pos.x), 2);
	xy.y = pow((p1Pos.y - p2Pos.y), 2);
	dist = sqrt(xy.x + xy.y);
	dist /= 100;

	//dist /= 5; 
	dist -= dist / 1.5;
	//lower value = more zoomed out 
	//should not go above 20 or 25 

	/*if (f) {
		camera.Zoom(5);
		f = false;
	}*/

	if (f) {
		if (old > dist) {
			camera.Zoom(dist);
		}
		else if (old < dist) {
			dist -= 0.05;
			camera.Zoom(-dist);
		}
		f = false;
	}
	else {
		old = dist;
		f = true;
	}
}

void BarBreaker::Update()
{
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

	//If the player ended their turn, and the system hasnt begun counting yet
	if (turnEnd && !counting)
	{
		counting = true;
		beginClk = time(0);
	}

	//Once the system has begun counting, end the turn only after five seconds have passed
	else if (turnEnd && (time(0) - beginClk >= 5))
	{
		EndTurn();
	}

	//Find the camera x and y, set the button sprite to them
	auto newX = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPositionX();
	auto newY = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPositionY();

	ECS::GetComponent<Transform>(moveButton.entity).SetPosition(vec3(newX, newY, 10.f));

	// CONTINUE HERE ***************
	//moveButton.min.x = ECS::GetComponent<Transform>(moveButton.entity).GetPositionX() - (ECS::GetComponent<Sprite>(moveButton.entity).GetWidth() / 2);
	//moveButton.min.y = ECS::GetComponent<Transform>(moveButton.entity).GetPositionY() - (ECS::GetComponent<Sprite>(moveButton.entity).GetHeight() / 2);

	//moveButton.max.x = ECS::GetComponent<Transform>(moveButton.entity).GetPositionX() + (ECS::GetComponent<Sprite>(moveButton.entity).GetWidth() / 2);
	//moveButton.max.y = ECS::GetComponent<Transform>(moveButton.entity).GetPositionY() + (ECS::GetComponent<Sprite>(moveButton.entity).GetHeight() / 2);

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

void BarBreaker::KeyboardUp()
{
}

void BarBreaker::SmallMoveRight()
{
	ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(60000.f, 50000.f), true);
	movesTaken++;
	if (movesTaken == 2)
	{
		turnEnd = true;
		//EndTurn();
	}
}

void BarBreaker::SmallMoveLeft()
{
	ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-60000.f, 50000.f), true);
	movesTaken++;
	if (movesTaken == 2)
	{
		turnEnd = true;
		//EndTurn();
	}
}

void BarBreaker::BigMoveRight()
{
	ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(100000.f, 90000.f), true);
	movesTaken++;
	if (movesTaken == 2)
	{
		turnEnd = true;
		//EndTurn();
	}
}

void BarBreaker::BigMoveLeft()
{
	ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-100000.f, 90000.f), true);
	movesTaken++;
	if (movesTaken == 2)
	{
		turnEnd = true;
		//EndTurn();
	}
}

void BarBreaker::Punch()
{
	ToneFire::CoreSound testSound{ "punch.wav" };

	if (abs(playerDistance) <= 60)
	{
		testSound.Play();
		testSound.SetVolume(0.5);
		if (playerDistance < 0)
		{
			ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(60000.f, 80000.f), true);
			turnEnd = true;
			//EndTurn();

		}
		else if (playerDistance > 0)
		{
			ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-60000.f, 80000.f), true);
			turnEnd = true;
			//EndTurn();

		}
	}
}

void BarBreaker::EndTurn()
{
	movesTaken = 0;
	turnEnd = false;
	counting = false;

	if (boundaryDistanceRightActive <= 30)
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-60000.f, 80000.f), true);
	}
	
	else if (boundaryDistanceLeftActive <= 30)
	{
		ECS::GetComponent<PhysicsBody>(activePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(60000.f, 80000.f), true);
	}

	if (boundaryDistanceRightInactive <= 30)
	{
		ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-60000.f, 80000.f), true);
	}

	else if (boundaryDistanceLeftInactive <= 30)
	{
		ECS::GetComponent<PhysicsBody>(inactivePlayer).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(60000.f, 80000.f), true);
	}

	SwitchPlayer();
}

void BarBreaker::SwitchPlayer()
{
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
	//Change the cursor location to use the bottom-left of the screen as the origin
	vec2 cursorRelative(evnt.x, BackEnd::GetWindowHeight() - evnt.y);
	/*std::cout << "Cursor X: " << cursorRelative.x << std::endl << "Cursor Y: "<< cursorRelative.y << std::endl;
	std::cout << "Button min X: " << moveButton.min.x << std::endl << "Button min Y: " << moveButton.min.y << std::endl;
	std::cout << "Button max X: " << moveButton.max.x << std::endl << "Button max Y: " << moveButton.max.y << std::endl;
	std::cout << "Box Width: " << ECS::GetComponent<Sprite>(moveButton.entity).GetWidth() << std::endl 
		<< "Box Height: " << ECS::GetComponent<Sprite>(moveButton.entity).GetHeight() << std::endl;
	//std::cout << "Window Length: " << BackEnd::GetWindowWidth() << std::endl
	//	<< "Window Height: " << BackEnd::GetWindowHeight() << std::endl;*/

	if (evnt.type == SDL_MOUSEBUTTONDOWN && (cursorRelative.x >= moveButton.min.x && cursorRelative.y >= moveButton.min.y) &&
		(cursorRelative.x <= moveButton.max.x && cursorRelative.y <= moveButton.max.y))
	{
		BigMoveRight();
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
		Punch();
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