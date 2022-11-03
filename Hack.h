#pragma once
#include <Windows.h>
#include "Player.h"
#include "LocalPlayer.h"
#include <iostream>



Player* GetClosestEnemy()
{
	LocalPlayer* localPlayer = LocalPlayer::Get();
	
	float closestDistance = 100000;
	int closesDistanceIndex = -1;

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
	int* mode = (int*)(moduleBase + 0x10F49C);

	for (int i = 1; i < *Player::GetMaxPlayer(); i++) 
	{
		Player* currentPlayer = Player::GetPlayer(i);

		if (!LocalPlayer::Get()->IsVisible(currentPlayer))
		{
			continue;
		}

		if (!currentPlayer || !(*(uintptr_t*)currentPlayer) || (uintptr_t)currentPlayer == (uintptr_t)localPlayer)
		{
			continue;
		}

		if (*mode == 7) 
		{
			if (*currentPlayer->GetTeam() == *localPlayer->GetTeam())
			{
				continue;
			}
		}
		

		if (*currentPlayer->GetHealth() < 1 || *localPlayer->GetHealth() < 1)
		{
			continue;
		}

		float currentDistance = localPlayer->GetDistance(currentPlayer->GetOrigin());

		if (currentDistance < closestDistance)
		{
			closestDistance = currentDistance;
			closesDistanceIndex = i;
		}
	}

	if (closesDistanceIndex == -1)
	{
		return NULL;
	}
	return Player::GetPlayer(closesDistanceIndex);
}

void Run()
{
	Player* closestEnemy = GetClosestEnemy();

	if (closestEnemy)
	{
		if (LocalPlayer::Get()->IsVisible(closestEnemy))
		{
			LocalPlayer::Get()->AimAt(closestEnemy->GetViewOffset());
		}
	}
}