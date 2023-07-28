#pragma once

#include "includes.hpp"
#include "Classes.hpp"
#include "Settings.hpp"
static float noclipspeed = 1.00001f;
static bool speed = false;

namespace NoClip
{
	void Hook(void)
	{
		if (Settings::Player::NoClip)
		{
			hk_World* World = (hk_World*)*(uint64_t*)(RAIDOS::World);
			if (!World)
				return;

			hk_Ped* LocalPlayer = World->LocalPlayer();
			if (!LocalPlayer)
				return;

			hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
			if (!Nav)
				return;

			Vector3 ActualPos = LocalPlayer->GetCoordinate();

			
			if (LocalPlayer->InVehicle() == true)
			{
				return;
			}

			auto player = PLAYER::PLAYER_ID();

			//if (LocalPlayer->GetHealth() < 100) return;
			/// Monter

			if ((GetAsyncKeyState)(VK_LSHIFT))
				speed = true;
			else
				speed = false;

			if (Settings::Player::NoClipSpeed_bool)
			{
				if (speed)
					noclipspeed = Settings::Player::Speed;
				else
					noclipspeed = Settings::Player::Speed;
			}
			else 
			{
				if (speed)
					noclipspeed = 1.0f;
				else
					noclipspeed = 0.1f;
			}

			//VEHICLE::GET_CLOSEST_VEHICLE(ActualPos.x, ActualPos.y, ActualPos.z, 200.0f, 0, 70);

			Nav->SetRotation({ 0, 0, 0, 0 });
			//Nav->SetRotation(0, 0, 0);

			if ((GetAsyncKeyState)(Settings::Player::ForwardHotkey) & 0x8000)
			{
				LocalPlayer->SetVelocity();

				DWORD64 addr = RAIDOS::GetCamera();
				Vector3 TPSangles = *(Vector3*)(addr + 0x03D0);
				if (TPSangles == Vector3(0, 0, 0))
				{
					TPSangles = *(Vector3*)(addr + 0x40);
				}
				Vector3 newpos = ActualPos;
				newpos.x += (TPSangles.x * noclipspeed);
				newpos.y += (TPSangles.y * noclipspeed);
				newpos.z += (TPSangles.z * noclipspeed);
				LocalPlayer->SetCoordinate(newpos);
				Nav->SetCoordinate(newpos);
			}

			if ((GetAsyncKeyState)(Settings::Player::BackwardHotkey) & 0x8000)
			{
				LocalPlayer->SetVelocity();

				DWORD64 addr = RAIDOS::GetCamera();
				Vector3 TPSangles = *(Vector3*)(addr + 0x03D0);
				if (TPSangles == Vector3(0, 0, 0))
				{
					TPSangles = *(Vector3*)(addr + 0x40);
				}
				Vector3 newpos = ActualPos;
				newpos.x -= (TPSangles.x * noclipspeed);
				newpos.y -= (TPSangles.y * noclipspeed);
				newpos.z -= (TPSangles.z * noclipspeed);
				LocalPlayer->SetCoordinate(newpos);
				Nav->SetCoordinate(newpos);
			}
		}
	}
}



