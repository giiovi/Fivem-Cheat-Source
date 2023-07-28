#pragma once

#include "includes.hpp"
#include "Settings.hpp"
#include "native.hpp"
#include "func.hpp"
bool AimbotCheck(DWORD64 pPed, DWORD64 Peddd) {
	auto handle = RAIDOS::pointer_to_handle(pPed);
	auto handle1 = RAIDOS::pointer_to_handle(Peddd);

	if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(handle1, handle, 17)) {

		return true;
	}
	return false;
}

namespace Aimbot
{
	uintptr_t GetDistancePlayer()
	{
		uintptr_t ClosestPlayer = 0;
		float closestdist = FLT_MAX;

		float dist;

		hk_World* World = (hk_World*)*(uint64_t*)(RAIDOS::World);
		if (!World) return NULL;

		hk_Ped* LocalPlayer = World->LocalPlayer();
		if (!LocalPlayer) return NULL;

		hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(RAIDOS::ReplayInterface);
		if (!ReplayInterface) return NULL;

		hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
		if (!PedInterface) return NULL;

		for (uint64_t pid = 0; pid < PedInterface->PedMaximum(); pid++)
		{
			hk_Ped* Peds = PedInterface->PedList()->Ped(pid);
			float PedLocX = Peds->GetCoordinate().x;
			float PedLocY = Peds->GetCoordinate().y;
			float PedLocZ = Peds->GetCoordinate().z;
			//float PedLocX = PedInterface->PedList()->Ped(pid)->GetCoordinate().x;
			//float PedLocY = PedInterface->PedList()->Ped(pid)->GetCoordinate().y;
			//float PedLocZ = PedInterface->PedList()->Ped(pid)->GetCoordinate().z;

			if (!Peds) continue;

			if (PedLocX == 0.0f || PedLocY == 0.0f || PedLocZ == 0.0f) continue;

			if (Peds == LocalPlayer || Peds->GetHealth() <= 0) continue;

			if (Settings::Aimbot::TypeOfTarget == 1)
			{
				auto ped_type = Peds->GetPedType();
				if (!ped_type) continue;
				ped_type = ped_type << 11 >> 25;

				if (ped_type != 2) {
					continue;
				}
			}
			else if (Settings::Aimbot::TypeOfTarget == 2)
			{
				auto ped_type = Peds->GetPedType();
				if (!ped_type) continue;

				ped_type = ped_type << 11 >> 25;


				if (ped_type == 2) {
					continue;
				}
			}

			if (Settings::Aimbot::VisibleCheck)
			{
				if (AimbotCheck((DWORD64)LocalPlayer, (DWORD64)Peds))
				{
				}
				else
				{
					continue;
				}
			}

			Vector3 PedLoc = Peds->GetCoordinate();
			Vector3 LocLoc = LocalPlayer->GetCoordinate();

			float Distance = Drawing::Distance(LocLoc, PedLoc);

			auto it = std::find(RAIDOS::Friend.begin(), RAIDOS::Friend.end(), GetPlayerNameNew((DWORD64)Peds));
			if (it != RAIDOS::Friend.end())
				continue;

			if (Distance <= Settings::Aimbot::DistanceAimbotLimit)
			{
				ImVec2 HEAD = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::HEAD));

				ImVec2 ScreenSize = ImVec2(RAIDOS::WindowSize.x / 2, RAIDOS::WindowSize.y / 2);
				dist = RAIDOS::pythag(ScreenSize, HEAD);

				if (dist < closestdist)
				{
					closestdist = dist;
					ClosestPlayer = reinterpret_cast<uintptr_t>(Peds);
				}
			}


		}

		return ClosestPlayer;
	}

	Vector3 ToVector3(Vector3 pVec)
	{
		return Vector3(pVec.x, pVec.y, pVec.z);
	}

	void do_aimbot(uint64_t entity)
	{ // pretty buggy, needs playing around with sensitivity
		auto bone_pos = Drawing::GetBonePosW2S(entity, NULL);

		auto center_x = RAIDOS::SizeX / 2.f;
		auto center_y = RAIDOS::SizeY / 2.f;
	
		switch (Settings::Aimbot::AimbotBone)
		{
		case 0:
			bone_pos = Drawing::GetBonePosW2S(entity, static_cast<int>(RAIDOS::Bone::HEAD));
			break;
		case 1:
			bone_pos = Drawing::GetBonePosW2S(entity, static_cast<int>(RAIDOS::Bone::NECK));
			break;
		case 2:
			bone_pos = Drawing::GetBonePosW2S(entity, static_cast<int>(RAIDOS::Bone::ABDOMEN));
			break;
		}

		float ScreenLocX = bone_pos.x - center_x;
		float ScreenLocY = bone_pos.y - center_y;

		float AimLocX = ScreenLocX /= Settings::Aimbot::AimbotSmooth + 8.0f; //1.5f
		float AimLocY = ScreenLocY /= Settings::Aimbot::AimbotSmooth + 8.0f; //1.5f

		float TargetX = 0.f;
		float TargetY = 0.f;

		if (AimLocX > center_x)
		{
			TargetX = -(center_x - AimLocX);
			if (TargetX + center_x > center_x * 2) TargetX = 0;
		}
		else
		{
			TargetX = AimLocX - center_x;
			if (TargetX + center_x < 0) TargetX = 0;
		}

		if (AimLocY > center_y)
		{
			TargetY = -(center_y - AimLocY);
			if (TargetY + center_y > center_y * 2) TargetY = 0;
		}
		else
		{
			TargetY = AimLocY - center_y;
			if (TargetY + center_y < 0) TargetY = 0;
		}

		ImVec2 ScreenMiddle = ImVec2(RAIDOS::SizeX / 2, RAIDOS::SizeY / 2);

		if (RAIDOS::pythag(ScreenMiddle, bone_pos) <= Settings::Aimbot::AimbotFov * 10)
		{
			mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(AimLocX), static_cast<DWORD>(AimLocY), 0, 0);
		}
	}

	void memoryAim(Vector3 point)
	{
		hk_World* World = (hk_World*)*(uint64_t*)(RAIDOS::World);
		if (!World) return;

		hk_Ped* LocalPlayer = World->LocalPlayer();
		if (!LocalPlayer) return;

		auto draw_list = ImGui::GetBackgroundDrawList();
		DWORD64 addr = RAIDOS::GetCamera();
		if (addr)
		{
			Vector3 CrosshairPos = *(Vector3*)(addr + 0x60);
			Vector3 caca(CrosshairPos.x - point.x, CrosshairPos.y - point.y, CrosshairPos.z - point.z);
			float distance = caca.Length();

			__try
			{
				uint64_t CamData = *(DWORD64*)(addr + 0x10);
				if (CamData != NULL)
				{

					if (*(float*)(CamData + 0x2AC) == -2)
					{
						*(float*)(CamData + 0x2AC) = 0.0f;
						*(float*)(CamData + 0x2C0) = 111.0f;
						*(float*)(CamData + 0x2C4) = 111.0f;
					}

					if (*(float*)(CamData + 0x130) == 8.0f)
					{
						*(float*)(CamData + 0x130) = 111.0f;
						*(float*)(CamData + 0x134) = 111.0f;
						*(float*)(CamData + 0x4CC) = 0.0f;

						if (*(float*)(CamData + 0x49C) == 1.0f)
							*(float*)(CamData + 0x49C) = 0.0f;

						*(float*)(CamData + 0x2AC) = 0.0f;
						*(float*)(CamData + 0x2B0) = 0.0f;
					}
				}
			}
			__except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}

			float threshold = 0.5f / (Settings::Aimbot::AimbotSmooth * 20);
			Vector3 Out = Vector3((point.x - CrosshairPos.x) / distance, (point.y - CrosshairPos.y) / distance, (point.z - CrosshairPos.z) / distance);

			if (Settings::Aimbot::AimbotSmooth <= 1)
			{
				*(Vector3*)(addr + 0x40) = Out;  //FPS
				*(Vector3*)(addr + 0x3D0) = Out; //TPS
			}			
			else
			{
				//First Person
				{
					Vector3 angles = *(Vector3*)(addr + 0x40);
					if (((Out.x - angles.x) > threshold) || ((angles.x - Out.x) > threshold))
					{
						if (angles.x > Out.x)
							*(float*)(addr + 0x40 + 0x0) -= threshold;
						else if (angles.x < Out.x)
							*(float*)(addr + 0x40 + 0x0) += threshold;
					}
					if (((Out.y - angles.y) > threshold) || ((angles.y - Out.y) > threshold))
					{
						if (angles.y > Out.y)
							*(float*)(addr + 0x40 + 0x4) -= threshold;
						else if (angles.y < Out.y)
							*(float*)(addr + 0x40 + 0x4) += threshold;
					}
					if (((Out.z - angles.z) > threshold) || ((angles.z - Out.z) > threshold))
					{
						if (angles.z > Out.z)
							*(float*)(addr + 0x40 + 0x8) -= threshold;
						else if (angles.z < Out.z)
							*(float*)(addr + 0x40 + 0x8) += threshold;
					}
				}

				//Third Person
				{
					Vector3 angles = *(Vector3*)(addr + 0x3D0);
					if (((Out.x - angles.x) > threshold) || ((angles.x - Out.x) > threshold))
					{
						if (angles.x > Out.x)
							*(float*)(addr + 0x3D0 + 0x0) -= threshold;
						else if (angles.x < Out.x)
							*(float*)(addr + 0x3D0 + 0x0) += threshold;
					}
					if (((Out.y - angles.y) > threshold) || ((angles.y - Out.y) > threshold))
					{
						if (angles.y > Out.y)
							*(float*)(addr + 0x3D0 + 0x4) -= threshold;
						else if (angles.y < Out.y)
							*(float*)(addr + 0x3D0 + 0x4) += threshold;
					}
					if (((Out.z - angles.z) > threshold) || ((angles.z - Out.z) > threshold))
					{
						if (angles.z > Out.z)
							*(float*)(addr + 0x3D0 + 0x8) -= threshold;
						else if (angles.z < Out.z)
							*(float*)(addr + 0x3D0 + 0x8) += threshold;
					}
				}
			}
			
		}
	}
	
	void magicbullet()
	{
		hk_World* World = (hk_World*)*(uint64_t*)(RAIDOS::World);
		if (!World) return;

		hk_Ped* LocalPlayer = World->LocalPlayer();
		if (!LocalPlayer) return;

		hk_WeaponManager* PlayerWeaponManager = LocalPlayer->WeaponManager();
		if (!PlayerWeaponManager) return;

		hk_WeaponInfo* PlayerWeaponInfo = PlayerWeaponManager->WeaponInfo();
		if (!PlayerWeaponInfo) return;
		
		uint64_t WeaponHash = PlayerWeaponInfo->GetHash();

		hk_Ped* PedC = (hk_Ped*)GetDistancePlayer();
		Vector3 PedLoc = PedC->GetCoordinate();

		auto player = PLAYER::PLAYER_PED_ID();
		if (!player) return;

		if (PED::IS_PED_SHOOTING(player))
		{
			MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(
				PedLoc.x, 
				PedLoc.y, 
				PedLoc.z, 
				PedLoc.x, 
				PedLoc.y, 
				PedLoc.z, 
				Settings::Aimbot::MagicDamage, 
				false, 
				WeaponHash, 
				player, 
				true, 
				false, 
				3.f
			);
		}
	}

	void Hook(void)
	{
		if (Settings::Aimbot::Aimbot)
		{
		
			if ((GetAsyncKeyState)(Settings::Aimbot::Hotkey) & 0x8000)
			{
				uint64_t entity = GetDistancePlayer();
				if (!entity) return;

				auto center_x = RAIDOS::SizeX / 2.f;
				auto center_y = RAIDOS::SizeY / 2.f;

				auto get_distance = [](double x1, double y1, double x2, double y2) {
					return sqrtf(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
				};

				auto bone_pos = Drawing::GetBonePos(entity, NULL);

				switch (Settings::Aimbot::AimbotBone)
				{
				case 0:
					bone_pos = Drawing::GetBonePos(entity, static_cast<int>(RAIDOS::Bone::HEAD));
					break;
				case 1:
					bone_pos = Drawing::GetBonePos(entity, static_cast<int>(RAIDOS::Bone::NECK));
					break;
				case 2:
					bone_pos = Drawing::GetBonePos(entity, static_cast<int>(RAIDOS::Bone::ABDOMEN));
					break;
				}

				ImVec2 screen = Drawing::WorldToScreen(bone_pos);

				auto fov = get_distance(center_x, center_y, screen.x, screen.y);

				//if (Drawing::in_fov(center_x, center_y, aimbot::fov, target.x, target.y) && screen_distance < best_aimbot_distance)
				//{
					//best_aimbot_distance = screen_distance;

					switch (Settings::Aimbot::AimbotTypes)
					{
					case 0: //mouse Aim - buggy
						if (!entity) return;
						do_aimbot(entity);
						break;
					case 1: //memory Aim - car Aim problem
						if (fov < Settings::Aimbot::AimbotFov * 10)
							memoryAim(bone_pos);
						break;
					case 2: //magic bullet
						if (fov < Settings::Aimbot::AimbotFov * 10)
							magicbullet();
						break;
					}
				//}
			}

		}
	}
}