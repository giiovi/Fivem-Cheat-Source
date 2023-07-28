#pragma once  

#include "includes.hpp"
#include "Classes.hpp"
#include "native.hpp"
namespace Players
{
	void Hook(void)
	{
		hk_World* World = (hk_World*)*(uint64_t*)(RAIDOS::World);
		if (!World)
			return;

		hk_Ped* LocalPlayer = World->LocalPlayer();
		if (!LocalPlayer)
			return;
		hk_PlayerInfo* PlayerInfo = LocalPlayer->PlayerInfo();
		if (!PlayerInfo)
			return;

		hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
		if (!LocalPlayer)
			return;

		if (Settings::Player::Semi_Godmod)
		{
			if (LocalPlayer->GetHealth() < 35)
				LocalPlayer->SetMaxHealth();
		}
		if (Settings::Player::Infinite_Armor)
			if (LocalPlayer->GetArmor() < 35)
				LocalPlayer->SetArmor(Settings::Player::ArmorToSet);

		if (Settings::Player::Gravity){
			NETWORK::NETWORK_SET_IN_FREE_CAM_MODE(Settings::Player::Gravity);
		}
		if (Settings::Player::AntiHS)
		{
			auto player = PLAYER::PLAYER_PED_ID(); //dtc
			PED::SET_PED_SUFFERS_CRITICAL_HITS(player, false); //dtc
		}
		else
		{
			auto player = PLAYER::PLAYER_PED_ID(); //dtc
			PED::SET_PED_SUFFERS_CRITICAL_HITS(player, true); //dtc
		}

		if (Settings::Player::Invisible)
		{
			auto player = PLAYER::PLAYER_PED_ID(); //dtc
			ENTITY::SET_ENTITY_VISIBLE(player, false, 0); //LocalPlayer->SetInvisible(0X1);
		}
		else
		{
			auto player = PLAYER::PLAYER_PED_ID(); //dtc
			ENTITY::SET_ENTITY_VISIBLE(player, true, 0); //LocalPlayer->SetInvisible(0X37);
		}

		if (Settings::Player::Godmod)
			LocalPlayer->SetGoodmode(true);
		else
			LocalPlayer->SetGoodmode(false);

		if (Settings::Player::NoRagDoll || LocalPlayer->InVehicle())
			LocalPlayer->Set_Ragdoll(Settings::Player::NoRagDoll); 

		if (Settings::Player::fastrun)
			PlayerInfo->SetSpeed(Settings::Player::RunSpeedToSet);
		else
			PlayerInfo->SetSpeed(1);

		if (Settings::Player::EnableFovChange)
		{
			DWORD64 addr = RAIDOS::GetCamera();
			uintptr_t CameraData = *(uintptr_t*)(addr + 0x10);
			*(float*)(CameraData + 0x30) = Settings::Player::FovValue;
			Settings::Player::EnableFovChange = false;
		}		
		if (Settings::Player::TpToWaypoint)
		{
			auto player = PLAYER::PLAYER_PED_ID(); //dtc

			int WaypointHandle = HUD::GET_FIRST_BLIP_INFO_ID(8); //dtc
			if (HUD::DOES_BLIP_EXIST(WaypointHandle)) //dtc
			{
				Vector3 waypoint1 = HUD::GET_BLIP_COORDS(WaypointHandle); //dtc

				PED::SET_PED_COORDS_KEEP_VEHICLE(player, waypoint1.x, waypoint1.y, waypoint1.z); //dtc
			}
			Settings::Player::TpToWaypoint = false;
		}
		if (Settings::Player::SuperJump)
		{
			auto player = PLAYER::PLAYER_PED_ID(); //dtc

			TASK::TASK_JUMP(player, false, true, Settings::Player::SuperJump);
		}
		if (Settings::Player::SetHealth)
		{
			LocalPlayer->SetHealth(100 + Settings::Player::HealthToSet);
			Settings::Player::SetHealth = false;
		}
		if (Settings::Player::SetArmor)
		{
			LocalPlayer->SetArmor(Settings::Player::ArmorToSet);
			Settings::Player::SetArmor = false;
		}
		if ((GetAsyncKeyState)(Settings::Player::HotkeyHealth) & 0x8000)
		{
			Settings::Player::SetHealth = true;
		}
		if ((GetAsyncKeyState)(Settings::Player::HotkeyArmor) & 0x8000)
		{
			Settings::Player::SetArmor = true;
		}
		if ((GetAsyncKeyState(Settings::misc::NoclipKey) & 1)) Settings::Player::NoClip = !Settings::Player::NoClip;

		if ((GetAsyncKeyState(Settings::misc::PanicKey) & 1)) Settings::misc::isPanic = !Settings::misc::isPanic;
	}
}