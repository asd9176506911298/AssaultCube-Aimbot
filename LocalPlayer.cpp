#include <Windows.h>
#include <math.h>
#include "LocalPlayer.h"
#include "Player.h"
#include "offsets.h"


typedef Player* (__cdecl* tGetCrossHairEnt)();
uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
tGetCrossHairEnt GetCrossHairEnt = tGetCrossHairEnt(moduleBase + 0x607C0);

LocalPlayer* LocalPlayer::Get()
{
	static uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
	static LocalPlayer* localPlayer = (LocalPlayer*)(moduleBase + 0x109B74);
	return localPlayer;
}

int* LocalPlayer::GetHealth()
{
	return (int*)(*(uintptr_t*)this + health);
}

int* LocalPlayer::GetTeam()
{
	return (int*)(*(uintptr_t*)this + team);
}

Vector3* LocalPlayer::GetOrigin()
{
	return (Vector3*)(*(uintptr_t*)this + pos);
}

Vector3* LocalPlayer::GetViewOffset()
{
	return (Vector3*)(*(uintptr_t*)this + headPos);
}

double PI = 3.14159265358;

void LocalPlayer::AimAt(Player* targetEnemy)
{
	static LocalPlayer* localPlayer = Get();
	static Vector3* viewAngles = (Vector3*)(*(uintptr_t*)localPlayer + viewAngle);
	Vector3* target = targetEnemy->GetHead();
	if (target->x == -1.0f && target->y == -1.0f && target->z == -1.0f)
	{
		target = targetEnemy->GetViewOffset();
	}
	//target = targetEnemy->GetViewOffset();
	Vector3* myPos = GetViewOffset();
	
	Vector3 deltaVec = { target->x - myPos->x, target->y - myPos->y, target->z - myPos->z };
	float deltaVecLength = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);

	float pitch = asin(deltaVec.z / deltaVecLength) * (180 / PI);
	float yaw = atan2f(deltaVec.y, deltaVec.x) * (180 / PI);
	//std::cout << atan2f(deltaVec.y, deltaVec.x) << std::endl;
	//std::cout << yaw + 90 << std::endl;
	viewAngles->x = yaw + 90;
	viewAngles->y = pitch;
	Shoot();
}

float LocalPlayer::GetDistance(Vector3* other)
{
	Vector3* myPos = GetOrigin();
	Vector3 delta = Vector3(other->x - myPos->x, other->y - myPos->y, other->z - myPos->z);

	return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

bool LocalPlayer::TriggerBot()
{
	Player* CrossHairEnnt = GetCrossHairEnt();

	if (CrossHairEnnt)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void LocalPlayer::Shoot()
{
	if(TriggerBot())
	{
		*(int*)(*(uintptr_t*)this + attack) = 1;
		Sleep(1);
		*(int*)(*(uintptr_t*)this + attack) = 0;
	}
	else
	{
		*(int*)(*(uintptr_t*)this + attack) = 0;
	}
}

struct traceresult_s
{
	Vector3 end;
	bool collided;
};

bool LocalPlayer::IsVisible(Player* ent)
{
	DWORD traceLine = 0x48A310;
	traceresult_s traceresult;
	traceresult.collided = false;

	//LocalPlayer localPlayer = *LocalPlayer::Get();

	Vector3 from = *LocalPlayer::Get()->GetViewOffset();
	Vector3 to = *ent->GetViewOffset();

	
	__asm
	{
		push 0
		push 0
		push this
		push to.z
		push to.y
		push to.x
		push from.z
		push from.y
		push from.x
		lea eax, [traceresult]
		call traceLine
		add esp, 36
	}

	return !traceresult.collided;
}

