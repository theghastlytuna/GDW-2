#pragma once
class Health
{
public:
	void reduceHealth(int damage);
	double GetHealth();
	int qPosition;//Queued position of entity: Where the entity is supposed to be after a movement, in game units
	int playerNum;

protected:
	double health = 100;
};