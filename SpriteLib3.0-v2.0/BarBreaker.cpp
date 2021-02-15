#include "BarBreaker.h"
#include "Utilities.h"

BarBreaker::BarBreaker(std::string name)
	:Scene(name)
{
	//no gravity this is a top down scene
	m_gravity = b2Vec2(0.f, 0.f);
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

	//Setup new Entity
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "LinkStandby.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);

	}
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

}

void updateCamera() {
	auto& player = ECS::GetComponent<Transform>(MainEntities::MainPlayer());
	auto& camera = ECS::GetComponent<Camera>(MainEntities::MainCamera());
	static bool f = false;
	//player1 = first player's physics body
	//player2 = second player's physics body

	pos p1Pos, p2Pos;
	p1Pos.x = player1.GetBody()->GetPosition().x;
	p1Pos.y = player1.GetBody()->GetPosition().y;

	p2Pos.x = player2.GetBody()->GetPosition().x;
	p2Pos.y = player2.GetBody()->GetPosition().y;

	float averageX, averageY;
	averageX = (p2Pos.x + p1Pos.x) / 2.0;
	averageY = (p1Pos.y + p2Pos.y) / 2.0;

	player.SetPositionX(averageX);
	player.SetPositionY(averageY);

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
	updateCamera();
}

void BarBreaker::AdjustScrollOffset()
{
}

void BarBreaker::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	float speed = 1.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 5.f;
	}

	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 0.f), true);
	}

	//Change physics body size for circle
	if (Input::GetKey(Key::O))
	{
		player.ScaleBody(1.3 * Timer::deltaTime, 0);
	}
	else if (Input::GetKey(Key::I))
	{
		player.ScaleBody(-1.3 * Timer::deltaTime, 0);
	}

}

void BarBreaker::KeyboardDown()
{
}

void BarBreaker::KeyboardUp()
{
}
