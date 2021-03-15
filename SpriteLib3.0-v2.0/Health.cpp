#include "Health.h"
#include <iostream>
using namespace::std;

void Health::reduceHealth(int damage)
{
	health -= damage;
	cout << "Health reduced by " << damage << ", Health now " << health << endl;
}

double Health::GetHealth()
{
	return health;
}
