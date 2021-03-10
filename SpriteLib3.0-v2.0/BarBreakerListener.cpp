#include "BarBreakerListener.h"
#include "ECS.h"

BarBreakerListener::BarBreakerListener()
	:b2ContactListener()
{

}

void BarBreakerListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerEnter(fixtureA);
		}
		else if (sensorB)
		{
			TriggerEnter(fixtureB);
		}
	}

	b2Filter filterA = fixtureA->GetFilterData();
	b2Filter filterB = fixtureB->GetFilterData();

	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == GROUND) || (filterB.categoryBits == PLAYER && filterA.categoryBits == GROUND))
	{
		if (filterA.categoryBits == PLAYER)
		{
			int playerNum = (int)fixtureA->GetBody()->GetUserData();
			ECS::GetComponent<CanJump>(playerNum).m_canJump = true;
			//Sets the entity position to qPosition times 10 (10:1 ratio engine to game units)
			ECS::GetComponent<PhysicsBody>(playerNum).SetPosition(b2Vec2(
				(float32)ECS::GetComponent<Health>(playerNum).qPosition * 10, ECS::GetComponent<PhysicsBody>(playerNum).GetPosition().y), true);
		}
		else if (filterB.categoryBits == PLAYER)
		{
			int playerNum = (int)fixtureB->GetBody()->GetUserData();
			ECS::GetComponent<CanJump>(playerNum).m_canJump = true;
			//Sets the entity position to qPosition times 10 (10:1 ratio engine to game units)
			ECS::GetComponent<PhysicsBody>(playerNum).SetPosition(b2Vec2(
				(float32)ECS::GetComponent<Health>(playerNum).qPosition * 10, ECS::GetComponent<PhysicsBody>(playerNum).GetPosition().y), true);
		}
	}
	//PICKUP is a thrown item, checks for any collisions of thrown items
	if ((filterA.categoryBits == PICKUP) || (filterB.categoryBits == PICKUP))
	{
		//if it hits a player, deals damage
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).reduceHealth(10);
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).reduceHealth(10);
		}

		//deletes the object on contact with anything
		if (filterA.categoryBits == PICKUP)
		{
			PhysicsBody::m_bodiesToDelete.push_back((int)fixtureA->GetBody()->GetUserData());
		}
		if (filterB.categoryBits == PICKUP)
		{
			PhysicsBody::m_bodiesToDelete.push_back((int)fixtureB->GetBody()->GetUserData());
		}

	}
}

void BarBreakerListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerExit(fixtureA);
		}
		else if (sensorB)
		{
			TriggerExit(fixtureB);
		}
	}

	b2Filter filterA = fixtureA->GetFilterData();
	b2Filter filterB = fixtureB->GetFilterData();

	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == GROUND) || (filterB.categoryBits == PLAYER && filterA.categoryBits == GROUND))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureA->GetBody()->GetUserData()).m_canJump = false;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).m_canJump = false;
		}
	}
}

void BarBreakerListener::TriggerEnter(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnEnter();

}

void BarBreakerListener::TriggerExit(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnExit();

}
