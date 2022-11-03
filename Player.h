#pragma once
#include "Vector3.h"

class Player
{
private:
	Player();
public:
	static int* GetMaxPlayer();
	static Player* GetPlayer(int index);

	int* GetHealth();
	int* GetTeam();
	Vector3* GetOrigin();
	Vector3* GetViewOffset();
};
