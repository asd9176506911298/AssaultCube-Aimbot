#include <Windows.h>
#include "offsets.h"
#include "Player.h"
#include "Vector3.h"

int* Player::GetMaxPlayer()
{
	static uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
	return (int*)(moduleBase + 0x10f500);
}

Player* Player::GetPlayer(int index)
{
	static uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
	static uintptr_t entityList = moduleBase + 0x10f4f8;
	return (Player*)(*(uintptr_t*)(entityList) + index * 0x4);
}

int* Player::GetHealth()
{
	return (int*)(*(uintptr_t*)this + health);
}

int* Player::GetTeam()
{
	return (int*)(*(uintptr_t*)this + team);
}

Vector3* Player::GetOrigin()
{
	return (Vector3*)(*(uintptr_t*)this + pos);
}

Vector3* Player::GetViewOffset()
{
	return (Vector3*)(*(uintptr_t*)this + headPos);
}