
#pragma once

#include "includes.hpp"

class Vector3 final
{
public:

	float x, y, z;

	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
	bool operator == (const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
	Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
	Vector3& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vector3& operator /= (const float& rhs) { return *this = *this / rhs; }
	float Length() const { return sqrt(x * x + y * y + z * z); }
	Vector3 Normalize() const { return *this * (1 / Length()); }
	float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
	void Invert() { *this *= -1; }
	static Vector3 FromM128(__m128 in) { return Vector3(in.m128_f32[0], in.m128_f32[1], in.m128_f32[2]); }
};

class Vector4
{
public:
	float x, y, z, w;
};

/*
inline ImVec2 GetWindowSize()
{
    RECT rect;
    HWND hwnd = GetActiveWindow();
    if (GetWindowRect(hwnd, &rect))
    {
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        return ImVec2(width, height);
      
    }
    return ImVec2(0, 0);
}
*/

namespace RAIDOS
{
    using namespace RAIDOS;
	using pizza_to_spaghetti_t = int32_t(*)(intptr_t pointer); //convert ped to handle
	inline std::vector<std::string> Friend;

	inline pizza_to_spaghetti_t pointer_to_handle;

    inline uint64_t World, PlayerInterface, ReplayInterface, Camera;

    //inline DWORD PlayerID, Armor, EntityType, WeaponManager, PlayerInfo, Recoil, Spread , AmmoType ,WeaponName, AmmoExplosiveType , Range , ReloadMultiplier , VehiculeReloadMultiplier , InVehicle;

	inline uint64_t PlayerID = NULL;
	inline uint64_t Armor = NULL;
	inline uint64_t EntityType = NULL;
	inline uint64_t WeaponManager = NULL;
	inline uint64_t PlayerInfo = NULL;
	inline uint64_t Recoil = NULL;
	inline uint64_t Spread = NULL;
	inline uint64_t AmmoType = NULL;
	inline uint64_t WeaponName = NULL;
	inline uint64_t AmmoExplosiveType = NULL;
	inline uint64_t Range = NULL;
	inline uint64_t ReloadMultiplier = NULL;
	inline uint64_t VehiculeReloadMultiplier = NULL;
	inline uint64_t InVehicle = NULL;

	inline int SizeX = GetSystemMetrics(SM_CXSCREEN);
	inline int SizeY = GetSystemMetrics(SM_CYSCREEN);

    inline ImVec2 WindowSize = ImVec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));



	inline DWORD64 GetCamera()
	{
		if (Camera)
			return *(DWORD64*)(Camera + 0x0);
	}

	inline float pythag(ImVec2 src, ImVec2 dst)
	{
		return sqrtf(pow(src.x - dst.x, 2) + pow(src.y - dst.y, 2));
	}
	inline float pythagVec3(Vector3 src, Vector3 dst)
	{
		return sqrtf(pow(src.x - dst.x, 2) + pow(src.y - dst.y, 2) + pow(src.z - dst.z, 2));
	}

	enum class Bone
	{
		HEAD,
		L_FOOT,
		R_FOOT,
		L_ANKLE,
		R_ANKLE,
		L_HAND,
		R_HAND,
		NECK,
		ABDOMEN
	};

	/*
	enum class Bone
	{
		HEAD = 0x796e,
		PELVIS = 0x2e28,
		NECK = 0x9995,
		SPINE3 = 0x60F2,
		L_UPPERARM = 0xb1c5,
		R_UPPERARM = 0x9d4d,
		L_ELBOW = 0x58b7,
		R_ELBOW = 0xbb0,
		L_HAND = 0x49d9,
		R_HAND = 0xdead,
		L_THIGH = 0xe39f,
		R_THIGH = 0xca72,
		L_CALF = 0xf9bb,
		R_CALF = 0x9000,
		L_FOOT = 0x3779,
		R_FOOT = 0xcc4d
	};
	*/
}

namespace RAIDAI
{

}
inline bool IsOnScreen(ImVec2 coords)
{
	if (coords.x < 0 || coords.y < 0 || coords.x > RAIDOS::SizeX || coords.y > RAIDOS::SizeY)
		return false;
	else 
		return true;
}

inline void DrawHealthBarhorizontal(ImVec2 pos, ImVec2 pos2, ImVec2 dim, ImColor col)
{
	if (IsOnScreen(pos)) //heh
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos.x + dim.y,pos.y), ImVec2(pos2.x , pos.y ), col, dim.x);
}
inline void DrawHealthBar(ImVec2 pos, ImVec2 dim, ImColor col)
{
	if (IsOnScreen(pos)) //heh
        ImGui::GetBackgroundDrawList()->AddLine(pos, ImVec2(pos.x, pos.y - dim.y), col, dim.x);
}
inline void DrawMultiColorHealthBar(ImVec2 pos, ImVec2 posx, ImColor col,ImColor col2)
{
	if (IsOnScreen(pos)) //heh
		ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(pos,posx,col,col,col2,col2);
}

inline const char* get_weapon_name(DWORD hash)
{
	const char* dagger = "Dagger";
	const char* bat = "Bat";
	const char* bottle = "Bottle";
	const char* crowbar = "Crow Bar";
	const char* unarmed = "None";
	const char* flashlight = "Flash Light";
	const char* golfclub = "Golf Club";
	const char* hammer = "Hammer";
	const char* hatchet = "Hatchet";
	const char* knuckle = "Knuckle";
	const char* knife = "Knife";
	const char* machete = "Machete";
	const char* switchblade = "Switch Blade";
	const char* nightstick = "Night Stick";
	const char* wrench = "Wrench";
	const char* battleaxe = "Battle Axe";
	const char* poolcue = "Pool Cue";
	const char* pistol = "Pistol";
	const char* pistolmk2 = "Pistol MK2";
	const char* combatpistol = "Combat Pistol";
	const char* appistol = "AP Pistol";
	const char* stungun = "Stungun";
	const char* pistol50 = "Pistol 50";
	const char* snspistol = "SNS PISTOL";
	const char* snspistolmk2 = "SNS Pistol MK2";
	const char* heavypistol = "Heavy Pistol";
	const char* vintagepistol = "Vintage Pistol";
	const char* flaregun = "Flare Gun";
	const char* marksmanpistol = "Marksman Pistol";
	const char* revolver = "Revolver";
	const char* revolvermk2 = "Revolver MK2";
	static auto doubleaction = "Double Action";
	static auto microsmg = "Micro Smg";
	static auto smg = "Smg";
	static auto smgmk2 = "Smg MK2";
	static auto assaultsmg = "Assault Smg";
	static auto combatpdw = "Combat PDW";
	static auto machinepistol = "Machine Pistol";
	static auto minismg = "Mini Smg";
	static auto pumpshotgun = "Pump Shotgun";
	static auto pumpshotgun_mk2 = "Pump Shotgun MK2";
	static auto sawnoffshotgun = "Sawnoff Shotgun";
	static auto assaultshotgun = "Assault Shotgun";
	static auto bullpupshotgun = "Bullpup Shotgun";
	static auto musket = "Musket";
	static auto heavyshotgun = "Heavy Shotgun";
	static auto dbshotgun = "DB Shotgun";
	static auto autoshotgun = "Auto Shotgun";
	static auto assaultrifle = "Assault Rifle";
	static auto assaultrifle_mk2 = "Assault Rifle MK2";
	static auto carbinerifle = "Carbine Rifle";
	static auto carbinerifle_mk2 = "Carbine Rifle MK2";
	static auto advancedrifle = "Advanced Rifle";
	static auto specialcarbine = "Special Carbine";
	static auto specialcarbine_mk2 = "Special Carbine MK2";
	static auto bullpuprifle = "Bullpup Rifle";
	static auto bullpuprifle_mk2 = "Bullpup Rifle MK2";
	static auto compactrifle = "Compact Rifle";
	static auto machine_gun = "Machine Gun";
	static auto combatmg = "Combat MG";
	static auto combatmg_mk2 = "Combat MG MK2";
	static auto gusenberg = "GUSENBERG";
	static auto sniperrifle = "Sniper Rifle";
	static auto heavysniper = "AWP";
	static auto heavysniper_mk2 = "AWP MK2";
	static auto marksmanrifle = "Marksman Rifle";
	static auto marksmanrifle_mk2 = "Marksman Rifle MK2";
	static auto rpg = "RPG";
	static auto grenadelauncher = "Grenade Launcher";
	static auto grenadelauncher_smoke = "Grenade Launcher Smoke";
	static auto minigun = "MiniGun";
	static auto firework = "FireWork";
	static auto railgun = "RailGun";
	static auto hominglauncher = "Homing Launcher";
	static auto compactlauncher = "Compact Launcher";
	static auto grenade = "Grenade";
	static auto bzgas = "BZGAS";
	static auto smokegrenade = "Smoke Grenade";
	static auto flare = "Flare";
	static auto molotov = "Molotov";
	static auto stickybomb = "Sticky BOMB";
	static auto proxmine = "Prox Mine";
	static auto snowball = "SnowBall";
	static auto pipebomb = "Pipe Bomb";
	static auto ball = "Ball";
	static auto petrolcan = "Petrol Can";
	static auto fireextinguisher = "Fire Extinguisher";
	static auto parachute = "Parachute";

	switch (hash)
	{
	case 0x92A27487:
		return dagger; break;
	case 0x958A4A8F:
		return bat; break;
	case 0xF9E6AA4B:
		return bottle; break;
	case 0x84BD7BFD:
		return crowbar; break;
	case 0xA2719263:
		return unarmed; break;
	case 0x8BB05FD7:
		return flashlight; break;
	case 0x440E4788:
		return golfclub; break;
	case 0x4E875F73:
		return hammer; break;
	case 0xF9DCBF2D:
		return hatchet; break;
	case 0xD8DF3C3C:
		return knuckle; break;
	case 0x99B507EA:
		return knife; break;
	case 0xDD5DF8D9:
		return machete; break;
	case 0xDFE37640:
		return switchblade; break;
	case 0x678B81B1:
		return nightstick; break;
	case 0x19044EE0:
		return wrench; break;
	case 0xCD274149:
		return battleaxe; break;
	case 0x94117305:
		return poolcue; break;
	case 0x1B06D571:
		return pistol; break;
	case 0xBFE256D4:
		return pistolmk2; break;
	case 0x5EF9FEC4:
		return combatpistol; break;
	case 0x22D8FE39:
		return appistol; break;
	case 0x3656C8C1:
		return stungun; break;
	case 0x99AEEB3B:
		return pistol50; break;
	case 0xBFD21232:
		return snspistol; break;
	case 0x88374054:
		return snspistolmk2; break;
	case 0xD205520E:
		return heavypistol; break;
	case 0x83839C4:
		return vintagepistol; break;
	case 0x47757124:
		return flaregun; break;
	case 0xDC4DB296:
		return marksmanpistol; break;
	case 0xC1B3C3D1:
		return revolver; break;
	case 0xCB96392F:
		return revolvermk2; break;
	case 0x97EA20B8:
		return doubleaction; break;
	case 0x13532244:
		return microsmg; break;
	case 0x2BE6766B:
		return smg; break;
	case 0x78A97CD0:
		return smgmk2; break;
	case 0xEFE7E2DF:
		return assaultsmg; break;
	case 0xA3D4D34:
		return combatpdw; break;
	case 0xDB1AA450:
		return machinepistol; break;
	case 0xBD248B55:
		return minismg; break;
	case 0x1D073A89:
		return pumpshotgun; break;
	case 0x555AF99A:
		return pumpshotgun_mk2; break;
	case 0x7846A318:
		return sawnoffshotgun; break;
	case 0xE284C527:
		return assaultshotgun; break;
	case 0x9D61E50F:
		return bullpupshotgun; break;
	case 0xA89CB99E:
		return musket; break;
	case 0x3AABBBAA:
		return heavyshotgun; break;
	case 0xEF951FBB:
		return dbshotgun; break;
	case 0x12E82D3D:
		return autoshotgun; break;
	case 0xBFEFFF6D:
		return assaultrifle; break;
	case 0x394F415C:
		return assaultrifle_mk2; break;
	case 0x83BF0278:
		return carbinerifle; break;
	case 0xFAD1F1C9:
		return carbinerifle_mk2; break;
	case 0xAF113F99:
		return advancedrifle; break;
	case 0xC0A3098D:
		return specialcarbine; break;
	case 0x969C3D67:
		return specialcarbine_mk2; break;
	case 0x7F229F94:
		return bullpuprifle; break;
	case 0x84D6FAFD:
		return bullpuprifle_mk2; break;
	case 0x624FE830:
		return compactrifle; break;
	case 0x9D07F764:
		return machine_gun; break;
	case 0x7FD62962:
		return combatmg; break;
	case 0xDBBD7280:
		return combatmg_mk2; break;
	case 0x61012683:
		return gusenberg; break;
	case 0x5FC3C11:
		return sniperrifle; break;
	case 0xC472FE2:
		return heavysniper; break;
	case 0xA914799:
		return heavysniper_mk2; break;
	case 0xC734385A:
		return marksmanrifle; break;
	case 0x6A6C02E0:
		return marksmanrifle_mk2; break;
	case 0xB1CA77B1:
		return rpg; break;
	case 0xA284510B:
		return grenadelauncher; break;
	case 0x4DD2DC56:
		return grenadelauncher_smoke; break;
	case 0x42BF8A85:
		return minigun; break;
	case 0x7F7497E5:
		return firework; break;
	case 0x6D544C99:
		return railgun; break;
	case 0x63AB0442:
		return hominglauncher; break;
	case 0x781FE4A:
		return compactlauncher; break;
	case 0x93E220BD:
		return grenade; break;
	case 0xA0973D5E:
		return bzgas; break;
	case 0xFDBC8A50:
		return smokegrenade; break;
	case 0x497FACC3:
		return flare; break;
	case 0x24B17070:
		return molotov; break;
	case 0x2C3731D9:
		return stickybomb; break;
	case 0xAB564B93:
		return proxmine; break;
	case 0x787F0BB:
		return snowball; break;
	case 0xBA45E8B8:
		return pipebomb; break;
	case 0x23C9F95C:
		return ball; break;
	case 0x34A67B97:
		return petrolcan; break;
	case 0x60EC506:
		return fireextinguisher; break;
	case 0xFBAB5776:
		return parachute; break;
	default:
		return unarmed; break;
	}
}

class hk_PlayerAngles;
class hk_CameraAngles;
class hk_GameCameraAngles;
class hk_CameraManagerAngles;

class hk_CameraAngles
{
public:
	hk_PlayerAngles* VehiclePointer1()
	{
		if (!this) return 0;
		return(hk_PlayerAngles*)(*(uint64_t*)(this + 0x2C0));
	}
	hk_PlayerAngles* VehiclePointer2()
	{
		if (!this) return 0;
		return(hk_PlayerAngles*)(*(uint64_t*)(this + 0x2C8));
	}
	hk_PlayerAngles* MyFpsAngles()
	{
		if (!this) return 0;
		return(hk_PlayerAngles*)(*(uint64_t*)(this + 0x3C0));
	}
	int pTPSCamEDX()
	{
		if (!this) return 0;
		return *(int*)(this + 0x3D8);
	}
};

class hk_PlayerAngles
{
public:
	Vector3 GetCamRotFPS()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x40);
	}
	Vector3 GetCrosshairLocation()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x60);
	}
	Vector3 GetFov()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 90);
	}
	Vector3 GetCamRotation()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x3D0);
	}
};

class hk_GameCameraAngles
{
public:
	hk_CameraManagerAngles* MyCameraManagerAngles()
	{
		if (!this) return 0;
		return(hk_CameraManagerAngles*)(*(uint64_t*)(this + 0x0));
	}
};

class hk_CameraManagerAngles
{
public:
	hk_CameraAngles* MyCameraAngles()
	{
		if (!this) return 0;
		return (hk_CameraAngles*)(*(uint64_t*)(this + 0x0));
	}
};
class hk_FixedAmmoCount
{
public:
    float SetAmmo(float caca)
    {
        if (!this) return 0;
        return *(uint32_t*)(this + 0x18) = caca;
    }
};
class hk_AmmoCount
{
public:

    hk_FixedAmmoCount* FixedAmmoCount()
    {
        if (!this) return 0;
        return (hk_FixedAmmoCount*)(*(uint64_t*)(this + 0x0));
    }
};
class hk_AmmoInfo
{
public:
    hk_AmmoCount* AmmoCount()
    {
        if (!this) return 0;
        return (hk_AmmoCount*)(*(uint64_t*)(this + 0x8));
    }
	float SetSpeedBullet(float value)
	{
		if (!this) return 0;
		return *(float*)(this + 0x58) = value;
	}
};
class hk_WeaponInfo
{
public:
	uint64_t GetHash()
	{
		if (!this) return NULL;
		return *(uint64_t*)(this + 0x10);
	}
	uint64_t SetHash(DWORD caca)
	{
		if (!this) return NULL;
		return *(uint64_t*)(this + 0x10) = caca;
	}
	float SetBulletDamage(float value) //eklendi
	{
		if (!this) return 0;
		return *(float*)(this + 0xB0) = value;
	}
    float SetSpread(float value)
    {
        if (!this) return 0;
        return *(float*)(this + RAIDOS::Spread) = value; //Spread
    }
    float SetRecoil(float value)
    {
        if (!this) return 0;
        return *(float*)(this + RAIDOS::Recoil) = value; //Recoil
    }
    hk_AmmoInfo* AmmoInfo()
    {
        if (!this) return 0;
        return (hk_AmmoInfo*)(*(uint64_t*)(this + 0x60));
    }
    float SetReload(float value)
    {
        if (!this) return 0;
        return *(float*)(this + RAIDOS::ReloadMultiplier) = value; //ReloadMultiplier
    }
	float SetRange(float value)
	{
		if (!this) return 0;
		return *(float*)(this + RAIDOS::Range) = value; //Range
	}
    int32_t  SetAmmoType(float value)
    {
        if (!this) return 0;
        return *(int32_t*)(this + RAIDOS::AmmoType) = value; //AmmoType
    }
    int32_t  SetAmmoExplosiveType(float value)
    {
        if (!this) return 0;
        return *(int32_t*)(this + RAIDOS::AmmoExplosiveType) = value; //AmmoExplosiveType
    }
	int32_t  SetSpeedType(float value)
	{
		if (!this) return 0;
		return *(int32_t*)(this + 0x54) = value;
	}
	float  SetSpeedValue(float value)
	{
		if (!this) return 0;
		return *(float*)(this + 0x011C) = value;
	}
};
class hk_WeaponManager
{
public:
    hk_WeaponInfo* WeaponInfo()
    {
        if (!this) return 0;
        return (hk_WeaponInfo*)(*(uint64_t*)(this + 0x20));
    }
	Vector3 GetWeaponCoordinate()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x1A0);
	}
	Vector3 SetWeaponCoordinate(Vector3 Cords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x1A0) = Cords;
	}
	Vector3 GetLastImpactCoords() //eklendi
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x1B0);
	}
};

class hk_ObjectNavigationPed
{
public:
    Vector3 GetCoordinate()
    {
        if (!this) return Vector3{ 0,0,0 };
        return *(Vector3*)(this + 0x50);
    }
    Vector3 SetCoordinate(Vector3 Cords)
    {
        if (!this) return Vector3{ 0,0,0 };
        return *(Vector3*)(this + 0x50) = Cords;
    }
	Vector4 SetRotation(Vector4 Coords)
	{
		if (!this) return Vector4{ 0,0,0,0 };
		return *(Vector4*)(this + 0x30) = Coords;
	}
};
class hk_VehicleModelInfo
{
public:
	char* GetCharName()
	{
		if (!this) return 0;
		return (char*)(this + 0x298);
	}
};
class hk_VehicleHandlingInfo
{
public:

	float SetMass(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x0C) = Health;
	}
	float SetAcceleration(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x4C) = Health;
	}
	float SetHandBrake(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x7C) = Health;
	}
	float SetBrake(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x6C) = Health;
	}
};
class hk_Vehicle
{
public:
	hk_VehicleModelInfo* VehicleModelInfo()
	{
		if (!this) return 0;
		return (hk_VehicleModelInfo*)(*(uint64_t*)(this + 0x20));
	}
	hk_VehicleHandlingInfo* VehicleHandlingInfo()
	{
		if (!this) return 0;
		return (hk_VehicleHandlingInfo*)(*(uint64_t*)(this + 0x938));
	}
	Vector3 GetCoordinate()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x90);
	}
	Vector3 SetCoordinate(Vector3 Cords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x90) = Cords;
	}
	Vector3 SetVelocity()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x320) = Vector3(0, 0, 0);
	}
	float GetHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x280);
	}
	bool Godmode(bool value)
	{
		if (!this) return 0;
		return *(bool*)(this + 0x189) = value;
	}
	float GetMaxHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x284);
	}
	float SetHealth(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x280) = Health;
	}
	float SetGravity(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0xC5C) = Health;
	}
	float SetBodyHealth(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x840) = Health;
	}
	float SetEngineHealth(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x908) = Health;
	}
};
class hk_Gravity
{
public:
    bool IsNoGravity()
	{
		if (*(uint16_t*)(this + 0x1A) == 780)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	uint16_t SetNoGravity(bool caca)
	{
		if (!this) return 0;
		if (caca)
			return *(uint16_t*)(this + 0x1A) = 0x30C;
		else
		return *(uint16_t*)(this + 0x1A) = 0x304;
	}
};
class hk_PlayerInfo
{
public:
	int GetHealth()
	{
		if (!this) return 0;
		return *(int*)(this + 0x88); //Playerid
	}
	float SetSpeed(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x0CF0) = Health;
	}
};
class hk_PedModelInfo
{
public:

	Vector3 GetMin()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x30);
	}
	Vector3 GetMax()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x40);
	}

};
class hk_Ped
{
public:
	hk_PedModelInfo* PedModelInfo()
	{
		if (!this) return 0;
		return (hk_PedModelInfo*)(*(uint64_t*)(this + 0x20));
	}
	hk_PlayerInfo* PlayerInfo()
	{
		if (!this) return 0;
		return (hk_PlayerInfo*)(*(uint64_t*)(this + RAIDOS::PlayerInfo)); //Playerinfo
	}
    hk_WeaponManager* WeaponManager()
    {
        if (!this) return 0;
        return (hk_WeaponManager*)(*(uint64_t*)(this + RAIDOS::WeaponManager )); //Weaponmanager
    }
	hk_Gravity* GravityManager()
	{
		if (!this) return 0;
		return (hk_Gravity*)(*(uintptr_t*)(this + 0x1110));
	}
	hk_Vehicle* VehicleManager()
	{
		if (!this) return 0;
		return (hk_Vehicle*)(*(uint64_t*)(this + 0xD30));
	}
    hk_ObjectNavigationPed* ObjectNavigation()
    {
        if (!this) return 0;
        return (hk_ObjectNavigationPed*)(*(uint64_t*)(this + 0x30));
    }
	bool InVehicle()
	{
		if (!this) return false;
		if (*(BYTE*)(this + RAIDOS::InVehicle) == 0x40) //InVehicle
		{
			return true;
		}
		else
		{
			return false;
		}
	}
    Vector3 GetCoordinate()
    {
        if (!this) return Vector3{ 0,0,0 };
        return *(Vector3*)(this + 0x90); //Cord 0x90
    }
    Vector3 SetCoordinate(Vector3 Cords)
    {
        if (!this) return Vector3{ 0,0,0 };
        return *(Vector3*)(this + 0x90) = Cords; //Cord 0x90
    }
	Vector3 SetVelocity()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x320) = Vector3(0,0,0);
	}
    float GetHealth()
    {
        if (!this) return 0;
        return *(float*)(this + 0x280); //Entity Health
    }
	float GetArmor()
	{
		if (!this) return 0;
		return *(float*)(this + RAIDOS::Armor); //Armor : 0x14E0 + 0x50 = 0x1530
	}
	float SetHealth(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x280) = Health;
	}
	float SetArmor(float Armor)
	{
		if (!this) return 0;
		return *(float*)(this + RAIDOS::Armor) = Armor; //Armor: 0x14E0 + 0x50 = 0x1530
	}
	float SetMaxHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x280) = GetMaxHealth();
	}
    float GetMaxHealth()
    {
        if (!this) return 0;
        return *(float*)(this + 0x2A0); //GetMaxHealth
    }
	bool SetGoodmode(bool Health)
	{
		if (!this) return 0;
		return *(bool*)(this + 0x189) = Health;
	}
    uint32_t GetPedType()
    {
        if (!this) return 0;
        return *(uint32_t*)(this + RAIDOS::EntityType); //EntityType
    }
	int32_t FreezePlayer()
	{
		if (!this) return 0;
		return *(int32_t*)(this + 0x238) = 2;
	}


	bool IsPedOrFalse()
	{
		if (!this) return 0;
		auto ped_type = this->GetPedType();

		ped_type = ped_type << 11 >> 25;
	
		if (ped_type != 2)
		{
			return true;

		}
		else
			return false;
	}
	BYTE SetInvisible(BYTE caca)
	{
		if (!this) return 0;
		return *(BYTE*)(this + 0x2C) = caca; // 0X1 ou 0X37
	}
	float SetSpeed(float Health)
	{
		if (!this) return 0;
		return *(float*)((this + RAIDOS::PlayerInfo) + 0x0CF0) = Health; //PlayerInfo
	}
	unsigned char Set_Ragdoll(bool value)
	{
		if (!this) return 0;
			if (value)
			{
				return *(unsigned char*)(this + 0x10B8) =0x0;
			}
			else
			{
				return *(unsigned char*)(this + 0x10B8) = 0x20;
			}
	}
};

class hk_PedList
{
public:
    hk_Ped* Ped(int index)
    {
        if (!this) return 0;
        return (hk_Ped*)(*(uint64_t*)(this + (index * 0x10)));
    }
};
class hk_PedInterface
{
public:
    uint64_t PedMaximum()
    {
        if (!this) return 0;
        return (uint64_t)(*(uint64_t*)(this + 0x108)); // A8
    }

    hk_PedList* PedList()
    {
        if (!this) return 0;
        return (hk_PedList*)(*(uint64_t*)(this + 0x100)); // A0
    }
};
class hk_VehicleList
{
public:
	hk_Vehicle* Vehicle(int index)
	{
		if (!this) return 0;
		return (hk_Vehicle*)(*(uint64_t*)(this + (index * 0x10)));
	}

};
class hk_VehicleInterface
{
public:
	uint64_t VehicleMaximum()
	{
		if (!this) return 0;
		return (uint64_t)(*(uint64_t*)(this + 0x188));
	}

	hk_VehicleList* VehicleList()
	{
		if (!this) return 0;
		return (hk_VehicleList*)(*(uint64_t*)(this + 0x180));
	}
};
class hk_ReplayInterface
{
public:
    hk_PedInterface* PedInterface()
    {
        if (!this) return 0;
        return (hk_PedInterface*)(*(uint64_t*)(this + 0x18));
    }

	hk_VehicleInterface* VehicleInterface()
	{
		if (!this) return 0;
		return (hk_VehicleInterface*)(*(uint64_t*)(this + 0x10));
	}
};
class hk_PlayerInterface
{
public:

	uint64_t PlayerMaximum()
	{
		if (!this) return 0;
		return (uint64_t)(*(uint64_t*)(this + 0x178));
	}


};
class hk_World
{
public:
    hk_Ped* LocalPlayer()
    {
        if (!this) return 0;
        return (hk_Ped*)(*(uint64_t*)(this + 0x8));
    }
};

