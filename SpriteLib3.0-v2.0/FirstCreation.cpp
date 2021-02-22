#include "FirstCreation.h"
#include "Utilities.h"
#include "Tone Fire/Tonefire.h"

FirstCreation::FirstCreation(std::string name)
	: Scene(name)
{
	//no gravity this is a top down scene
	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);
}

void FirstCreation::InitScene(float windowWidth, float windowHeight)
{
	//ToneFire::FMODCore fmod{};

	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//ToneFire::CoreSound testSound{ "test.mp3" };

	//testSound.Play();

	//Setup Main Camera entity
	{
		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);
		//ECS::AttachComponent<HorizontalScroll>(entity);
		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to the vert and horizontal scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Setup static box
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//Setup new Entity
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "LinkStandby.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void FirstCreation::Update()
{
	//ToneFire::FMODCore fmod{};

	//ToneFire::CoreSound testSound("test.mp3");

	//testSound.Play();

	//Scene::AdjustScrollOffset();

	//fmod.Update();
}

void FirstCreation::KeyboardHold()
{
	auto& player = ECS::GetComponent<Transform>(MainEntities::MainPlayer());
	float speed = 15.f;
	//ToneFire::FMODCore fmod{};
	//ToneFire::CoreSound testSound{ "test.mp3" };

	if (Input::GetKey(Key::W))
	{
		player.SetPositionY(player.GetPositionY() + (speed * Timer::deltaTime));
	}
	if (Input::GetKey(Key::S))
	{
		player.SetPositionY(player.GetPositionY() - (speed * Timer::deltaTime));
	}

	if (Input::GetKey(Key::A))
	{
		player.SetPositionX(player.GetPositionX() - (speed * Timer::deltaTime));
	}
	if (Input::GetKey(Key::D))
	{
		player.SetPositionX(player.GetPositionX() + (speed * Timer::deltaTime));
	}

	if (Input::GetKey(Key::P))
	{
		//testSound.Play();
	}
}

void FirstCreation::KeyboardDown()
{
}

void FirstCreation::KeyboardUp()
{
}
//test again