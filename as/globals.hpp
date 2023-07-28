#pragma once

#define DECLARE_VAR(type, name, value) inline type name = value;

namespace ui
{
	inline int tab = 0;
}

namespace aimbot
{
	inline bool enabled = false;
	inline bool silent = false;
	inline int silent_key = VK_LBUTTON;
	inline float smoothing = 4.5f;
	inline float fov = 150;
	inline float fov_color[4]{ 1, 1, 1, 1 };
	inline bool visible_check = false;
	inline bool draw_fov = false;

	inline bool crosshair = false;
	inline float crosshair_color[4]{ 1, 1, 1, 1 };

	inline float max_distance = 200.0f;
	inline int keybind = VK_RBUTTON;

	inline int bone = 0; // head, neck, chest
	inline int type = 0; // memory, mouse
}

namespace esp
{
	inline bool enabled = false;

	inline bool boxes = false;
	inline bool boxes_filled = false;
	inline bool boxes_3d = false;
	inline float boxes_color[4] = { 1, 0, 0, 1 };

	inline bool corners = false;
	inline float corners_color[4] = { 1, 0,0, 1 };

	inline bool snaplines = false;
	inline float snaplines_color[4] = { 1, 1, 1, 1 };

	inline bool skeleton = false;
	inline float skeleton_color[4] = { 1, 1, 1, 1 };

	inline bool distance = false;
	inline float distance_color[4] = { 1, 1, 1, 1 };

	inline bool names = false;
	inline float names_color[4] = { 1, 1, 1, 1 };

	inline float max_distance = 200.0f;
	inline bool visible_check = false;
	inline bool armour_bar = false;
	inline bool health_bar = false;
	inline bool ignore_npc = true;
}

namespace misc
{
	inline bool godmode =false;
	inline bool noclip = false;
	inline int noclip_key = 'X';
	inline bool no_recoil = false;
	inline bool no_spread = false;
	inline bool unlimited_ammo = false;
	inline bool crosshair = false;
	inline int menu_keybind = VK_DELETE;
	inline bool fov_changer = false;
	inline float fov = 50.0f;
}

namespace settings
{
	inline int transparency = 240.0f;
}

namespace fonts
{
	inline void* courbd;
}

namespace data
{
	inline std::uint64_t local_player = 0;
	inline std::uint64_t world_to_screen = 0;
	inline std::uint64_t get_bone_by_index = 0;
	inline std::uint64_t replay_interface = 0;
	inline std::uint64_t world = 0;
	inline std::uint64_t camera_angles_offset = 0;
	inline std::uint64_t game_build = 0;
	inline int max_peds = 0;
	inline std::uint64_t entity_list = 0;
	inline std::uint64_t dec_weapon_ammo = 0;

	namespace netvar
	{
		// 48 8B 81 ? ? ? ? 48 85 C0 74 ? F6 80 ? ? ? ? ? 74 ? 32 C0
		// 48 8B 90 ? ? ? ? 4C 8D 82 ? ? ? ? 48 85 D2 75
		inline std::uint64_t player_info = 0;

		// 48 8B B7 ? ? ? ? 48 85 F6 74 ? 48 8B CE E8 ? ? ? ? 48 85 C0 74 ? 48 8B CE E8 ? ? ? ? 4C 8B 70
		inline std::uint64_t weapon_manager = 0;

		// 8B 83 ? ? ? ? 45 0F 5B D2
		inline std::uint64_t entity_type = 0;

		// F3 0F 10 8F ? ? ? ? F3 0F 10 97 ? ? ? ? F3 41 0F 10 1F
		inline std::uint64_t max_health = 0;

		// 48 8D BE ? ? ? ? 32 C9 48 8B 5F ? 44 8B D8
		inline std::uint64_t armour = 0;

		// F3 0F 10 B0 ? ? ? ? 49 8B 87
		inline std::uint64_t recoil = 0;

		// F3 0F 59 70 ? 0F 28 C6
		inline std::uint64_t spread = 0;
	}

	inline const char* (__fastcall* GetPlayerNameFromScAddr)(void*);
	inline std::uint64_t start_shape_test_capsule = 0;
	inline std::uint64_t get_raycast_result = 0;
}

namespace b1604
{
	inline std::uint64_t player_info = 0x10B8;      // CPlayerInfo*
	inline std::uint64_t weapon_manager = 0x10C8;   // CPedWeaponManager*
	inline std::uint64_t entity_type = 0x10A8;
	inline std::uint64_t max_health = 0x2A0;
	inline std::uint64_t armour = 0x14B8;
	inline std::uint64_t recoil = 0x2E8;
	inline std::uint64_t spread = 0x74;
}

namespace b2060
{
	inline std::uint64_t player_info = 0x10C8;      // CPlayerInfo*
	inline std::uint64_t weapon_manager = 0x10D8;   // CPedWeaponManager*
	inline std::uint64_t entity_type = 0x10B8;
	inline std::uint64_t max_health = 0x2A0;
	inline std::uint64_t armour = 0x14E0;
	inline std::uint64_t recoil = 0x2F4; // untested
	inline std::uint64_t spread = 0x74; // untested
}

namespace b2189
{
	inline std::uint64_t player_info = 0x10C8;      // CPlayerInfo*
	inline std::uint64_t weapon_manager = 0x10D8;   // CPedWeaponManager*
	inline std::uint64_t entity_type = 0x10B8;
	inline std::uint64_t max_health = 0x2A0;
	inline std::uint64_t armour = 0x14E0;
	inline std::uint64_t recoil = 0x2F4; // untested
	inline std::uint64_t spread = 0x74; // untested
}

namespace b2372
{
	inline std::uint64_t player_info = 0x10C8;      // CPlayerInfo*
	inline std::uint64_t weapon_manager = 0x10D8;   // CPedWeaponManager*
	inline std::uint64_t entity_type = 0x10B8;
	inline std::uint64_t max_health = 0x2A0;
	inline std::uint64_t armour = 0x14E0;
	inline std::uint64_t recoil = 0x2F4;
	inline std::uint64_t spread = 0x74;
}

namespace b2545
{
	inline std::uint64_t player_info = 0x10C8;      // CPlayerInfo*
	inline std::uint64_t weapon_manager = 0x10D8;   // CPedWeaponManager*
	inline std::uint64_t entity_type = 0x10B8;
	inline std::uint64_t max_health = 0x2A0;
	inline std::uint64_t armour = 0x14E0 + 0x50;
	inline std::uint64_t recoil = 0x2F4;
	inline std::uint64_t spread = 0x74;
}

namespace b2612
{
	inline std::uint64_t player_info = 0x10C8;      // CPlayerInfo*
	inline std::uint64_t weapon_manager = 0x10D8;   // CPedWeaponManager*
	inline std::uint64_t entity_type = 0x10B8;
	inline std::uint64_t max_health = 0x2A0;
	inline std::uint64_t armour = 0x1530;
	inline std::uint64_t recoil = 0x2F4;
	inline std::uint64_t spread = 0x74;
}

namespace b2699
{
	inline std::uint64_t player_info = 0x10C8;      // CPlayerInfo*
	inline std::uint64_t weapon_manager = 0x10D8;   // CPedWeaponManager*
	inline std::uint64_t entity_type = 0x10B8;
	inline std::uint64_t max_health = 0x2A0;
	inline std::uint64_t armour = 0x1530;
	inline std::uint64_t recoil = 0x2F4;
	inline std::uint64_t spread = 0x74;
}