#include "OpenScreen.h"
#include "Utilities.h"
#include "Timer.h"
#include "Tone Fire/Tonefire.h"

OpenScreen::OpenScreen(std::string name)
	:Scene(name)
{
	//no gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -9.f);
	m_physicsWorld->SetGravity(m_gravity);
}

void OpenScreen::InitScene(float windowWidth, float windowHeight)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;
	Scene::SetClearColor(vec4(0.65098, 0.47843, 0.28627, 0));

	Scene::CreateCameraEntity(true, windowWidth, windowHeight, -75.f, 75.f, -75.f, 75.f, -100.f, 100.f, aspectRatio, true, true);
	ECS::GetComponent<Camera>(MainEntities::MainCamera()).Zoom(-20);

	

	//P2 Winner Text
	{
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "titleGraphic.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 120);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));
		
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		
	}

	//P1 Winner Text
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "introText.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 120, 40);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -75.f, 10.f));

		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void OpenScreen::Update()
{

}

void OpenScreen::KeyboardDown()
{
	if (Input::GetKeyDown(Key::Space))
	{
		finished = true;
	}
}

bool OpenScreen::IsFinished()
{
	return finished;
}

void OpenScreen::SetWinner(int in_winner)
{
	winner = in_winner;
}