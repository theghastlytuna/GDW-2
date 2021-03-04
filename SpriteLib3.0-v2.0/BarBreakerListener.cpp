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
			ECS::GetComponent<CanJump>((int)fixtureA->GetBody()->GetUserData()).m_canJump = true;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).m_canJump = true;
		}
	}

	if ((filterA.categoryBits == PICKUP) || (filterB.categoryBits == PICKUP))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).reduceHealth(10);
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).reduceHealth(10);
		}

		if (filterA.categoryBits == PICKUP)
		{
			PhysicsBody::m_bodiesToDelete.push_back(ECS::GetComponent<EntityNumber>((int)fixtureA->GetBody()->GetUserData()).entityNumber);
		}
		if (filterB.categoryBits == PICKUP)
		{
			PhysicsBody::m_bodiesToDelete.push_back(ECS::GetComponent<EntityNumber>((int)fixtureB->GetBody()->GetUserData()).entityNumber);
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
