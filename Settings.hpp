#pragma once
#include "editor.h"
#include "skCrpyt.h"

namespace Settings
{
	inline ImVec2 WindowsPosActual = ImVec2(0, 0);
	namespace Player
	{
		inline bool Godmod = false;
		inline bool Semi_Godmod = false;
		inline bool Infinite_Armor = false;
		inline bool SetHealth = false;
		inline bool SetArmor = false;
		inline bool fastrun = false; 
		inline int RunSpeedToSet = 1;

		inline int HealthToSet = 100;
		inline int ArmorToSet = 100;
		inline bool NoClip = false; 
		inline int noclipspeed = false;
		inline bool Invisible = false;
		inline bool AntiHS = false;
		inline int HotkeyHealth = 0x79;
		inline int HotkeyArmor = 0x7A;
		inline int ForwardHotkey = 87;
		inline int BackwardHotkey = 83;

		inline bool EnableFovChange = false;
		inline bool TpToWaypoint = false;
		inline bool NoRagDoll = false;
		inline float FovValue = 50;
		inline bool NoClipSpeed_bool = false;
		inline int Speed = 0;
		inline bool Gravity = false;
		inline bool SuperJump = false;

	}
	namespace Weapon
	{
		inline bool weaponenable = false;

		static int weapon_current = 0;
		inline const char* weapon_list[]{ "Dagger", "Bat", "Knife", "Machete", "Pistol", "Pistol MK2", "Combat Pistol", "AP Pistol", "Stungun", "Pistol 50", "SNS PISTOL", "SNS Pistol MK2", "Heavy Pistol", "Vintage Pisol", "Flare Gun", "Marksman Pistol", "Revolver", "Revolver MK2", "Double Action", "Micro Smg", "Smg", "Smg MK2", "Assault Smg", "Combat PDW", "Machine Pistol", "Mini Smg", "Pump Shotgun", "Pump Shotgun MK2", "Sawnoff Shotgun", "Assault Shotgun", "Bullpup Shotgun", "Musket", "Heavy Shotgun", "DB Shotgun", "Auto Shotgun", "Assault Rifle", "Assault Rifle MK2", "Carbine Rifle", "Carbine Rifle MK2", "Advanced Rifle", "Special Carbine", "Special Carbine MK2", "Bullpup Rifle", "Bullpup Rifle MK2", "Compact Rifle", "Machine Gun", "Combat MG", "Combat MG MK2", "GUSENBERG", "Sniper Rifle", "AWP", "AWP MK2", "Marksman Rifle", "Marksman Rifle MK2", "RPG", "Grenade Launcher", "MiniGun", "FireWork", "RailGun", "Homing Launcher", "Compact Launcher", "Grenade", "BZGAS", "Smoke Grenade", "Flare", "Molotov", "Sticky BOMB", "Prox Mine", "SnowBall", "Pipe Bomb", "Ball", "Petrol Can", "Fire Extinguisher", "Parachute" };
		inline bool No_Recoil = false;
		inline bool No_Spread = false;
		inline bool InfiniteAmmo = false;
		inline bool NoRangeLimit = false;
		inline bool SetAmmo = false;
		inline bool ExplosiveAmmo = false;
		inline bool InstantHitAmmo = false;
		inline bool rapidfire = false;
		inline bool NoReload = false;
		//inline bool FastNoReload = false;

		inline int AmmoAmount = 30;
		inline int WeaponType = 10;
		inline bool DamageBoost = false;
		inline float DamageBoost_value = 1;
		inline int ReloadSpeed = 0;
		inline int RecoilManipulation = 0;

	}
	namespace Aimbot
	{
		inline bool Aimbot = false;
		inline bool Prediction = false;
		inline bool VisibleCheck = false;
		inline bool TriggerBot = false;
		inline int AimbotPrediction = 1;

		inline bool Draw_Fov = false;
		inline bool PreviewTarget = false;
		inline int AimbotFov = 10;
		inline int AimbotSmooth = 1;
		inline int TypeOfTarget = 0;
		inline const char *AimbotTarget[]{ "All", "Player", "Bot/Peds" };
		inline float DistanceAimbotLimit = 150;
		inline int AimbotBone = 0;
		inline const char* AimbotBones[]{ "Head", "Neck", "Stomach" };
		inline int AimbotTypes = 0;
		inline const char* AimbotType[]{ "Aim V1", "Aim V2", "Magic Bullet (non working)"};
		inline int MagicDamage = 50;

		inline int Fovid = 0;
		inline const char* FovType[]{ "Circle", "Filled Circle"}; //"Colored Circle"

		inline int Hotkey = 2;
		inline bool rapidfire = false;
		inline bool crosshair = false;

	}
	namespace Visuals
	{
		inline bool visualsenable = false;
		inline bool extendedesp = false;
		inline bool Box = false;
		inline bool Filled = false;
		inline bool Corner = false; 
		inline bool Skeleton = false;
		inline bool Snapline = false;
		inline bool flags = false;
		inline bool HealthBar = false;
		inline bool ArmorBar = false;
		inline bool Weapon = false;
		inline bool DistanceESP = false;
		inline bool NameEsp = false;
		inline bool PreviewEsp = false;
		inline bool PlayerName = false;
		inline bool VisibleCheck = false;
		inline float CornerSize = 1;
		inline float BoxSize = 1;

		inline int HealthBar_idx = 0;
		inline bool HealthBar_Multicolor = false;

		inline const char* HealthbarStyle[]{ "Right","Left","Bottom","Head"};

		inline float BoneThick = 1;
		inline bool Keybinds = false;
		inline bool Watermark = false;

		inline bool IgnorePlayers = false;
		inline bool Draw_LocalPlayer = false;
		inline bool Draw_Ped = false; 

		inline int Distance = 150;
		inline int FillBoxAlpha = 35;

		inline int FovAlpha = 125;

		inline ImColor FovColor = ImColor(255, 255, 255, 255);

		inline ImColor FovColored = ImColor(255, 255, 255, 35);

		inline ImColor BoxColor = ImColor(255, 255, 255, 255);
		inline ImColor BoxColorVisible = ImColor(252, 3, 23, 255);

		inline ImColor CornerColor = ImColor(33, 140, 176, 255);
		inline ImColor CornerColorVisible = ImColor(252, 3, 23, 255);

		inline ImColor CrosshairColor = ImColor(255, 255, 255, 255);

		inline ImColor SkelColor = ImColor(255, 255, 255, 255);

		inline ImColor SkelColorVisible = ImColor(252, 3, 23, 255);

		inline ImColor MenuColor = ImColor(255, 255, 255, 255);
	}
	namespace vehicle
	{
		inline bool vehicleenable = false;
		inline bool stickycar = false;
		inline bool godmode = false;
		inline bool fastspeed = false; 
		//inline bool nogravity = false; 
		inline bool setmaxhealth = false;
		inline int Speed = 10; 
		inline bool unlockcar = false;
		inline bool autostart_vehicle = false;
		inline bool warp_vehicle = false;
		inline bool repaire = false;
		inline bool shift_boost = false;
		inline bool vehicle_speed_bool = false;
		inline int vehicle_speed_value = 0;
		inline bool max_current_vehicle = false;
		inline bool on_car = false;
		inline bool NameESP = false;


		inline int mod_idx = 0;
		inline int primary_color = 0;
		inline int secondary_color = 0;
		inline int add_idx = 0;

		inline bool spawn_vehicle = false;
		inline char VehicleHashString[15] = "";
		inline char plate_text[8] = "";
		inline bool update_colors = false;
		inline bool update_plate = false;


	}
	namespace misc
	{
		inline bool menu = true;
		inline TextEditor editor;
		inline bool isPanic = false;
		inline bool Hitsound = false;
		inline int Menu_Hotkey = 0x74; //F5
		inline bool PedList = false;
		inline int NoclipKey = 0;
		inline int PanicKey = 0;
		inline char cfg_name[10] = "";

	}
	namespace TempColor
	{
		inline float Box[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float BoxVisible[4] = { 255.f, 255.f, 255.f, 255.f };

		inline float Corner[4] = { 33.f, 140.f, 176.f, 255.f };
		inline float CornerVisible[4] = { 33.f, 140.f, 176.f, 255.f };

		inline float crosshair[4] = { 255.f, 255.f, 255.f, 255.f };

		inline float fov[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float foved[4] = { 255.f, 255.f, 255.f, 125.f };

		inline float Skeleton[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float SkeletonVisible[4] = { 255.f, 255.f, 255.f, 255.f };

		inline float Menu[4] = { 255.f, 255.f, 255.f, 255.f };
	}
}