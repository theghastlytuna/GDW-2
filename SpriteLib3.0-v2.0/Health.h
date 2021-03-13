#pragma once
class Health
{
public:
	void reduceHealth(int damage);
	int qPosition;//Queued position of entity: Where the entity is supposed to be after a movement, in game units
protected:
	int health = 100;
};