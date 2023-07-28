#pragma once

#include "includes.hpp"
#include <sstream>

#include "native.hpp"
#include "func.hpp"
#include <fa_solid_900.h>

std::string GetPlayerNameNew(DWORD64 pPed) 
{
	auto handle = RAIDOS::pointer_to_handle(pPed);
	if (handle > 1) {
		int NetworkGetPlayerIndex = NETWORK::NETWORK_GET_PLAYER_INDEX_FROM_PED(handle); //dtc
		if (NetworkGetPlayerIndex > 1)
			return std::string(PLAYER::GET_PLAYER_NAME(NetworkGetPlayerIndex)); //dtc
			//return std::string(Get_player_name(NetworkGetPlayerIndex));
	}
	
	return "Peds";
	
}

bool EspCheck(DWORD64 pPed, DWORD64 Peddd) {
	auto handle = RAIDOS::pointer_to_handle(pPed);
	auto handle1 = RAIDOS::pointer_to_handle(Peddd);
	if (ENTITY::_HAS_ENTITY_CLEAR_LOS_TO_ENTITY_2(handle1, handle, 17)) {

		return true;
	}
	return false;
}

auto DrawOutlinedText(ImFont* font, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center) -> VOID
{
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int index = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = font->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			//ImGui::GetBackgroundDrawList()->AddText(font, size + 1, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * index)), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{
			//ImGui::GetBackgroundDrawList()->AddText(font, size + 1, ImVec2((pos.x) + 1, (pos.y + textSize.y * index)), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(pos.x, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (index + 1);
		index++;
	}
}

static hk_Ped* selected = 0;
static int selected_ped = 1;
static int selected_veh = 1;
static int selected_resource = 1;

namespace Visuals
{
	void Hook(void)
	{
		if (Settings::Visuals::visualsenable)
		{
			hk_World* World = (hk_World*)*(uint64_t*)(RAIDOS::World);
			if (!World)	return;

			hk_Ped* LocalPlayer = World->LocalPlayer();
			if (!LocalPlayer) return;

			hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(RAIDOS::ReplayInterface);
			if (!ReplayInterface) return;

			hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
			if (!PedInterface) return;

			ImDrawList* drawlist = ImGui::GetBackgroundDrawList();
		
			for (uint64_t i = 0; i < PedInterface->PedMaximum(); i++)
			{
				hk_Ped* Peds = PedInterface->PedList()->Ped(i);
				if (!Peds) continue;

				float PedLocX = PedInterface->PedList()->Ped(i)->GetCoordinate().x;
				float PedLocY = PedInterface->PedList()->Ped(i)->GetCoordinate().y;
				float PedLocZ = PedInterface->PedList()->Ped(i)->GetCoordinate().z;

				if (PedLocX == 0.0f || PedLocY == 0.0f || PedLocZ == 0.0f) continue;

				auto ped_type = Peds->GetPedType();
				if (!ped_type || Peds->GetHealth() <= 0) continue;

				if (!Settings::Visuals::Draw_LocalPlayer && Peds == LocalPlayer) continue;
				//if (!Settings::Visuals::Draw_Dead && Peds->GetHealth() <= 0) continue;
				
				auto playerName = GetPlayerNameNew((DWORD64)Peds);

				if (Settings::Visuals::IgnorePlayers && playerName != "Peds") continue;

				if (!Settings::Visuals::Draw_Ped && playerName == "Peds") continue;

				ImVec2 Ped_Location = Drawing::WorldToScreen(Peds->GetCoordinate());
				ImVec2 LocalPlayer_Location = Drawing::WorldToScreen(LocalPlayer->GetCoordinate());

				Vector3 PedLoc = Peds->GetCoordinate();
				Vector3 LocLoc = LocalPlayer->GetCoordinate();

				float Distance = Drawing::Distance(LocLoc, PedLoc);

				if (Distance < Settings::Visuals::Distance)
				{		
					ImVec2 Head = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::HEAD));
					ImVec2 Left_Foot = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::L_FOOT));
					ImVec2 Right_Foot = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::R_FOOT));

					if (!IsOnScreen(Head) || !IsOnScreen(Right_Foot) || !IsOnScreen(Right_Foot)) continue;

					float Foot_Middle = (Left_Foot.y + Right_Foot.y) / 2.f;
					float Height = abs(Head.y - Foot_Middle) * 1.37f;
					float Width = (Height / 1.80f);
					float Width2 = (Height / 1.80f);
					if (Settings::Visuals::HealthBar == true)
					{
						float Width2 = (Height / 2.00f);
					}
					float HealthDistance = Width / 16;


					if (Settings::Visuals::Snapline)
					{
						drawlist->AddLine(ImVec2(RAIDOS::SizeX / 2, RAIDOS::SizeY / 2), Ped_Location, ImColor(255, 255, 255, 255), 1.f);
					}

					if (Settings::Visuals::Box)
					{
						ImColor caca = { 0, 0, 0 ,255 };

						if (EspCheck((DWORD64)LocalPlayer, (DWORD64)Peds))
						{
							caca = Settings::Visuals::BoxColor;
						}
						else
						{
							caca = Settings::Visuals::BoxColorVisible;
						}

						drawlist->AddRect(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImColor(0, 0, 0, 255), 0, 0, 0.9 * Settings::Visuals::BoxSize);

						drawlist->AddRect(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), caca, 0, 0, 0.5 * Settings::Visuals::BoxSize);

					}
					/*
					if (Settings::Visuals::Corner){
						ImColor caca = { 0, 0, 0 ,255 };
						if (IsVisible((DWORD64)LocalPlayer, (DWORD64)Peds))
						{
							caca = Settings::Visuals::CornerColor;
						}
						else
						{
							caca = Settings::Visuals::CornerColorVisible;
						}

						float CornerSize = (Width) / 4.f;

						drawlist->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) + CornerSize), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);
						drawlist->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2((Ped_Location.x - Width / 2) + CornerSize, Ped_Location.y - Height / 2), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);

						drawlist->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, (Ped_Location.y - Height / 2) + CornerSize), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);
						drawlist->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y - Height / 2), ImVec2((Ped_Location.x + Width / 2) - CornerSize, Ped_Location.y - Height / 2), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);

						drawlist->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y + Height / 2), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y + Height / 2) - CornerSize), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);
						drawlist->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y + Height / 2), ImVec2((Ped_Location.x - Width / 2) + CornerSize, Ped_Location.y + Height / 2), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);

						drawlist->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImVec2(Ped_Location.x + Width / 2, (Ped_Location.y + Height / 2) - CornerSize), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);
						drawlist->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImVec2((Ped_Location.x + Width / 2) - CornerSize, Ped_Location.y + Height / 2), ImColor(0, 0, 0, 255), 1.8 * Settings::Visuals::CornerSize);

						drawlist->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) + CornerSize), caca, 1 * Settings::Visuals::CornerSize);
						drawlist->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2((Ped_Location.x - Width / 2) + CornerSize, Ped_Location.y - Height / 2), caca, 1 * Settings::Visuals::CornerSize);

						drawlist->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, (Ped_Location.y - Height / 2) + CornerSize), caca, 1 * Settings::Visuals::CornerSize);
						drawlist->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y - Height / 2), ImVec2((Ped_Location.x + Width / 2) - CornerSize, Ped_Location.y - Height / 2), caca, 1 * Settings::Visuals::CornerSize);

						drawlist->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y + Height / 2), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y + Height / 2) - CornerSize), caca, 1 * Settings::Visuals::CornerSize);
						drawlist->AddLine(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y + Height / 2), ImVec2((Ped_Location.x - Width / 2) + CornerSize, Ped_Location.y + Height / 2), caca, 1 * Settings::Visuals::CornerSize);

						drawlist->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImVec2(Ped_Location.x + Width / 2, (Ped_Location.y + Height / 2) - CornerSize), caca, 1 * Settings::Visuals::CornerSize);
						drawlist->AddLine(ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImVec2((Ped_Location.x + Width / 2) - CornerSize, Ped_Location.y + Height / 2), caca, 1 * Settings::Visuals::CornerSize);
					}
					*/

					if (Settings::Visuals::Skeleton)
					{
						ImVec2 L_Foot = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::L_FOOT));
						ImVec2 R_Foot = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::R_FOOT));
						ImVec2 L_HAND = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::L_HAND));
						ImVec2 R_HAND = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::R_HAND));
						ImVec2 NECK = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::NECK)); //Boyun
						ImVec2 ABDOMEN = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::ABDOMEN)); //Karýn

						if (!IsOnScreen(NECK) || !IsOnScreen(ABDOMEN) || !IsOnScreen(R_HAND) || !IsOnScreen(L_HAND) || !IsOnScreen(R_Foot) || !IsOnScreen(L_Foot))
						{
						}
						else
						{
							float bone_thickness = Settings::Visuals::BoneThick;

							ImColor caca = { 0, 0, 0 ,255 };
							if (EspCheck((DWORD64)LocalPlayer, (DWORD64)Peds))
							{
								caca = Settings::Visuals::SkelColor;
							}
							else
							{
								caca = Settings::Visuals::SkelColorVisible;
							}

							drawlist->AddLine(NECK, R_HAND, caca, bone_thickness);
							drawlist->AddLine(NECK, L_HAND, caca, bone_thickness);

							drawlist->AddLine(NECK, ABDOMEN, caca, bone_thickness);

							drawlist->AddLine(ABDOMEN, R_Foot, caca, bone_thickness);
							drawlist->AddLine(ABDOMEN, L_Foot, caca, bone_thickness);
						}

						/*
						ImVec2 Pelvis = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::PELVIS));
						ImVec2 Neck = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::NECK));
						ImVec2 Uperarm_left = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::L_UPPERARM));
						ImVec2 Uperarm_right = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::R_UPPERARM));
						ImVec2 Elbow_l = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::L_ELBOW));
						ImVec2 Elbow_r = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::R_ELBOW));
						ImVec2 SKEL_R_Hand = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::R_HAND));
						ImVec2 SKEL_L_Hand = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::L_HAND));
						ImVec2 SKEL_L_Thigh = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::L_THIGH));
						ImVec2 SKEL_R_Thigh = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::R_THIGH));
						ImVec2 SKEL_L_Calf = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::L_CALF));
						ImVec2 SKEL_R_Calf = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::R_CALF));
						ImVec2 SKEL_L_Foot = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::L_FOOT));
						ImVec2 SKEL_R_Foot = Drawing::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), static_cast<int>(RAIDOS::Bone::R_FOOT));

						if (!IsOnScreen(Pelvis) || !IsOnScreen(Neck) || !IsOnScreen(Uperarm_left) || !IsOnScreen(Uperarm_right) || !IsOnScreen(Elbow_l) || !IsOnScreen(Elbow_r) || !IsOnScreen(SKEL_R_Hand) || !IsOnScreen(SKEL_L_Hand) || !IsOnScreen(SKEL_R_Thigh) || !IsOnScreen(SKEL_L_Thigh) || !IsOnScreen(SKEL_R_Calf) || !IsOnScreen(SKEL_L_Calf)|| !IsOnScreen(SKEL_R_Foot) || !IsOnScreen(SKEL_L_Foot))
						{
						}
						else
						{
							float bone_thickness = Settings::Visuals::BoneThick;

							ImColor caca = { 0, 0, 0 ,255 };
							if (EspCheck((DWORD64)LocalPlayer, (DWORD64)Peds))
							{
								caca = Settings::Visuals::SkelColor;
							}
							else
							{
								caca = Settings::Visuals::SkelColorVisible;
							}

							drawlist->AddLine(Neck, Uperarm_right, caca, bone_thickness);
							drawlist->AddLine(Neck, Uperarm_left, caca, bone_thickness);

							drawlist->AddLine(Uperarm_right, Elbow_r, caca, bone_thickness);
							drawlist->AddLine(Uperarm_left, Elbow_l, caca, bone_thickness);

							drawlist->AddLine(Elbow_r, SKEL_R_Hand, caca, bone_thickness);
							drawlist->AddLine(Elbow_l, SKEL_L_Hand, caca, bone_thickness);

							drawlist->AddLine(Neck, Pelvis, caca, bone_thickness);

							drawlist->AddLine(Pelvis, SKEL_L_Thigh, caca, bone_thickness);
							drawlist->AddLine(Pelvis, SKEL_R_Thigh, caca, bone_thickness);

							drawlist->AddLine(SKEL_L_Thigh, SKEL_L_Calf, caca, bone_thickness);
							drawlist->AddLine(SKEL_R_Thigh, SKEL_R_Calf, caca, bone_thickness);

							drawlist->AddLine(SKEL_L_Calf, SKEL_L_Foot, caca, bone_thickness);
							drawlist->AddLine(SKEL_R_Calf, SKEL_R_Foot, caca, bone_thickness);
						}
						*/
					}

					if (Settings::Visuals::HealthBar)
					{
						if (Distance <= 1) continue;

						bool IsOnGlife = false;
						if (Peds->GetMaxHealth() > 201)
							IsOnGlife = true;

						float HealthPercentage = Height / 100 * (Peds->GetHealth() - 100);
						float VerticalHealth = Height / 100 * 100;
						if (IsOnGlife)
							HealthPercentage = Height / 100 * ((Peds->GetHealth() - 100) / 2);

						float fixedhealth = Peds->GetHealth() / 2;
						if (IsOnGlife)
							(Peds->GetHealth() - 100) / 2;

						ImColor	Full = ImColor(52, 176, 48, 255);
						ImColor Dead = ImColor(0, 0, 0, 255);

						float caca = (Width) / 10.f;
						//	caca = (Width) / 10.f;

						float HorizontalHealth = Width / 100 * (Peds->GetHealth() - 100);

						if (Settings::Visuals::HealthBar_idx == 0)
						{
							if (fixedhealth != 0)
							DrawHealthBar(ImVec2((Ped_Location.x + Width / 2) + HealthDistance, Ped_Location.y + Height / 2), ImVec2(2, VerticalHealth), Dead);
							DrawHealthBar(ImVec2((Ped_Location.x + Width / 2) + HealthDistance, Ped_Location.y + Height / 2), ImVec2(1.8, HealthPercentage), Full);
						}
						else if (Settings::Visuals::HealthBar_idx == 1)
						{
							if (fixedhealth != 0)
							DrawHealthBar(ImVec2((Ped_Location.x - Width / 2) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(2, VerticalHealth), Dead);
							DrawHealthBar(ImVec2((Ped_Location.x - Width / 2) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(1.8, HealthPercentage), Full);
						}
						else if (Settings::Visuals::HealthBar_idx == 2)
						{
							if (fixedhealth != 0)
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y + Height / 2) + caca + 5), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y + Height / 2) + caca + 5), ImVec2(2, HorizontalHealth), Dead);
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y + Height / 2) + caca + 5), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y + Height / 2) + caca + 5), ImVec2(1.8, HorizontalHealth), Full);

						}
						else if (Settings::Visuals::HealthBar_idx == 3)
						{
							if (fixedhealth != 0)
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y - Height / 2) - 20), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 20), ImVec2(2, HorizontalHealth), Dead);
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y - Height / 2) - 20), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 20), ImVec2(1.8, HorizontalHealth), Full);
						}

						//DrawMultiColorHealthBar(ImVec2((Ped_Location.x + Width / 2) + HealthDistance + 2, Ped_Location.y - Height / 2), ImVec2((Ped_Location.x + Width / 2) + HealthDistance, (Ped_Location.y + Height / 2) - HealthPercentage), col, ImColor(255, 0, 0, 255));
					}
					if (Settings::Visuals::ArmorBar)
					{
						if (Distance <= 1) continue;

						float HealthPercentage = Height / 100 * (Peds->GetArmor());
						float VerticalArmor = Height / 100 * 100;

						float fixedhealth = Peds->GetArmor();

						ImColor Full = ImColor(10, 145, 255, 255);
						ImColor Dead = ImColor(0, 0, 0, 255);

						float caca = (Width) / 10.f;
						//caca = (Width) / 10.f;

						float HorizontalArmor = Width / 100 * (Peds->GetArmor());

						if (Settings::Visuals::HealthBar_idx == 0)
						{
							if (fixedhealth != 0)
							DrawHealthBar(ImVec2((Ped_Location.x + Width2 / 2) + HealthDistance, Ped_Location.y + Height / 2), ImVec2(2, VerticalArmor), Dead);
							DrawHealthBar(ImVec2((Ped_Location.x + Width2 / 2) + HealthDistance, Ped_Location.y + Height / 2), ImVec2(1.8, HealthPercentage), Full);

						}
						else if (Settings::Visuals::HealthBar_idx == 1)
						{
							if (fixedhealth != 0)
							DrawHealthBar(ImVec2((Ped_Location.x - Width2 / 2) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(2, VerticalArmor), Dead);
							DrawHealthBar(ImVec2((Ped_Location.x - Width2 / 2) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(1.8, HealthPercentage), Full);

						}
						else if (Settings::Visuals::HealthBar_idx == 2)
						{

							if (fixedhealth != 0)
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width2 / 2), (Ped_Location.y + Height / 2) + caca + 5), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y + Height / 2) + caca + 5), ImVec2(2, HorizontalArmor), Dead);
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width2 / 2), (Ped_Location.y + Height / 2) + caca + 5), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y + Height / 2) + caca + 5), ImVec2(1.8, HorizontalArmor), Full);

						}
						else if (Settings::Visuals::HealthBar_idx == 3)
						{
							if (fixedhealth != 0)
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width2 / 2), (Ped_Location.y - Height / 2) - 20), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 20), ImVec2(2, HorizontalArmor), Dead);
							DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width2 / 2), (Ped_Location.y - Height / 2) - 20), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 20), ImVec2(1.8, HorizontalArmor), Full);

						}

					}

					std::string str = "";

					if (Settings::Visuals::Weapon){
						hk_WeaponManager* PedsWeaponManager = Peds->WeaponManager();
						if (!PedsWeaponManager) continue;
						hk_WeaponInfo* PedsWeaponInfo = PedsWeaponManager->WeaponInfo();
						if (!PedsWeaponInfo) continue;
						uint64_t Hash = PedsWeaponInfo->GetHash();
						str.append(get_weapon_name(Hash));

						//DrawOutlinedText(ImGui::GetFont(), get_weapon_name(Hash), ImVec2(Ped_Location.x, Ped_Location.y + Height / 2 + 5), 15.0f, ImColor(255, 255, 255), true);
					}
					if (Settings::Visuals::DistanceESP) {


						std::string strr = " | " + std::to_string(Distance) + " m";
						if (!Settings::Visuals::Weapon)
							strr = std::to_string(Distance) + " m ";
						str.append(strr);
					}

					if (Settings::Visuals::DistanceESP || Settings::Visuals::Weapon)
					{
						DrawOutlinedText(ImGui::GetFont(), str, ImVec2(Ped_Location.x, Ped_Location.y + Height / 2 + 5), 15.0f, ImColor(255, 255, 255), true);
					}

					if (Settings::Visuals::PlayerName)
					{
						//DrawOutlinedText(ImGui::GetFont(), playerName, ImVec2(Ped_Location.x, Ped_Location.y - Height / 2 - 10), 15.0f, ImColor(255, 255, 255), true);
						//std::string NewPlayerName = playerName + " | " + get_weapon_name(Hash) + " | " + distance;

						ImVec2 TextSize = ImGui::CalcTextSize((char*)playerName.c_str());

						//drawlist->AddRectFilled(ImVec2(Ped_Location.x - (TextSize.x / 2), Ped_Location.y - Height / 2 - 5 - (TextSize.y / 2) - 1), ImVec2(Ped_Location.x + (TextSize.x / 2), Ped_Location.y - Height / 2 - 5 + (TextSize.y / 2) - 1), ImColor(0, 0, 0, 150));
						DrawOutlinedText(ImGui::GetFont(), playerName, ImVec2(Ped_Location.x, Ped_Location.y - Height / 2 - 5 - 10), 14, ImColor(255, 255, 255), true);
					}				
				}
			}

			//5 dakika aç kapat
		}

		if (Settings::vehicle::NameESP)
		{
			hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(RAIDOS::ReplayInterface);
			if (!ReplayInterface)
				return;
			hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface();
			if (!VehicleInterface)
				return;


			for (int i = 0; i < VehicleInterface->VehicleMaximum(); i++)
			{
				hk_Vehicle* Ped = VehicleInterface->VehicleList()->Vehicle(i);
				if (!Ped) return;
				ImVec2 Ped_Location = Drawing::WorldToScreen(Ped->GetCoordinate());


				auto playerName = Ped->VehicleModelInfo()->GetCharName();
				const char* c = playerName;
				const char* items[] = { c };

				std::string VehicleName = playerName;
				DrawOutlinedText(ImGui::GetFont(), VehicleName, ImVec2(Ped_Location.x, Ped_Location.y), 14.0f, ImColor(255, 255, 255), true);


			}
		}
	}
}
