#pragma once
#include "Vector3.h"
#include "Player.h"

class LocalPlayer
{
private:
	LocalPlayer();
public:
	static LocalPlayer* Get();

	int* GetHealth();
	int* GetTeam();
	Vector3* GetOrigin();
	Vector3* GetViewOffset();
	void AimAt(Player* targetEnemy);
	float GetDistance(Vector3* other);

	void Shoot();
	bool TriggerBot();
	bool IsVisible(Player* ent);

};
