#include "EndScreen.h"
#include "Utilities.h"
#include "Timer.h"
#include "Tone Fire/Tonefire.h"

EndScreen::EndScreen(std::string name)
	:Scene(name)
{
	//no gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -9.f);
	m_physicsWorld->SetGravity(m_gravity);
}

void EndScreen::InitScene(float windowWidth, float windowHeight)
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

	//P1 Winner Text
	{
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "p1WinsImage.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 200);
		
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

		if (winner == 1)
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		}

		else
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
	}

	//P2 Winner Text
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "p2WinsImage.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 200);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));

		if (winner == 2)
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		}

		else
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void EndScreen::Update()
{

}

bool EndScreen::IsFinished()
{
	return false;
}

void EndScreen::SetWinner(int in_winner)
{
	winner = in_winner;
}
