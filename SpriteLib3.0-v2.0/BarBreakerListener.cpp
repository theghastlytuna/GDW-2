#include "BarBreakerListener.h"
#include "BarBreaker.h"
#include <random>
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

	if (filterA.categoryBits == PLAYER && filterB.categoryBits == PLAYER)
	{
		if (ECS::GetComponent<CanJump>((int)fixtureA->GetBody()->GetUserData()).m_canJump == true)
		{
			ECS::GetComponent<PhysicsBody>((int)fixtureA->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-62000, 50000), true);

		}
		else if (ECS::GetComponent<CanJump>((int)fixtureA->GetBody()->GetUserData()).m_canJump == false)
		{
			ECS::GetComponent<PhysicsBody>((int)fixtureA->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-130000, 50000), true);
		}

		if (ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).m_canJump == true)
		{
			ECS::GetComponent<PhysicsBody>((int)fixtureB->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(62000, 50000), true);
		}
		else if (ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).m_canJump == false)
		{
			ECS::GetComponent<PhysicsBody>((int)fixtureB->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(130000, 50000), true);
		}

		ECS::GetComponent<AnimationController>((int)fixtureA->GetBody()->GetUserData()).SetActiveAnim(3);
		ECS::GetComponent<AnimationController>((int)fixtureB->GetBody()->GetUserData()).SetActiveAnim(3);
		ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).qPosition = (ECS::GetComponent<PhysicsBody>((int)fixtureA->GetBody()->GetUserData()).GetPosition().x / 10) - 3;
		ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).qPosition = (ECS::GetComponent<PhysicsBody>((int)fixtureB->GetBody()->GetUserData()).GetPosition().x / 10) + 3;
	}

	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == GROUND) || (filterB.categoryBits == PLAYER && filterA.categoryBits == GROUND))
	{
		if (filterA.categoryBits == PLAYER)
		{
			int playerNum = (int)fixtureA->GetBody()->GetUserData();
			ECS::GetComponent<AnimationController>(playerNum).SetActiveAnim(0);
			ECS::GetComponent<CanJump>(playerNum).m_canJump = true;
			//Sets the entity position to qPosition times 10 (10:1 ratio engine to game units)
			ECS::GetComponent<PhysicsBody>(playerNum).SetPosition(b2Vec2(
				(float32)ECS::GetComponent<Health>(playerNum).qPosition * 10, ECS::GetComponent<PhysicsBody>(playerNum).GetPosition().y), true);
		}
		else if (filterB.categoryBits == PLAYER)
		{
			int playerNum = (int)fixtureB->GetBody()->GetUserData();
			ECS::GetComponent<AnimationController>(playerNum).SetActiveAnim(0);
			ECS::GetComponent<CanJump>(playerNum).m_canJump = true;
			//Sets the entity position to qPosition times 10 (10:1 ratio engine to game units)
			ECS::GetComponent<PhysicsBody>(playerNum).SetPosition(b2Vec2(
				(float32)ECS::GetComponent<Health>(playerNum).qPosition * 10, ECS::GetComponent<PhysicsBody>(playerNum).GetPosition().y), true);
		}
	}

	//PICKUP is a thrown bottle, checks for any collisions of thrown items
	if ((filterA.categoryBits == PICKUP) || (filterB.categoryBits == PICKUP))
	{
		srand(time(0));
		int damage = rand() % 8 + 5;

		//if it hits a player, deals damage
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).reduceHealth(damage);

			//If it's player 1, send them leftwards
			if (ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).playerNum == 1)
			{
				ECS::GetComponent<PhysicsBody>((int)fixtureA->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 40000.f), true);
				ECS::GetComponent<AnimationController>((int)fixtureA->GetBody()->GetUserData()).SetActiveAnim(3);
				//ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).qPosition -= 5;
			}

			//If it's player 2, send them rightwards
			else
			{
				ECS::GetComponent<PhysicsBody>((int)fixtureA->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 40000.f), true);
				ECS::GetComponent<AnimationController>((int)fixtureA->GetBody()->GetUserData()).SetActiveAnim(3);
				//ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).qPosition += 5;
			}	
		}

		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).reduceHealth(damage);
			
			//If it's player 1, send them leftwards
			if (ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).playerNum == 1)
			{
				ECS::GetComponent<PhysicsBody>((int)fixtureB->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 40000.f), true);
				ECS::GetComponent<AnimationController>((int)fixtureB->GetBody()->GetUserData()).SetActiveAnim(3);
				//ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).qPosition -= 5;
			}

			//If it's player 2, send them rightwards
			else
			{
				ECS::GetComponent<PhysicsBody>((int)fixtureB->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 40000.f), true);
				ECS::GetComponent<AnimationController>((int)fixtureB->GetBody()->GetUserData()).SetActiveAnim(3);;
				//ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).qPosition += 5;
			}
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

	//ENEMY is a thrown chair, checks for any collisions of thrown items
	if ((filterA.categoryBits == ENEMY) || (filterB.categoryBits == ENEMY))
	{
		srand(time(0));
		int damage = rand() % 8 + 10;

		//if it hits a player, deals damage
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).reduceHealth(damage);

			//If it's player 1, send them leftwards
			if (ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).playerNum == 1)
			{
				ECS::GetComponent<PhysicsBody>((int)fixtureA->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-85000.f, 75000.f), true);
				ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).qPosition -= 5;
				ECS::GetComponent<AnimationController>((int)fixtureA->GetBody()->GetUserData()).SetActiveAnim(3);
			}

			//If it's player 2, send them rightwards
			else
			{
				ECS::GetComponent<PhysicsBody>((int)fixtureA->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(85000.f, 75000.f), true);
				ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).qPosition += 5;
				ECS::GetComponent<AnimationController>((int)fixtureA->GetBody()->GetUserData()).SetActiveAnim(3);
			}
		}

		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).reduceHealth(damage);

			//If it's player 1, send them leftwards
			if (ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).playerNum == 1)
			{
				ECS::GetComponent<PhysicsBody>((int)fixtureB->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-85000.f, 75000.f), true);
				ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).qPosition -= 5;
				ECS::GetComponent<AnimationController>((int)fixtureB->GetBody()->GetUserData()).SetActiveAnim(3);
			}

			//If it's player 2, send them rightwards
			else
			{
				ECS::GetComponent<PhysicsBody>((int)fixtureB->GetBody()->GetUserData()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(85000.f, 75000.f), true);
				ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).qPosition += 5;
				ECS::GetComponent<AnimationController>((int)fixtureB->GetBody()->GetUserData()).SetActiveAnim(3);
			}
		}

		//deletes the object on contact with anything
		if (filterA.categoryBits == ENEMY)
		{
			PhysicsBody::m_bodiesToDelete.push_back((int)fixtureA->GetBody()->GetUserData());
		}
		if (filterB.categoryBits == ENEMY)
		{
			PhysicsBody::m_bodiesToDelete.push_back((int)fixtureB->GetBody()->GetUserData());
		}
	}

	//PLAYER-BOUNDARY collision
	if ((filterA.categoryBits == BOUNDARY) || (filterB.categoryBits == BOUNDARY))
	{
		if (filterA.categoryBits == PLAYER)
		{
			//Set the player's qPosition based on if they are touching the left or right boundary
			if (ECS::GetComponent<PhysicsBody>((int)fixtureA->GetBody()->GetUserData()).GetPosition().x < 0)
			{
				ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).qPosition = -27;
			}

			else
			{
				ECS::GetComponent<Health>((int)fixtureA->GetBody()->GetUserData()).qPosition = 27;
			}
		}

		else if (filterB.categoryBits == PLAYER)
		{
			//Set the player's qPosition based on if they are touching the left or right boundary
			if (ECS::GetComponent<PhysicsBody>((int)fixtureB->GetBody()->GetUserData()).GetPosition().x < 0)
			{
				ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).qPosition = -27;
			}

			else
			{
				ECS::GetComponent<Health>((int)fixtureB->GetBody()->GetUserData()).qPosition = 27;
			}
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
