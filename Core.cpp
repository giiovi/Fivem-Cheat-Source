#pragma once
#include "Core.hpp"
#include <iomanip>
#include "xorstr.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <regex>
#include "globals.h"

std::string GetProcessorId() {
    std::array<int, 4> cpuInfo;
    __cpuid(cpuInfo.data(), 1);
    std::ostringstream buffer;
    buffer
        << std::uppercase << std::hex << std::setfill('0')
        << std::setw(8) << cpuInfo.at(3)
        << std::setw(8) << cpuInfo.at(0);
    return buffer.str();
}










void typeTextSlowly(const std::string& text, WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);

    for (char c : text) {
        std::cout << c;
        std::this_thread::sleep_for(std::chrono::milliseconds(65));
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}









enum heads {
    aim, online, visuals, settings, weapons, configs, scripts
};
enum sub_heads {
    aim1, aim2, online1, online2, online3, visuals1, visuals2, settings1, settings2, weapons1, weapons2, configs1, configs2, scripts1, scripts2
};



FILE* p_stream;
static bool remembered = false;

DWORD weaponsList[] = {
0x93E220BD, 0xA0973D5E, 0x24B17070, 0x2C3731D9, 0xAB564B93, 0x787F0BB, 0xBA45E8B8, 0x23C9F95C, 0xFDBC8A50, 0x497FACC3, //Throwables
0x34A67B97, 0xFBAB5776, 0x060EC506, //Miscellaneous
0xB1CA77B1, 0xA284510B, 0x42BF8A85, 0x7F7497E5, 0x6D544C99, 0x63AB0442, 0x0781FE4A, 0xB62D1F67, //Heavy Weapons
0x05FC3C11, 0x0C472FE2, 0xA914799, 0xC734385A, 0x6A6C02E0, //Sniper Rifles
0x9D07F764, 0x7FD62962, 0xDBBD7280, 0x61012683, //Light Machine Guns
0xBFEFFF6D, 0x394F415C, 0x83BF0278, 0xFAD1F1C9, 0xAF113F99, 0xC0A3098D, 0x969C3D67, 0x7F229F94, 0x84D6FAFD, 0x624FE830, //Assault Rifles
0x13532244, 0x2BE6766B, 0x78A97CD0, 0xEFE7E2DF, 0x0A3D4D34, 0xDB1AA450, 0xBD248B55, 0x476BF155, //Submachine Guns
0x1B06D571, 0xBFE256D4, 0x5EF9FEC4, 0x22D8FE39, 0x3656C8C1, 0x99AEEB3B, 0xBFD21232, 0x88374054, 0xD205520E, 0x83839C4, 0x47757124, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0xAF3696A1, //Pistols
0x92A27487, 0x958A4A8F, 0xF9E6AA4B, 0x84BD7BFD, 0x8BB05FD7, 0x440E4788, 0x4E875F73, 0xF9DCBF2D, 0xD8DF3C3C, 0x99B507EA, 0xDD5DF8D9, 0xDFE37640, 0x678B81B1, 0x19044EE0, 0xCD274149, 0x94117305, 0x3813FC08, //Mele
0x1D073A89, 0x555AF99A, 0x7846A318, 0xE284C527, 0x9D61E50F, 0xA89CB99E, 0x3AABBBAA, 0xEF951FBB, 0x12E82D3D
};

#pragma region HideConsole
void HIDECONSOLE()	
{	
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}
#pragma endregion

void InitImGui()
{
	using namespace DirectX;

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    io.SetPlatformImeDataFn = nullptr;
    


	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	
    // Load Fonts
    ImFontConfig font_config;
    //closed -> font_config.MergeMode = true;
    font_config.PixelSnapH = false;
    font_config.OversampleH = 5;
    font_config.OversampleV = 5;
    font_config.RasterizerMultiply = 1.2f;

    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0xE000, 0xE226, // icons
        0,
    };

    font_config.GlyphRanges = ranges;

    medium = io.Fonts->AddFontFromMemoryTTF(PTRootUIMedium, sizeof(PTRootUIMedium), 15.0f, &font_config, ranges);
    bold = io.Fonts->AddFontFromMemoryTTF(PTRootUIBold, sizeof(PTRootUIBold), 15.0f, &font_config, ranges);
    tab_icons = io.Fonts->AddFontFromMemoryTTF(clarityfont, sizeof(clarityfont), 15.0f, &font_config, ranges);
    logo = io.Fonts->AddFontFromMemoryTTF(clarityfont, sizeof(clarityfont), 21.0f, &font_config, ranges);
    tab_title = io.Fonts->AddFontFromMemoryTTF(PTRootUIBold, sizeof(PTRootUIBold), 19.0f, &font_config, ranges);
    tab_title_icon = io.Fonts->AddFontFromMemoryTTF(clarityfont, sizeof(clarityfont), 18.0f, &font_config, ranges);
    subtab_title = io.Fonts->AddFontFromMemoryTTF(PTRootUIBold, sizeof(PTRootUIBold), 15.0f, &font_config, ranges);
    combo_arrow = io.Fonts->AddFontFromMemoryTTF(combo, sizeof(combo), 9.0f, &font_config, ranges);

    // Our state
    ImVec4 clear_color = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WindowHandler(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (Core::Open)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return (CallWindowProcA)(DirectX::WindowEx, hWnd, uMsg, wParam, lParam);
}

bool BindD3DInfo(IDXGISwapChain* pSwapChain)
{
	if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&DirectX::pDevice)))
	{
		DirectX::pDevice->GetImmediateContext(&DirectX::pContext);
		DXGI_SWAP_CHAIN_DESC sd;
        pSwapChain->GetDesc(&sd);
		DirectX::Window = sd.OutputWindow;
		ID3D11Texture2D* pBackBuffer;
        pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
		DirectX::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &DirectX::renderTargetView);
		pBackBuffer->Release();

		DirectX::WindowEx = (WNDPROC)SetWindowLongPtrA(DirectX::Window, GWLP_WNDPROC, (LONG_PTR)WindowHandler);
        
        InitImGui();
        Core::FirstTime = false;

		return true;
	}

	return false;
}

#pragma region raidaimfunc


void Core::GetOffset() {
    /*
    std::string game_2628 = "1.46";
    std::string game_2545 = "1.58";
    std::string game_2372 = "1.57";
    std::string game_2189 = "1.52";
    std::string game_2060 = "1.51";
    std::string game_1604 = "1.46";*/

    auto version = NETWORK::_GET_ONLINE_VERSION();
    float vOut = (float)strtod(version, NULL);
    int vOuts = vOut * 100;

    switch (vOuts) {
        #pragma region version_2699
        case 161:
            RAIDOS::EntityType = 0x10B8;
            RAIDOS::Armor = 0x1530; //0x14E0 + ( 0x50 )

            RAIDOS::WeaponManager = 0x10D8; //D8
            RAIDOS::PlayerInfo = 0x10C8; //B8
            RAIDOS::PlayerID = 0x88;

            RAIDOS::Recoil = 0x2F4; //m_recoil_shake_hash
            RAIDOS::Spread = 0x84; //m_accuracy_spread
            RAIDOS::ReloadMultiplier = 0x134; //m_anim_reload_time
            RAIDOS::AmmoType = 0x20;
            //RAIDOS::AmmoExplosiveType = 0x24;

            //RAIDOS::WeaponName = 0x5F0;
            RAIDOS::InVehicle = 0x146B; //157
            RAIDOS::Range = 0x28C; 
            break;
        #pragma endregion
        #pragma region version_2612
        case 159:
            RAIDOS::EntityType = 0x10B8;
            RAIDOS::Armor = 0x1530;

            RAIDOS::WeaponManager = 0x10D8;
            RAIDOS::PlayerInfo = 0x10C8;
            RAIDOS::Recoil = 0x2F4;
            RAIDOS::PlayerID = 0x88;

            RAIDOS::Spread = 0x84;
            RAIDOS::ReloadMultiplier = 0x134;
            RAIDOS::AmmoType = 0x20;
            //RAIDOS::AmmoExplosiveType = 0x24;
 
            //RAIDOS::WeaponName = 0x5F0;
            RAIDOS::InVehicle = 0x146B;
            RAIDOS::Range = 0x28C;

            break;
        #pragma endregion
        #pragma region version_2545
        case 158:
            RAIDOS::EntityType = 0x10B8;
            RAIDOS::Armor = 0x14E0 + 0x50;

            RAIDOS::WeaponManager = 0x10D8;
            RAIDOS::PlayerID = 0x88;

            RAIDOS::PlayerInfo = 0x10C8;
            RAIDOS::Recoil = 0x2F4;
            RAIDOS::Spread = 0x84;
            RAIDOS::ReloadMultiplier = 0x134;
            RAIDOS::AmmoType = 0x20;
            //RAIDOS::AmmoExplosiveType = 0x24;

            //RAIDOS::WeaponName = 0x5F0;
            RAIDOS::InVehicle = 0x146B;
            RAIDOS::Range = 0x28C;

            break;
        #pragma endregion
        #pragma region version_2372
        case 157:
            RAIDOS::EntityType = 0x10B8;
            RAIDOS::Armor = 0x14E0;

            RAIDOS::WeaponManager = 0x10D8;
            RAIDOS::PlayerInfo = 0x10C8;
            RAIDOS::Recoil = 0x2F4;
            RAIDOS::PlayerID = 0x88;

            RAIDOS::Spread = 0x84;
            RAIDOS::ReloadMultiplier = 0x134;
            RAIDOS::AmmoType = 0x20;
            //RAIDOS::AmmoExplosiveType = 0x24;

            //RAIDOS::WeaponName = 0x5F0;
            RAIDOS::InVehicle = 0x146B;
            RAIDOS::Range = 0x28C;

            break;
        #pragma endregion
        #pragma region version_2189
        case 152:
            RAIDOS::EntityType = 0x10B8;
            RAIDOS::Armor = 0x14E0;

            RAIDOS::WeaponManager = 0x10D8;
            RAIDOS::PlayerInfo = 0x10C8;
            RAIDOS::PlayerID = 0x68;

            RAIDOS::Recoil = 0x2F4;
            RAIDOS::Spread = 0x84;
            RAIDOS::ReloadMultiplier = 0x134;
            RAIDOS::AmmoType = 0x20;
            //RAIDOS::AmmoExplosiveType = 0x24;

            //RAIDOS::WeaponName = 0x5F0;
            RAIDOS::InVehicle = 0x146B;
            RAIDOS::Range = 0x28C;

            break;
        #pragma endregion
        #pragma region version_2060
        case 151:
            RAIDOS::EntityType = 0x10B8;
            RAIDOS::Armor = 0x14E0;

            RAIDOS::WeaponManager = 0x10D8;
            RAIDOS::PlayerInfo = 0x10B8;
            RAIDOS::PlayerID = 0x88;

            RAIDOS::Recoil = 0x2F4;
            RAIDOS::Spread = 0x84;
            RAIDOS::ReloadMultiplier = 0x134;
            RAIDOS::AmmoType = 0x20;
            //RAIDOS::AmmoExplosiveType = 0x24;

            //RAIDOS::WeaponName = 0x5F0;
            RAIDOS::InVehicle = 0x146B;
            RAIDOS::Range = 0x28C;

            break;
        #pragma endregion
        #pragma region version_1604_2628
        case 146:
            RAIDOS::EntityType = 0x10A8;
            RAIDOS::Armor = 0x14B8;

            RAIDOS::WeaponManager = 0x10C8;
            RAIDOS::PlayerInfo = 0x10B8;
            RAIDOS::PlayerID = 0x68;

            RAIDOS::Recoil = 0x2E8;
            RAIDOS::Spread = 0x74;
            RAIDOS::ReloadMultiplier = 0x12C;
            RAIDOS::AmmoType = 0x20;
            //RAIDOS::AmmoExplosiveType = 0x24;
            
            //RAIDOS::WeaponName = 0x5E0;
            RAIDOS::InVehicle = 0x146B;
            RAIDOS::Range = 0x25C;
            break;
        #pragma endregion
    }

    /*MessageBox(0, "1.59 sürümü", "", MB_OKCANCEL | MB_ICONHAND);*/
}

void target_window()
{

}

#pragma endregion

HRESULT __stdcall PresentHook(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags)
{
	if (Core::FirstTime)
	{
		if (!BindD3DInfo(SwapChain))
			return DirectX::OriginalPresent(SwapChain, SyncInterval, Flags);





        HIDECONSOLE();

    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (GetAsyncKeyState(Settings::misc::Menu_Hotkey) & 1) {
        try {
            Core::Open = !Core::Open;
        }
        catch (...) {}
    }

    ImGui::GetIO().MouseDrawCursor = Core::Open;
    ImGui::GetIO().WantCaptureKeyboard = Core::Open;

    
        //POINT mouse;
        //RECT rc = { 0 };
        //GetWindowRect(DirectX::Window, &rc);

        Aimbot::Hook();
        Visuals::Hook();
        Players::Hook();
        Weapon::Hook();
        NoClip::Hook();
        Veh::Hook();

        if (Core::Open)
        {
            Core::Drawing();

            //PAD::DISABLE_ALL_CONTROL_ACTIONS(0); //dtc
        }
        else
        {
            Core::Open = false;
        }

    if (Settings::Aimbot::crosshair)
    {
        ImColor cross = { 0, 0, 0 ,255 };
        cross = Settings::Visuals::CrosshairColor;
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(955, 540), ImVec2(965, 540), ImColor(cross), 1);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(960, 535), ImVec2(960, 545), ImColor(cross), 1);
    }

    if (Settings::Aimbot::Draw_Fov)
    {
        if (Settings::Aimbot::Fovid == 0)
            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(RAIDOS::SizeX / 2, RAIDOS::SizeY / 2), Settings::Aimbot::AimbotFov * 10, Settings::Visuals::FovColor, 90.F);
        else if (Settings::Aimbot::Fovid == 1)
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RAIDOS::SizeX / 2, RAIDOS::SizeY / 2), Settings::Aimbot::AimbotFov * 10, ImColor(0, 0, 0, Settings::Visuals::FovAlpha), 90.F); //90.F
        //else if (Settings::Aimbot::Fovid == 2)
            //ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(RAIDOS::SizeX / 2, RAIDOS::SizeY / 2), Settings::Aimbot::AimbotFov * 10, ImColor(Settings::Visuals::FovColored), 90.F); //90.F
    }

    if (Settings::misc::Hitsound)
    {
    }

    if (Settings::misc::isPanic)
    {
        static bool Open = NULL;
        static bool FirstTime = NULL;
        int HotkeyArmor = NULL;
        int HotkeyHealth = NULL;
        int ForwardHotkey = NULL;
        int BackwardHotkey = NULL;
        bool Aimbot = false;
        bool visualsenable = false;
        bool extendedesp = false;
        bool playersenable = false;
        bool vehicleenable = false;
        bool crosshair = false;
        bool NoClip = false;
        int noclipspeed = false;
        bool Gravity = false;
        bool fastrun = false;
        int RunSpeedToSet = 1;
        bool SetHealth = false;
        bool SetArmor = false;
        bool InfiniteAmmo = false;
        bool DamageBoost = false;
        //bool FastNoReload = false;
        bool NoReload = false;
        bool No_Recoil = false;
        bool No_Spread = false;
        bool NoRangeLimit = false;
        bool rapidfire = false;
        bool Invisible = false;
        bool AntiHS = false;
        bool EnableFovChange = false;
        bool TpToWaypoint = false;
        bool NoRagDoll = false;
        bool NoClipSpeed_bool = false;
        int Speed = 0;
        bool Box = false;
        bool Filled = false;
        bool Corner = false;
        bool Skeleton = false;
        bool Snapline = false;
        bool flags = false;
        bool HealthBar = false;
        bool ArmorBar = false;
        bool Weapon = false;
        bool DistanceESP = false;
        bool NameEsp = false;
        bool PreviewEsp = false;
        bool PlayerName = false;
        bool VisibleCheck = false;
        float CornerSize = 1;
        float BoxSize = 1;
        int HealthBar_idx = 0;
        bool HealthBar_Multicolor = false;
        float BoneThick = 1.f;
        bool Keybinds = false;
        bool Watermark = false;
        bool IgnorePlayers = false;
        bool Draw_LocalPlayer = false;
        bool Draw_Ped = false;
        bool stickycar = false;
        bool godmode = false;
        bool fastspeed = false;
        bool setmaxhealth = false;
        bool unlockcar = false;
        bool autostart_vehicle = false;
        bool warp_vehicle = false;
        bool repaire = false;
        bool shift_boost = false;
        bool vehicle_speed_bool = false;
        bool max_current_vehicle = false;
        bool on_car = false;
        bool NameESP = false;
        bool spawn_vehicle = false;
        bool update_colors = false;
        bool update_plate = false;
        bool menu = NULL;
        int Menu_Hotkey = NULL; //F5
        int NoclipKey = NULL;
        bool PedList = false;
    }



    ImGui::Render();


    DirectX::pContext->OMSetRenderTargets(1, &DirectX::renderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return DirectX::OriginalPresent(SwapChain, SyncInterval, Flags);

}




void Core::Loader() {
    if (Core::Open) {
        
        static heads tab{ online };
        static sub_heads subtab{ online1 };

        const char* tab_name = tab == online ? skCrypt("Welcome") : 0;
        const char* tab_icon = tab == online ? skCrypt("C") : 0;

        ImGui::SetNextWindowSize({ 460, 320 });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });


        ImGui::Begin(skCrypt("Deep"), nullptr, ImGuiWindowFlags_NoDecoration); {
            auto draw = ImGui::GetWindowDrawList();

            auto pos = ImGui::GetWindowPos();
            auto size = ImGui::GetWindowSize();

            ImGuiStyle style = ImGui::GetStyle();

            draw->AddRectFilled(pos, ImVec2(pos.x + 210, pos.y + size.y), ImColor(24, 24, 26), style.WindowRounding, ImDrawFlags_RoundCornersLeft);
            draw->AddLine(ImVec2(pos.x + 210, pos.y + 2), ImVec2(pos.x + 210, pos.y + size.y - 2), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
            draw->AddLine(ImVec2(pos.x + 47, pos.y + 2), ImVec2(pos.x + 47, pos.y + size.y - 2), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
            draw->AddLine(ImVec2(pos.x + 2, pos.y + 47), ImVec2(pos.x + 47, pos.y + 47), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
            draw->AddLine(ImVec2(pos.x + 63, pos.y + 47), ImVec2(pos.x + 195, pos.y + 47), ImColor(1.0f, 1.0f, 1.0f, 0.03f));

            draw->AddText(tab_title_icon, 18.0f, ImVec2(pos.x + 65, pos.y + 14), ImColor(128, 128, 128), tab_icon);
            draw->AddText(tab_title, 19.0f, ImVec2(pos.x + 93, pos.y + 15), ImColor(1.0f, 1.0f, 1.0f), tab_name);

            draw->AddRect(pos + ImVec2(1, 1), pos + size - ImVec2(1, 1), ImColor(1.0f, 1.0f, 1.0f, 0.03f), style.WindowRounding);

            ImGui::SetCursorPos({ 8, 56 });
            ImGui::BeginGroup(); {
                if (elements::tab(skCrypt("C"), tab == online)) { tab = online; }
            } ImGui::EndGroup();

            switch (tab)
            {
            case online:
                draw->AddText(subtab_title, 15.0f, ImVec2(pos.x + 72, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.4f), skCrypt("Deep")); //cheatname

                ImGui::SetCursorPos({ 57, 86 });
                ImGui::BeginGroup(); {
                    if (elements::subtab(skCrypt("Sign In"), subtab == online1)) { subtab = online1; }
                    if (elements::subtab(skCrypt("Sign Up"), subtab == online2)) { subtab = online2; }
                }
                ImGui::EndGroup();

                switch (subtab) {
                case online1:
                    ImGui::SetCursorPos({ 226, 16 });

                    e_elements::begin_child(skCrypt("Sign In"), ImVec2(220, 290)); {
                        ImGui::Text(skCrypt("Username : "));
                       
                        ImGui::Text(skCrypt("Password : "));
                       
                        ImGui::Spacing();
                        ImGui::Checkbox(skCrypt("Remember Me"), &remembered);
                        ImGui::Spacing();
                        if (ImGui::Button(skCrypt("Login"))) { Core::GetOffset(); }
                        
                    }
                    e_elements::end_child();
                    break;

                case online2:
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child(skCrypt("Sign Up"), ImVec2(220, 290)); {
                        ImGui::Text(skCrypt("Username : "));
                       
                        ImGui::Text(skCrypt("Password : "));
                        
                        ImGui::Text(skCrypt("license : "));
                        
                        ImGui::Spacing();

                        if (ImGui::Button(skCrypt("Register"))) { Core::GetOffset(); }
                    }
                    e_elements::end_child();


                    break;
                }

                /*
                                                ImGui::Spacing();
                                ImGui::Text("Select a game");
                                ImGui::Combo(skCrypt("##Cheats"), &style_idx, cheats, IM_ARRAYSIZE(cheats), 2);
                                ImGui::SameLine();
                                if (ImGui::Button(skCrypt("Inject"))) { St3rt(); }

                                ImGui::Spacing();
                                ImGui::Text("	");
                                ImGui::Text(skCrypt("Welcome : %s"), username);
                                ImGui::Text("Status : UD");
                                ImGui::Text("Build : Prebeta");
                                ImGui::Text(skCrypt("Version : %s"), Menu::project_version);*/

                break;
            }

        }
        ImGui::End();

        ImGui::PopStyleVar();

        // Rendering
        ImGui::EndFrame();
    }
}







void Core::Drawing(void)
{
    static heads tab{ aim };
    static sub_heads subtab{ aim1 };

    const char* tab_name = tab == aim ? skCrypt("Aimbot") : tab == online ? skCrypt("Online") : tab == visuals ? skCrypt("Visuals") : tab == settings ? skCrypt("Settings") : tab == weapons ? skCrypt("Weapons") : tab == configs ? skCrypt("Configs") : tab == scripts ? "Scripts" : 0;
    const char* tab_icon = tab == aim ? skCrypt("B") : tab == online ? skCrypt("C") : tab == visuals ? skCrypt("D") : tab == settings ? skCrypt("E") : tab == weapons ? skCrypt("F") : tab == configs ? skCrypt("G") : tab == scripts ? skCrypt("H") : 0;

    ImGui::SetNextWindowSize({ 730, 460 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

    ImGui::Begin(skCrypt("hi world"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar); {
        auto draw = ImGui::GetWindowDrawList();

        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();

        ImGuiStyle style = ImGui::GetStyle();

        draw->AddRectFilled(pos, ImVec2(pos.x + 210, pos.y + size.y), ImColor(24, 24, 26), style.WindowRounding, ImDrawFlags_RoundCornersLeft);
        draw->AddLine(ImVec2(pos.x + 210, pos.y + 2), ImVec2(pos.x + 210, pos.y + size.y - 2), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
        draw->AddLine(ImVec2(pos.x + 47, pos.y + 2), ImVec2(pos.x + 47, pos.y + size.y - 2), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
        draw->AddLine(ImVec2(pos.x + 2, pos.y + 47), ImVec2(pos.x + 47, pos.y + 47), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
        draw->AddLine(ImVec2(pos.x + 63, pos.y + 47), ImVec2(pos.x + 195, pos.y + 47), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
       // draw->AddText(logo, 21.0f, ImVec2(pos.x + 14, pos.y + 12), ImColor(128, 128, 128), "A");

        draw->AddText(tab_title_icon, 18.0f, ImVec2(pos.x + 65, pos.y + 14), ImColor(128, 128, 128), tab_icon);
        draw->AddText(tab_title, 19.0f, ImVec2(pos.x + 93, pos.y + 15), ImColor(1.0f, 1.0f, 1.0f), tab_name);

        draw->AddRect(pos + ImVec2(1, 1), pos + size - ImVec2(1, 1), ImColor(1.0f, 1.0f, 1.0f, 0.03f), style.WindowRounding);

        hk_World* World = (hk_World*)*(uint64_t*)(RAIDOS::World);
        if (!World) return;

        hk_Ped* LocalPlayer = World->LocalPlayer();
        auto numResources = CFX::GET_NUM_RESOURCES();
        //if (!LocalPlayer) return;

        ImGui::SetCursorPos({ 8, 56 });
        ImGui::BeginGroup(); {
            if (elements::tab(skCrypt("B"), tab == aim)) { tab = aim; }
            if (elements::tab(skCrypt("C"), tab == online)) { tab = online; }
            if (elements::tab(skCrypt("D"), tab == visuals)) { tab = visuals; }
            if (elements::tab(skCrypt("E"), tab == settings)) { tab = settings; }
            if (elements::tab(skCrypt("F"), tab == weapons)) { tab = weapons; }
            if (elements::tab(skCrypt("G"), tab == configs)) { tab = configs; }
            if (elements::tab(skCrypt("H"), tab == scripts)) { tab = scripts; }
        } ImGui::EndGroup();

        switch (tab) 
        {
            /// Aim

            case aim:
                draw->AddText(subtab_title, 15.0f, ImVec2(pos.x + 72, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.4f), skCrypt("Deep")); //cheatname

                ImGui::SetCursorPos({ 57, 86 });
                ImGui::BeginGroup(); {
                    if (elements::subtab(skCrypt("General"), subtab == aim1)) { subtab = aim1; }
                    //if (elements::subtab("Exploits", subtab == aim2)) { subtab = aim2; }
                } 
                ImGui::EndGroup();

            switch (subtab) {
            case aim1:
                ImGui::SetCursorPos({ 226, 16 });
                e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                    ImGui::Checkbox(skCrypt("Enable Aimbot"), &Settings::Aimbot::Aimbot);
                    ImGui::Raidkey(skCrypt("Hotkey      "), &Settings::Aimbot::Hotkey, ImVec2(125, 25));

                    ImGui::Separator();

                    // if (Settings::Aimbot::Aimbot == false)
                    //ImGui::BeginDisabled();
                    //ImGui::Text(skCrypt("Aimbot:"));

                    ImGui::Checkbox((skCrypt("Draw Fov")), &Settings::Aimbot::Draw_Fov);
                    if (Settings::Aimbot::Draw_Fov)
                    {
                        if (Settings::Aimbot::Fovid == 0) 
                        {
                            ImGui::SameLine();
                            ImGui::Text(skCrypt(" "));
                            ImGui::SameLine();
                            ImGui::ColorEdit4((skCrypt("##ColorBox")), Settings::TempColor::fov, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                            Settings::Visuals::FovColor.Value.x = Settings::TempColor::fov[0];
                            Settings::Visuals::FovColor.Value.y = Settings::TempColor::fov[1];
                            Settings::Visuals::FovColor.Value.z = Settings::TempColor::fov[2];
                            Settings::Visuals::FovColor.Value.w = Settings::TempColor::fov[3];  
                        }
                        else if (Settings::Aimbot::Fovid == 1)
                        {
                            ImGui::SliderInt((skCrypt("Circle FOV")), &Settings::Visuals::FovAlpha, 1, 255, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        }
                        /*else if (Settings::Aimbot::Fovid == 2)
                        {
                            ImGui::SameLine();
                            ImGui::Text(skCrypt(" "));
                            ImGui::SameLine();
                            ImGui::ColorEdit4((skCrypt("##ColorBoy")), Settings::TempColor::foved, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                            Settings::Visuals::FovColored.Value.x = Settings::TempColor::foved[0];
                            Settings::Visuals::FovColored.Value.y = Settings::TempColor::foved[1];
                            Settings::Visuals::FovColored.Value.z = Settings::TempColor::foved[2];
                            Settings::Visuals::FovColored.Value.w = Settings::TempColor::foved[3];
                        }*/

                        ImGui::Combo(skCrypt(("Fov Type")), &Settings::Aimbot::Fovid, Settings::Aimbot::FovType, IM_ARRAYSIZE(Settings::Aimbot::FovType), ImGuiComboFlags_NoArrowButton);
                    }

                    ImGui::Checkbox(skCrypt("Draw Crosshair"), &Settings::Aimbot::crosshair);
                    if (Settings::Aimbot::crosshair)
                    {

                        ImGui::SameLine();
                        ImGui::Text(skCrypt(" "));
                        ImGui::SameLine();
                        ImGui::ColorEdit4((skCrypt("##ColorBo")), Settings::TempColor::crosshair, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                        Settings::Visuals::CrosshairColor.Value.x = Settings::TempColor::crosshair[0];
                        Settings::Visuals::CrosshairColor.Value.y = Settings::TempColor::crosshair[1];
                        Settings::Visuals::CrosshairColor.Value.z = Settings::TempColor::crosshair[2];
                        Settings::Visuals::CrosshairColor.Value.w = Settings::TempColor::crosshair[3];

                    }

                    ImGui::Checkbox((skCrypt("Visible Only")), &Settings::Aimbot::VisibleCheck);

                    ImGui::Checkbox((skCrypt("Hitsound (Updating)")), &Settings::misc::Hitsound);


                    // if (Settings::Aimbot::Aimbot == false)
                    //ImGui::EndDisabled();
                }
                e_elements::end_child();

                ImGui::SetCursorPos({ 476, 16 });
                e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                    if (Settings::Aimbot::Aimbot == false)
                        ImGui::BeginDisabled();

                    ImGui::Combo(skCrypt(("Aim Type")), &Settings::Aimbot::AimbotTypes, Settings::Aimbot::AimbotType, IM_ARRAYSIZE(Settings::Aimbot::AimbotType), ImGuiComboFlags_NoArrowButton);
                    ImGui::Combo(skCrypt(("Target Type")), &Settings::Aimbot::TypeOfTarget, Settings::Aimbot::AimbotTarget, IM_ARRAYSIZE(Settings::Aimbot::AimbotTarget), ImGuiComboFlags_NoArrowButton);
                    ImGui::Combo(skCrypt(("Target Bone")), &Settings::Aimbot::AimbotBone, Settings::Aimbot::AimbotBones, IM_ARRAYSIZE(Settings::Aimbot::AimbotBones), ImGuiComboFlags_NoArrowButton);

                    ImGui::SliderInt((skCrypt("Field Of View")), &Settings::Aimbot::AimbotFov, 1, 1200 / 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                    ImGui::SliderInt((skCrypt("Smooth")), &Settings::Aimbot::AimbotSmooth, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput); ;
                    ImGui::SliderFloat((skCrypt("Distance")), &Settings::Aimbot::DistanceAimbotLimit, 1, 2000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                    if (Settings::Aimbot::AimbotTypes == 2)
                    {
                        ImGui::SliderInt((skCrypt("Magic Bullet Damage")), &Settings::Aimbot::MagicDamage, 50, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                    }


                    //if (Settings::Aimbot::AimbotTypes == 0)
                        //{
                        //ImGui::Checkbox(skCrypt("Prediction // TriggerBot"), &Settings::Aimbot::TriggerBot);
                    //}


                    if (!Settings::Aimbot::Aimbot)
                        ImGui::EndDisabled();
                }
                e_elements::end_child();
            break;

            case aim2:
                break;
            }

            break;

            /// Online

            case online:
                draw->AddText(subtab_title, 15.0f, ImVec2(pos.x + 72, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.4f), skCrypt("Deep")); //cheatname

                ImGui::SetCursorPos({ 57, 86 });
                ImGui::BeginGroup(); {
                    if (elements::subtab(skCrypt("Self"), subtab == online1)) { subtab = online1; }
                    if (elements::subtab(skCrypt("Players (Risky)"), subtab == online2)) { subtab = online2; }
                    if (elements::subtab(skCrypt("Vehicles (Risky)"), subtab == online3)) { subtab = online3; }
                }
                ImGui::EndGroup();

                switch (subtab) {
                case online1:
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {

                        ImGui::Checkbox(skCrypt("Godmode"), &Settings::Player::Godmod);

                        ImGui::Checkbox(skCrypt("Semi-Godmode"), &Settings::Player::Semi_Godmod);

                        ImGui::Checkbox(skCrypt("Invisibility"), &Settings::Player::Invisible);

                        ImGui::Checkbox(skCrypt("No Ragdoll"), &Settings::Player::NoRagDoll);

                        ImGui::Checkbox(skCrypt("Anti HS (Risky)"), &Settings::Player::AntiHS);

                        ImGui::Checkbox(skCrypt("Unlimited Armor"), &Settings::Player::Infinite_Armor);

                        ImGui::Checkbox(skCrypt("Speedster"), &Settings::Player::fastrun);

                        if (Settings::Player::fastrun)
                        {
                            ImGui::SliderInt((skCrypt("Walking Speed")), &Settings::Player::RunSpeedToSet, 0, 20, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        }

                        //ImGui::Checkbox(skCrypt("Super Jump"), &Settings::Player::SuperJump);
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 476, 16 });
                    e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                        ImGui::Checkbox(skCrypt("Enable NoClip"), &Settings::Player::NoClip);

                        if (Settings::Player::NoClip)
                        {
                            ImGui::Raidkey(skCrypt("Forward  "), &Settings::Player::ForwardHotkey);
                            ImGui::Raidkey(skCrypt("Backward"), &Settings::Player::BackwardHotkey);
                            ImGui::Checkbox(skCrypt("Speed"), &Settings::Player::NoClipSpeed_bool);
                            if (Settings::Player::NoClipSpeed_bool)
                            {
                                ImGui::SliderInt((skCrypt("Speed")), &Settings::Player::Speed, 0, 40, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                            }
                        }

                        ImGui::SliderFloat((skCrypt("FOV")), &Settings::Player::FovValue, 50, 120, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        if (ImGui::Button(skCrypt("Set FOV")))
                        {
                            Settings::Player::EnableFovChange = true;
                        }

                        //ImGui::Checkbox(skCrypt("No Gravity"), &Settings::Player::Gravity);

                        ImGui::SliderInt((skCrypt("Health")), &Settings::Player::HealthToSet, 0, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        ImGui::SliderInt((skCrypt("Armor")), &Settings::Player::ArmorToSet, 0, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                        if (ImGui::Button(skCrypt("Health ##can")))
                        {
                           
                            Settings::Player::SetHealth = true;
                        }
                        ImGui::SameLine();
                        if (ImGui::Button(skCrypt("Armor ##armor")))
                        {
                           
                            Settings::Player::SetArmor = true;
                        }
                        ImGui::SameLine();
                        if (ImGui::Button(skCrypt("TP Point (Risky)")))
                        {
                            
                            Settings::Player::TpToWaypoint = true;
                        }
                    }
                    e_elements::end_child();
                    break;


                    
                case online2:
                    if (LocalPlayer)
                    {
                        hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
                        if (!Nav)
                            return;

                        hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(RAIDOS::ReplayInterface);
                        if (!ReplayInterface)
                            return;

                        hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
                        if (!PedInterface)
                            return;

                        ImGui::SetCursorPos({ 226, 16 });
                        e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                            static ImGuiTextFilter pedfilter;
                            pedfilter.Draw(skCrypt("P"), 240);

                            if (ImGui::ListBoxHeader(skCrypt("##PlayersList"), ImVec2(210, 330)))
                            {
                                for (uint64_t ip = 0; ip < PedInterface->PedMaximum(); ip++) {

                                    hk_Ped* Peds = PedInterface->PedList()->Ped(ip);
                                    if (!Peds) continue;
                                    int PlayerID = RAIDOS::pointer_to_handle((DWORD64)Peds);

                                    auto playerName = GetPlayerNameNew((DWORD64)Peds);
                                    const char* c = playerName.c_str();
                                    const char* items[] = { c };
                                    if (playerName == "Peds")
                                    {
                                        if (!Settings::misc::PedList)
                                            continue;
                                        playerName = "Peds ##" + std::to_string(ip);

                                    }

                                    bool is_selected = (selected_ped == ip);

                                    if (pedfilter.PassFilter(playerName.c_str()))
                                    {
                                        if (ImGui::Selectable(playerName.c_str(), is_selected))
                                        {
                                            selected_ped = ip;
                                        }
                                    }
                                }

                                ImGui::ListBoxFooter();
                            }
                        }
                        e_elements::end_child();

                        ImGui::SetCursorPos({ 476, 16 });
                        e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {

                            std::string SelectedString = "Selected one is : " + std::to_string(selected_ped);
                            ImGui::Text(SelectedString.c_str());
                            ImGui::Checkbox(skCrypt(("Show Ped ")), &Settings::misc::PedList);
                            hk_Ped* SelectedPed = PedInterface->PedList()->Ped(selected_ped);

                            if (SelectedPed->GetCoordinate().x != 0)
                            {
                                if (ImGui::TreeNode(skCrypt("Trolls")))
                                {
                                    /*if (ImGui::Button("Explode (Risk)"))
                                    {

                                    }*/

                                    if (ImGui::Button(skCrypt("Give All Weapons (Risky)")))
                                    {
                                        for (int i = 0; i < std::size(weaponsList); i++)
                                        {
                                            WEAPON::GIVE_DELAYED_WEAPON_TO_PED(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), weaponsList[i], (int)9999, 1); //dtc
                                        }
                                    }

                                    /*if (ImGui::Button(skCrypt("Remove All Weapons (Risky)")))
                                    {
                                        WEAPON::REMOVE_ALL_PED_WEAPONS(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), true); //dtc
                                    }

                                    if (ImGui::Button("Taze Player"))
                                    {
                                    }*/

                                    if (ImGui::Button(skCrypt("Kill Him (Risky)"))) //dtc
                                    {
                                        
                                        
                                        auto player = PLAYER::PLAYER_PED_ID();
                                        auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
                                        auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0x5EF9FEC4, NULL);
                                        MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)WeaponDamage, false, 0xBFEFFF6D, player, true, false, 3.f);
                                    }

                                    /*
                                    if (ImGui::Button(skCrypt("Destroy Vehicle"))) //dtc
                                    {
                                        auto vehicle = PED::GET_VEHICLE_PED_IS_IN(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), false);

                                        VEHICLE::SET_VEHICLE_BODY_HEALTH(vehicle, -4000);
                                        VEHICLE::SET_VEHICLE_ENGINE_HEALTH(vehicle, -4000);
                                        VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(vehicle);
                                        VEHICLE::SET_VEHICLE_TYRE_FIXED(vehicle, true);

                                    }
                                    ImGui::SameLine();
                                    if (ImGui::Button(skCrypt("Repair Vehicle"))) //dtc
                                    {
                                        auto vehicle = PED::GET_VEHICLE_PED_IS_IN(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), false);

                                        VEHICLE::SET_VEHICLE_BODY_HEALTH(vehicle, 1000);
                                        VEHICLE::SET_VEHICLE_ENGINE_HEALTH(vehicle, 1000);
                                        VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(vehicle);
                                        VEHICLE::SET_VEHICLE_TYRE_FIXED(vehicle, true);
                                    }
                                     
                                    if (ImGui::Button(skCrypt("Lock vehicle")))//dtc
                                    {
                                        auto vehicle = PED::GET_VEHICLE_PED_IS_IN(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), false);
                                        VEHICLE::SET_VEHICLE_DOORS_LOCKED(vehicle, 4);
                                    }
                                    ImGui::SameLine();
                                    if (ImGui::Button(skCrypt("Unlock vehicle"))) //dtc
                                    {
                                        auto vehicle = PED::GET_VEHICLE_PED_IS_IN(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), false);
                                        VEHICLE::SET_VEHICLE_DOORS_LOCKED(vehicle, 1);
                                    }

                                    if (ImGui::Button(skCrypt("HandCuff"))) //dtc
                                    {
                                        PED::SET_ENABLE_HANDCUFFS(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), true);
                                    }
                                    ImGui::SameLine();
                                    if (ImGui::Button(skCrypt("HandCuff Take Off"))) //dtc
                                    {
                                        PED::SET_ENABLE_HANDCUFFS(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), false);
                                    }

                                    if (ImGui::Button(skCrypt("Clone pedestrian"))) //dtc
                                    {
                                        auto player = PLAYER::PLAYER_PED_ID();
                                        PED::CLONE_PED_TO_TARGET(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), player);

                                    }
                                    if (ImGui::Button(skCrypt("Taze player"))) //dtc
                                    {
                                        auto player = PLAYER::PLAYER_PED_ID();
                                        auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
                                        auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0x8BB05FD7, NULL);
                                        MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)WeaponDamage, false, 0x8BB05FD7, player, true, false, 3.f);
                                    }
                                    */
                                    ImGui::TreePop();
                                }
                                if (ImGui::TreeNode(skCrypt("Vehicle Trolls"))) {
                                    /*if (ImGui::Button("Make Vehicle Fly!"))
                                    {
                                    }*/
                                    if (ImGui::Button(skCrypt("Stop Vehicle (Risky)"))) //dtc
                                    {
                                        

				                        int vehicle = PED::GET_VEHICLE_PED_IS_IN(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), false);
				                        if(PED::IS_PED_IN_ANY_VEHICLE(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), true))
				                        {
					                        //SET_VEHICLE_FORWARD_SPEED(player, 0.0f);
					                        //APPLY_FORCE(vehicle, 0, 0, 0, 0, 0, 0);
					                        ENTITY::FREEZE_ENTITY_POSITION(vehicle, true);
					                        //sleep(50);
                                            ENTITY::FREEZE_ENTITY_POSITION(vehicle, false);
				                        }

                                        //auto vehicle = PED::GET_VEHICLE_PED_IS_IN(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), false);
                                        //VEHICLE::SET_VEHICLE_ENGINE_HEALTH(vehicle, -4000);
                                    }
                                    
                                    /*
                                    if (ImGui::Button("Delete Vehicle (Coming Soon)"))
                                    {

                                    }*/
                                    if (ImGui::Button(skCrypt("Explode Tires (Risky)"))) //dtc
                                    {
                                        

                                        auto vehicle = PED::GET_VEHICLE_PED_IS_IN(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), false);

                                        if (PED::IS_PED_IN_ANY_VEHICLE(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), true))
                                        {
                                            for (int tire = 0; tire <= 7; tire++)
                                                VEHICLE::SET_VEHICLE_TYRE_BURST(vehicle, tire, true, 1000.0);
                                        }
                                        else
                                        {
                                            
                                        }
                                    }
                                    ImGui::TreePop();
                                }
                                if (ImGui::TreeNode(skCrypt("Utils")))
                                {
                                    if (ImGui::Button(skCrypt("Heal"))) //dtc
                                    {
                                        

                                        SelectedPed->SetMaxHealth();
                                    }
                                    ImGui::SameLine();
                                    if (ImGui::Button(skCrypt("Warp (Risky)"))) //dtc
                                    {
                                       

                                        auto player = PLAYER::PLAYER_PED_ID();
                                        auto vehicle = PED::GET_VEHICLE_PED_IS_IN(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), false);

                                        if (VEHICLE::IS_VEHICLE_SEAT_FREE(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), -1, false)) {
                                            PED::SET_PED_INTO_VEHICLE(player, RAIDOS::pointer_to_handle((DWORD64)SelectedPed), -1);
                                        }
                                        else if (VEHICLE::IS_VEHICLE_SEAT_FREE(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 0, false)) {
                                            PED::SET_PED_INTO_VEHICLE(player, RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 0);
                                        }
                                        else if (VEHICLE::IS_VEHICLE_SEAT_FREE(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 1, false)) {
                                            PED::SET_PED_INTO_VEHICLE(player, RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 1);
                                        }
                                        else if (VEHICLE::IS_VEHICLE_SEAT_FREE(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 2, false)) {
                                            PED::SET_PED_INTO_VEHICLE(player, RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 2);
                                        }
                                    }
                                   

                                    if (ImGui::Button(skCrypt("Teleport To Player")))
                                    {
                                        

                                        if (SelectedPed != LocalPlayer)
                                        {
                                            Nav->SetRotation({ 0, 0, 0, 0 });
                                            LocalPlayer->SetVelocity();
                                            LocalPlayer->SetCoordinate(SelectedPed->GetCoordinate());
                                            Nav->SetCoordinate(SelectedPed->GetCoordinate());
                                        }

                                    }

                                    /* if (ImGui::Button("Steal Outfit (Coming Soon)"))
                                    {
                                    }
                                    */


                                    if (ImGui::Button(skCrypt("Spectate (Risky)"))) //dtc
                                    {
                                        

                                        NETWORK::NETWORK_SET_IN_SPECTATOR_MODE((bool)true, RAIDOS::pointer_to_handle((DWORD64)SelectedPed));

                                    }
                                    ImGui::SameLine();
                                    if (ImGui::Button(skCrypt("Off Spectate (Risky)"))) //dtc
                                    {
                                        NETWORK::NETWORK_SET_IN_SPECTATOR_MODE((bool)false, RAIDOS::pointer_to_handle((DWORD64)SelectedPed));
                                    }
                                    


                                   // int PlayerID = RAIDOS::pointer_to_handle((DWORD64)SelectedPed);

                                    if (ImGui::Button(skCrypt("Soon")))
                                    {
                                        //std::cout << RAIDOS::pointer_to_handle((DWORD64)SelectedPed);
                                        //std::cout << skCrypt(" + ");
                                        //std::cout << std::to_string(RAIDOS::pointer_to_handle((DWORD64)SelectedPed));
                                    }
                                    ImGui::SameLine();

                                    auto it = std::find(RAIDOS::Friend.begin(), RAIDOS::Friend.end(), GetPlayerNameNew((DWORD64)SelectedPed));
                                    if (it != RAIDOS::Friend.end())
                                    {
                                        if (ImGui::Button(skCrypt("Remove Friend")))
                                        {
                                            //	auto it = std::find(RAIDOS::Friend.begin(), RAIDOS::Friend.end(), (DWORD64)SelectedPed);
                                            int index = it - RAIDOS::Friend.begin();
                                            RAIDOS::Friend.erase(RAIDOS::Friend.begin() + index);
                                        }
                                    }
                                    else
                                    {
                                        if (ImGui::Button(skCrypt("Add Friend")))
                                        {
                                            RAIDOS::Friend.push_back(GetPlayerNameNew((DWORD64)SelectedPed));
                                        }
                                    }

                                    ImGui::TreePop();
                                }

                                std::string healthtoshow = "Health : " + std::to_string(SelectedPed->GetHealth());
                                std::string armortoshow = "Armor : " + std::to_string(SelectedPed->GetArmor());
                                ImGui::Text(healthtoshow.c_str());
                                ImGui::Text(armortoshow.c_str());

                                std::string coordstoshowx = "X : " + std::to_string(SelectedPed->GetCoordinate().x);
                                std::string coordstoshowy = "Y : " + std::to_string(SelectedPed->GetCoordinate().y);
                                std::string coordstoshowz = "Z : " + std::to_string(SelectedPed->GetCoordinate().z);

                                ImGui::Text(coordstoshowx.c_str());
                                ImGui::Text(coordstoshowy.c_str());
                                ImGui::Text(coordstoshowz.c_str());
                            }
                        }
                        e_elements::end_child();
                    }
                    else
                    {
                        hk_World* World = (hk_World*)*(uint64_t*)(RAIDOS::World);
                        if (!World)
                            return;

                        hk_Ped* LocalPlayer = World->LocalPlayer();
                        //if (LocalPlayer) 
                        //{
                        //    inGame = true;
                        //}

                        ImGui::SetCursorPos({ 226, 16 });
                        e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                            ImGui::Text(skCrypt("Works only in-game")); // Only works in-game.
                        }
                        e_elements::end_child();

                        ImGui::SetCursorPos({ 476, 16 });
                        e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                            ImGui::Text(skCrypt("Works only in-game")); // Only works in-game.
                        }
                        e_elements::end_child();
                    }
                    break;


                case online3:
                    if (LocalPlayer) //LocalPlayer
                    {
                        hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
                        if (!Nav)
                            return;
                        hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(RAIDOS::ReplayInterface);
                        if (!ReplayInterface)
                            return;
                        hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface();
                        if (!VehicleInterface)
                            return;

                        ImGui::SetCursorPos({ 226, 16 });

                        e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {

                            static ImGuiTextFilter carfilter;
                            carfilter.Draw(skCrypt("S"), 240);

                            if (ImGui::ListBoxHeader(skCrypt("##VehicleList"), ImVec2(210, 330)))
                            {
                                for (int i = 0; i < VehicleInterface->VehicleMaximum(); i++) 
                                {

                                    hk_Vehicle* Peds = VehicleInterface->VehicleList()->Vehicle(i);
                                    if (!Peds) continue;
                                    auto playerName = Peds->VehicleModelInfo()->GetCharName();
                                    const char* c = playerName;
                                    const char* items[] = { c };

                                    std::string VehicleName = playerName;
                                    VehicleName.append(skCrypt(" ##"));
                                    VehicleName.append(std::to_string(i));
                                    bool is_selected = (selected_veh == i);

                                    if(carfilter.PassFilter(VehicleName.c_str())) 
                                    {
                                        if (ImGui::Selectable(VehicleName.c_str(), is_selected))
                                        {
                                            selected_veh = i;
                                        }
                                    }
                                }
                                ImGui::ListBoxFooter();
                            }

                        }
                        e_elements::end_child();

                        ImGui::SetCursorPos({ 476, 16 });
                        e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                        
                            std::string SelectedString = "Selected one is : " + std::to_string(selected_veh);
                            ImGui::Text(SelectedString.c_str());
                            hk_Vehicle* SelectedPed = VehicleInterface->VehicleList()->Vehicle(selected_veh);

                            if (ImGui::TreeNode(skCrypt("Troll")))
                            {
                                if (ImGui::Button(skCrypt("Destroy Vehicle"))) //dtc
                                {
                                    
                                    SelectedPed->SetBodyHealth(-4000);
                                    SelectedPed->SetEngineHealth(-4000);
                                    SelectedPed->SetHealth(0);
                                }
                                ImGui::SameLine();
                                if (ImGui::Button(skCrypt("Repair Vehicle (Risky)"))) //dtc
                                {
                                    
                                    SelectedPed->SetBodyHealth(1000);
                                    SelectedPed->SetEngineHealth(1000);
                                    SelectedPed->SetHealth(1000);

                                    VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(RAIDOS::pointer_to_handle((DWORD64)SelectedPed));
                                    VEHICLE::SET_VEHICLE_TYRE_FIXED(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 1);


                                    // VEHICLE::SET_DISABLE_VEHICLE_ENGINE_FIRES(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), true);
                                    // VEHICLE::_SET_VEHICLE_WHEELS_DEAL_DAMAGE(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), false);
                                    // VEHICLE::SET_VEHICLE_FIXED(RAIDOS::pointer_to_handle((DWORD64)SelectedPed));

                                }
                                if (ImGui::Button(skCrypt("Lock vehicle (Risky)"))) //dtc
                                {
                                    VEHICLE::SET_VEHICLE_DOORS_LOCKED(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 4);
                                }
                                ImGui::SameLine();
                                if (ImGui::Button(skCrypt("Unlock vehicle (Risky)"))) //dtc
                                {
                                    VEHICLE::SET_VEHICLE_DOORS_LOCKED(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 1);
                                }
                                if (ImGui::Button(skCrypt("Pollute (Risky)"))) //dtc
                                {
                                    VEHICLE::SET_VEHICLE_DIRT_LEVEL(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 15.0);
                                }
                                ImGui::SameLine();
                                if (ImGui::Button(skCrypt("Clean (Risky)"))) //dtc
                                {
                                    
                                    VEHICLE::SET_VEHICLE_DIRT_LEVEL(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 0.0);
                                }

                                ImGui::TreePop();
                            }
                            if (ImGui::TreeNode(skCrypt("Silent Troll"))) { //dtc
                                if (ImGui::Button(skCrypt("Destroy")))
                                {
                                   
                                    SelectedPed->SetBodyHealth(-4000);
                                    SelectedPed->SetEngineHealth(-4000);
                                    SelectedPed->SetHealth(0);
                                }
                                ImGui::SameLine();
                                if (ImGui::Button(skCrypt("Repair")))
                                {
                                    
                                    SelectedPed->SetBodyHealth(1000);
                                    SelectedPed->SetEngineHealth(1000);
                                    SelectedPed->SetHealth(1000);
                                }

                                ImGui::TreePop();
                            }
                            if (ImGui::TreeNode(skCrypt("Utils")))
                            {
                                if (ImGui::Button(skCrypt("TP To Him")))
                                {
                                    
                                    Nav->SetRotation({ 0, 0, 0, 0 });
                                    LocalPlayer->SetVelocity();
                                    LocalPlayer->SetCoordinate(SelectedPed->GetCoordinate());
                                    Nav->SetCoordinate(SelectedPed->GetCoordinate());
                                }
                                ImGui::SameLine();
                                if (ImGui::Button(skCrypt("Warp (Risky)"))) //dtc
                                {
                                    
                                    auto player = PLAYER::PLAYER_PED_ID(); //dtc
                                    auto vehicle = RAIDOS::pointer_to_handle((DWORD64)SelectedPed);

                                    if (VEHICLE::IS_VEHICLE_SEAT_FREE(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), -1, false)) {

                                        PED::SET_PED_INTO_VEHICLE(player, RAIDOS::pointer_to_handle((DWORD64)SelectedPed), -1);

                                    }
                                    else if (VEHICLE::IS_VEHICLE_SEAT_FREE(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 0, false)) {


                                        PED::SET_PED_INTO_VEHICLE(player, RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 0);

                                    }
                                    else if (VEHICLE::IS_VEHICLE_SEAT_FREE(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 1, false)) {


                                        PED::SET_PED_INTO_VEHICLE(player, RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 1);

                                    }
                                    else if (VEHICLE::IS_VEHICLE_SEAT_FREE(RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 2, false)) {


                                        PED::SET_PED_INTO_VEHICLE(player, RAIDOS::pointer_to_handle((DWORD64)SelectedPed), 2);

                                    }
                                }
                                /*if (ImGui::Button(skCrypt("Shoot bullet // Buggy"))) //dtc
                                {
                                    auto player = PLAYER::PLAYER_PED_ID();
                                    auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
                                    auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0xBFEFFF6D, NULL);
                                    MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)WeaponDamage, false, 0xBFEFFF6D, player, true, false, 3.f);
                                }*/
                                ImGui::TreePop();
                            }

                            std::string healthtoshow = "Health : " + std::to_string(SelectedPed->GetHealth());
                            ImGui::Text(healthtoshow.c_str());
                            std::string coordstoshowz = "X : " + std::to_string(SelectedPed->GetCoordinate().x);
                            std::string coordstoshowx = "Y : " + std::to_string(SelectedPed->GetCoordinate().y);
                            std::string coordstoshowy = "Z : " + std::to_string(SelectedPed->GetCoordinate().z);


                            ImGui::Text(coordstoshowx.c_str());
                            ImGui::Text(coordstoshowy.c_str());
                            ImGui::Text(coordstoshowz.c_str());
                        }
                        e_elements::end_child();

                    }
                    else
                    {
                        ImGui::SetCursorPos({ 226, 16 });
                        e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                            ImGui::Text(skCrypt("Only works in-game. (Risky)"));
                        }
                        e_elements::end_child();

                        ImGui::SetCursorPos({ 476, 16 });
                        e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                            ImGui::Text(skCrypt("Only works in-game. (Risky)"));
                        }
                        e_elements::end_child();
                    }
                    break;
                }

                break;

            /// Visuals

            case visuals:
                draw->AddText(subtab_title, 15.0f, ImVec2(pos.x + 72, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.4f), skCrypt("Deep")); //cheatname

                ImGui::SetCursorPos({ 57, 86 });
                ImGui::BeginGroup(); {
                    if (elements::subtab(skCrypt("ESP"), subtab == visuals1)) { subtab = visuals1; }
                    //if (elements::subtab("Others", subtab == visuals2)) { subtab = visuals2; }
                }
                ImGui::EndGroup();

                switch (subtab) {
                case visuals1:
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                        ImGui::Checkbox(skCrypt("Enable ESP"), &Settings::Visuals::visualsenable);

                        if (!Settings::Visuals::visualsenable)
                            ImGui::BeginDisabled();

                        ImGui::Checkbox((skCrypt("Extended ESP")), &Settings::Visuals::extendedesp);

                        ImGui::Checkbox((skCrypt("Box ESP")), &Settings::Visuals::Box);
                        if (Settings::Visuals::Box)
                        {
                            ImGui::SameLine();
                            ImGui::Text(" ");
                            ImGui::SameLine();
                            ImGui::ColorEdit4((skCrypt("##ColorBo")), Settings::TempColor::Box, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                            Settings::Visuals::BoxColor.Value.x = Settings::TempColor::Box[0];
                            Settings::Visuals::BoxColor.Value.y = Settings::TempColor::Box[1];
                            Settings::Visuals::BoxColor.Value.z = Settings::TempColor::Box[2];
                            Settings::Visuals::BoxColor.Value.w = Settings::TempColor::Box[3];

                            ImGui::SameLine();
                            ImGui::Text(" ");
                            ImGui::SameLine();
                            ImGui::ColorEdit4((skCrypt("##ColorBo4")), Settings::TempColor::BoxVisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                            Settings::Visuals::BoxColorVisible.Value.x = Settings::TempColor::BoxVisible[0];
                            Settings::Visuals::BoxColorVisible.Value.y = Settings::TempColor::BoxVisible[1];
                            Settings::Visuals::BoxColorVisible.Value.z = Settings::TempColor::BoxVisible[2];
                            Settings::Visuals::BoxColorVisible.Value.w = Settings::TempColor::BoxVisible[3];

                        }
                        /*
                        ImGui::Checkbox((skCrypt("Corner Box")), &Settings::Visuals::Corner);
                        if (Settings::Visuals::Corner)
                        {

                            ImGui::SameLine();
                            ImGui::Text(" ");
                            ImGui::SameLine();
                            ImGui::ColorEdit4((skCrypt("##ColorBo868")), Settings::TempColor::Corner, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                            Settings::Visuals::CornerColor.Value.x = Settings::TempColor::Corner[0];
                            Settings::Visuals::CornerColor.Value.y = Settings::TempColor::Corner[1];
                            Settings::Visuals::CornerColor.Value.z = Settings::TempColor::Corner[2];
                            Settings::Visuals::CornerColor.Value.w = Settings::TempColor::Corner[3];
                            ImGui::SameLine();
                            ImGui::Text(" ");
                            ImGui::SameLine();
                            ImGui::ColorEdit4((skCrypt("##ColorBo74545")), Settings::TempColor::CornerVisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                            Settings::Visuals::CornerColorVisible.Value.x = Settings::TempColor::CornerVisible[0];
                            Settings::Visuals::CornerColorVisible.Value.y = Settings::TempColor::CornerVisible[1];
                            Settings::Visuals::CornerColorVisible.Value.z = Settings::TempColor::CornerVisible[2];
                            Settings::Visuals::CornerColorVisible.Value.w = Settings::TempColor::CornerVisible[3];

                        }
                        if (Settings::Visuals::Box || Settings::Visuals::Corner)
                        {
                            ImGui::Checkbox((skCrypt("Fill Box")), &Settings::Visuals::Filled);
                        }
                        */
                        if (Settings::Visuals::extendedesp)
                        {
                            ImGui::Checkbox((skCrypt("Skeleton")), &Settings::Visuals::Skeleton);
                            if (Settings::Visuals::Skeleton)
                            {
                                ImGui::SameLine();
                                ImGui::Text(" ");
                                ImGui::SameLine();
                                ImGui::ColorEdit4((skCrypt("##ColorBo44")), Settings::TempColor::Skeleton, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                Settings::Visuals::SkelColor.Value.x = Settings::TempColor::Skeleton[0];
                                Settings::Visuals::SkelColor.Value.y = Settings::TempColor::Skeleton[1];
                                Settings::Visuals::SkelColor.Value.z = Settings::TempColor::Skeleton[2];
                                Settings::Visuals::SkelColor.Value.w = Settings::TempColor::Skeleton[3];
                                ImGui::SameLine();
                                ImGui::Text(" ");
                                ImGui::SameLine();
                                ImGui::ColorEdit4((skCrypt("##ColorBo1774")), Settings::TempColor::SkeletonVisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                Settings::Visuals::SkelColorVisible.Value.x = Settings::TempColor::SkeletonVisible[0];
                                Settings::Visuals::SkelColorVisible.Value.y = Settings::TempColor::SkeletonVisible[1];
                                Settings::Visuals::SkelColorVisible.Value.z = Settings::TempColor::SkeletonVisible[2];
                                Settings::Visuals::SkelColorVisible.Value.w = Settings::TempColor::SkeletonVisible[3];
                            }
                        }

                        ImGui::Checkbox((skCrypt("HealthBar")), &Settings::Visuals::HealthBar);

                        ImGui::Checkbox((skCrypt("ArmorBar")), &Settings::Visuals::ArmorBar);
                        ImGui::Checkbox((skCrypt("Snapline")), &Settings::Visuals::Snapline);

                        ImGui::Checkbox((skCrypt("Ignore Players")), &Settings::Visuals::IgnorePlayers);
                        ImGui::Checkbox((skCrypt("Draw Local")), &Settings::Visuals::Draw_LocalPlayer);
                        ImGui::Checkbox((skCrypt("Draw Peds")), &Settings::Visuals::Draw_Ped);

                        //ImGui::Checkbox((skCrypt("Flags")), &Settings::Visuals::flags);  

                        if (!Settings::Visuals::visualsenable)
                            ImGui::EndDisabled();
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 476, 16 });
                    e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                        if (!Settings::Visuals::visualsenable)
                            ImGui::BeginDisabled();

                        if (Settings::Visuals::extendedesp){
                            ImGui::SliderInt((skCrypt("Distance")), &Settings::Visuals::Distance, 1, 1000, NULL, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        }
                        else{
                            ImGui::SliderInt((skCrypt("Distance")), &Settings::Visuals::Distance, 1, 150, NULL, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        }
                        ImGui::SliderFloat((skCrypt("Box Size")), &Settings::Visuals::BoxSize, 0.5, 2, "%.1f", ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        ImGui::SliderFloat((skCrypt("Bone Size")), &Settings::Visuals::BoneThick, 1, 5, "%.1f", ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        //ImGui::SliderFloat((skCrypt("Corner Size")), &Settings::Visuals::CornerSize, 0.5, 2, "%.1f", ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                        ImGui::Checkbox((skCrypt("Player Name (Risky)")), &Settings::Visuals::PlayerName);
                        ImGui::Checkbox((skCrypt("Weapon Name (Risky)")), &Settings::Visuals::Weapon);
                        ImGui::Checkbox(skCrypt("Vehicle Name"), &Settings::vehicle::NameESP);

                        ImGui::Checkbox((skCrypt("Draw Distance")), &Settings::Visuals::DistanceESP);

                        if (Settings::Visuals::HealthBar) {

                            if (ImGui::Combo(skCrypt("Health Pos"), &Settings::Visuals::HealthBar_idx, skCrypt("Right\0Left\0Down\0Up")))
                            {
                                switch (Settings::Visuals::HealthBar_idx)
                                {
                                case 0: Settings::Visuals::HealthBar_idx = 0; break;
                                case 1: Settings::Visuals::HealthBar_idx = 1; break;
                                case 2: Settings::Visuals::HealthBar_idx = 2; break;
                                case 3: Settings::Visuals::HealthBar_idx = 3; break;
                                }
                            }
                        }

                        if (!Settings::Visuals::visualsenable)
                            ImGui::EndDisabled();
                    }
                    e_elements::end_child();
                    break;

                    /*case visuals2:
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child(skCrypt("Coming"), ImVec2(240, 430)); {
                        ImGui::Text(skCrypt("Coming Soon!"));
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 476, 16 });
                    e_elements::begin_child(skCrypt("Soon"), ImVec2(240, 430)); {
                        ImGui::Text(skCrypt("Coming Soon!"));
                    }
                    e_elements::end_child();
                    break; */
                }

                break;

            /// Settings

            case settings:
                draw->AddText(subtab_title, 15.0f, ImVec2(pos.x + 72, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.4f), skCrypt("Deep")); //cheatname

                ImGui::SetCursorPos({ 57, 86 });
                ImGui::BeginGroup(); {
                    if (elements::subtab(skCrypt("Vehicle"), subtab == settings1)) { subtab = settings1; }
                    if (elements::subtab(skCrypt("Others"), subtab == settings2)) { subtab = settings2; }
                }
                ImGui::EndGroup();

                switch (subtab) {
                case settings1:
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {

                        ImGui::Checkbox(skCrypt(("Enable Vehicle Option (Risky)")), &Settings::vehicle::vehicleenable);

                        if (!Settings::vehicle::vehicleenable)
                            ImGui::BeginDisabled();

                        ImGui::Checkbox(skCrypt("Auto Start Engine (Risky)"), &Settings::vehicle::autostart_vehicle);
                        ImGui::Checkbox(skCrypt("Force Unlock Car (Risky)"), &Settings::vehicle::unlockcar);
                        ImGui::Checkbox(skCrypt("Warp Vehicle (Risky)"), &Settings::vehicle::warp_vehicle);
                        ImGui::Checkbox(skCrypt("Sticky Car (Risky)"), &Settings::vehicle::stickycar);

                        if (!Settings::vehicle::vehicleenable)
                            ImGui::EndDisabled();

                        if (Settings::vehicle::on_car)
                        {
                            ImGui::Checkbox(skCrypt("GodMod"), &Settings::vehicle::godmode);
                            ImGui::Checkbox(skCrypt("Shift Boost"), &Settings::vehicle::shift_boost);

                            ImGui::Checkbox(skCrypt("Vehicle Speed"), &Settings::vehicle::vehicle_speed_bool);
                            if (Settings::vehicle::vehicle_speed_bool)
                            {
                                ImGui::SliderInt((skCrypt("Speed multiplier")), &Settings::vehicle::vehicle_speed_value, 0, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                            }
                            if (ImGui::Button(skCrypt("Repair (Risky)")))
                            {
                                Settings::vehicle::repaire = true;
                            }
                        }
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 476, 16 });
                    e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                        if (Settings::vehicle::on_car)
                        {
                            ImGui::SliderInt((skCrypt("Primary (Risky)")), &Settings::vehicle::primary_color, 0, 160, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                            ImGui::SliderInt((skCrypt("Secondary (Risky)")), &Settings::vehicle::secondary_color, 0, 160, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                            if (ImGui::Button(skCrypt("Update Colors (Risky)")))
                            {
                                Settings::vehicle::update_colors = true;
                            }

                            ImGui::Separator();

                            ImGui::SetCursorPosX(5);
                            ImGui::InputText(skCrypt("Plate (Risky)"), Settings::vehicle::plate_text, IM_ARRAYSIZE(Settings::vehicle::plate_text));

                            if (ImGui::Button(skCrypt("Update Plate")))
                            {

                                Settings::vehicle::update_plate = true;

                            }

                            ImGui::Separator();

                            if (ImGui::Combo(skCrypt("Vehicle Mod (Risky)"), &Settings::vehicle::mod_idx, "Spoilers\0Front Bumper\0Rear Bumper\0Side Skirt\0Exhaust\0Frame\0Grille\0Hood\0Fender\0Right Fender\0Roof\0Engine\0Brakes\0Transmission\0Horns\0Suspension\0Armor\0"))
                            {
                                switch (Settings::vehicle::mod_idx)
                                {
                                }
                            }

                            ImGui::SliderInt((skCrypt("Mod (Risky)")), &Settings::vehicle::add_idx, 0, 40, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                            if (ImGui::Button(skCrypt("Update Mods (Risky)")))
                            {
                                Settings::vehicle::max_current_vehicle = true;
                            }
                        }
                        else
                        {
                            ImGui::Text(skCrypt("Only works in-vehicle."));
                        }
                    }
                    e_elements::end_child();
                    break;

                case settings2:
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                        //ImGui::Checkbox(skCrypt("Watermark"), &Settings::Visuals::Watermark);
                        //ImGui::Checkbox(skCrypt("Coming Soon"), nullptr);
                        ImGui::Checkbox(skCrypt("Panic Mode"), &Settings::misc::isPanic);
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 476, 16 });
                    e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                        ImGui::Raidkey(skCrypt("Health :  "), &Settings::Player::HotkeyHealth);
                        ImGui::Raidkey(skCrypt("Armor  :  "), &Settings::Player::HotkeyArmor);
                        ImGui::Raidkey(skCrypt("Noclip :  "), &Settings::misc::NoclipKey);
                        ImGui::Raidkey(skCrypt("Panic  :  "), &Settings::misc::PanicKey);

                        ImGui::Spacing();
                        ImGui::Spacing();

                        ImGui::Raidkey(skCrypt("Menu   :  "), &Settings::misc::Menu_Hotkey);
                    }
                    e_elements::end_child();
                    break;
                }

                break;

            /// Weapons

            case weapons:
                draw->AddText(subtab_title, 15.0f, ImVec2(pos.x + 72, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.4f), skCrypt("Deep")); //cheatname

                ImGui::SetCursorPos({ 57, 86 });
                ImGui::BeginGroup(); {
                    if (elements::subtab(skCrypt("Weapon"), subtab == weapons1)) { subtab = weapons1; }
                    //if (elements::subtab("oan", subtab == weapons2)) { subtab = weapons2; }
                }
                ImGui::EndGroup();

                switch (subtab) {
                case weapons1:
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                        ImGui::Checkbox(skCrypt("Infinite Ammo"), &Settings::Weapon::InfiniteAmmo);
                        ImGui::Checkbox(skCrypt("Damage Boost"), &Settings::Weapon::DamageBoost);
                        if (Settings::Weapon::DamageBoost)
                        {
                            ImGui::SliderFloat((skCrypt("Damage")), &Settings::Weapon::DamageBoost_value, 0.5, 1000, "%.1f", ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        }

                        //ImGui::Checkbox(skCrypt("No Reload"), &Settings::Weapon::FastNoReload);
                        // bura baþlar
                        ImGui::Checkbox(skCrypt("No Reload"), &Settings::Weapon::NoReload);
                        if (Settings::Weapon::NoReload)
                        {
                            ImGui::SliderInt(skCrypt("Reload Speed"), &Settings::Weapon::ReloadSpeed, 1, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        }
                        ImGui::Checkbox(skCrypt("Recoil Manipulation"), &Settings::Weapon::No_Recoil);
                        if (Settings::Weapon::No_Recoil)
                        {
                            ImGui::SliderInt(skCrypt("Recoil Speed"), &Settings::Weapon::RecoilManipulation, 0, 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        }
                        ImGui::Checkbox(skCrypt("No Spread"), &Settings::Weapon::No_Spread);
                        ImGui::Checkbox(skCrypt("No Range Limit"), &Settings::Weapon::NoRangeLimit);
                        //
                        ImGui::SliderInt(skCrypt("Ammo"), &Settings::Weapon::AmmoAmount, 1, 9999, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                        
                        if (ImGui::Button(skCrypt("Set Ammo")))
                        {
                            Settings::Weapon::SetAmmo = true;
                        }

                        ImGui::Checkbox(skCrypt("Rocket Hit"), &Settings::Weapon::rapidfire);
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 476, 16 });
                    e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {

                        /*
                        static ImGuiTextFilter weplist;
                        weplist.Draw(skCrypt("  "), 240);

                        if (ImGui::ListBoxHeader(skCrypt("##WeaponList"), ImVec2(210, 330)))
                        {
                            auto selectedwep = Settings::Weapon::weapon_current;

                            //if (weplist.PassFilter(weapon_list))
                            //{
                                if (ImGui::Selectable(weaponsList.c_str(), selectedwep))
                                {
                                    //selectedwep = weaponsList;
                                }
                            //}


                            ImGui::ListBoxFooter();
                        }*/

                        ImGui::ListBox(skCrypt("##WeaponList"), &Settings::Weapon::weapon_current, Settings::Weapon::weapon_list, IM_ARRAYSIZE(Settings::Weapon::weapon_list), 10);
                    
                        if (ImGui::Button(skCrypt("Give (Risky)")))
                        {
                            switch (Settings::Weapon::weapon_current) //dtc
                            {
                            case 0: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x92A27487, (int)999, 1); break;
                            case 1: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x958A4A8F, (int)999, 1); break;
                            case 2: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x99B507EA, (int)999, 1); break;
                            case 3: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDD5DF8D9, (int)999, 1); break;

                            case 4: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x1B06D571, (int)999, 1); break;
                            case 5: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFE256D4, (int)999, 1); break;
                            case 6: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x5EF9FEC4, (int)999, 1); break;
                            case 7: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x22D8FE39, (int)999, 1); break;

                            case 8: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x3656C8C1, (int)999, 1); break;
                            case 9: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x99AEEB3B, (int)999, 1); break;
                            case 10: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFD21232, (int)999, 1); break;
                            case 11: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x88374054, (int)999, 1); break;
                            case 12: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xD205520E, (int)999, 1); break;
                            case 13: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x83839C4, (int)999, 1); break;
                            case 14: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x47757124, (int)999, 1); break;
                            case 15: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDC4DB296, (int)999, 1); break;
                            case 16: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC1B3C3D1, (int)999, 1); break;
                            case 17: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xCB96392F, (int)999, 1); break;
                            case 18: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x97EA20B8, (int)999, 1); break;
                            case 19: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x13532244, (int)999, 1); break;
                            case 20: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x2BE6766B, (int)999, 1); break;
                            case 21: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x78A97CD0, (int)999, 1); break;

                            case 22: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xEFE7E2DF, (int)999, 1); break;
                            case 23: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA3D4D34, (int)999, 1); break;
                            case 24: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDB1AA450, (int)999, 1); break;
                            case 25: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBD248B55, (int)999, 1); break;
                            case 26: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x1D073A89, (int)999, 1); break;
                            case 27: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x555AF99A, (int)999, 1); break;
                            case 28: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7846A318, (int)999, 1); break;
                            case 29: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xE284C527, (int)999, 1); break;
                            case 30: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x9D61E50F, (int)999, 1); break;
                            case 31: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA89CB99E, (int)999, 1); break;
                            case 32: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x3AABBBAA, (int)999, 1); break;
                            case 33: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xEF951FBB, (int)999, 1); break;
                            case 34: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x12E82D3D, (int)999, 1); break;
                            case 35: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFEFFF6D, (int)999, 1); break;
                            case 36: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x394F415C, (int)999, 1); break;
                            case 37: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x83BF0278, (int)999, 1); break;
                            case 38: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFAD1F1C9, (int)999, 1); break;
                            case 39: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xAF113F99, (int)999, 1); break;
                            case 40: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC0A3098D, (int)999, 1); break;
                            case 41: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x969C3D67, (int)999, 1); break;
                            case 42: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7F229F94, (int)999, 1); break;
                            case 43: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x84D6FAFD, (int)999, 1); break;
                            case 44: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x624FE830, (int)999, 1); break;
                            case 45: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x9D07F764, (int)999, 1); break;
                            case 46: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7FD62962, (int)999, 1); break;
                            case 47: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDBBD7280, (int)999, 1); break;
                            case 48: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x61012683, (int)999, 1); break;

                            case 49: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x5FC3C11, (int)999, 1); break;
                            case 50: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC472FE2, (int)999, 1); break;
                            case 51: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA914799, (int)999, 1); break;
                            case 52: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC734385A, (int)999, 1); break;
                            case 53: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x6A6C02E0, (int)999, 1); break;
                            case 54: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xB1CA77B1, (int)999, 1); break;
                            case 55: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA284510B, (int)999, 1); break;
                            case 56: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x42BF8A85, (int)999, 1); break;
                            case 57: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7F7497E5, (int)999, 1); break;
                            case 58: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x6D544C99, (int)999, 1); break;

                            case 59: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x63AB0442, (int)999, 1); break;
                            case 60: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x781FE4A, (int)999, 1); break;
                            case 61: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x93E220BD, (int)999, 1); break;
                            case 62: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA0973D5E, (int)999, 1); break;
                            case 63: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFDBC8A50, (int)999, 1); break;
                            case 64: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x497FACC3, (int)999, 1); break;
                            case 65: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x24B17070, (int)999, 1); break;
                            case 66: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x2C3731D9, (int)999, 1); break;
                            case 67: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xAB564B93, (int)999, 1); break;
                            case 68: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x787F0BB, (int)999, 1); break;

                            case 69: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBA45E8B8, (int)999, 1); break;
                            case 70: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x23C9F95C, (int)999, 1); break;

                            case 71: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x34A67B97, (int)999, 1); break;
                            case 72: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x60EC506, (int)999, 1); break;
                            case 73: WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, (int)999, 1); break;
                            }
                        }
                        ImGui::SameLine();
                        if (ImGui::Button(skCrypt("Remove (Risky)"))) //dtc
                        {
                            switch (Settings::Weapon::weapon_current)
                            {
                            case 0: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x92A27487); break;
                            case 1: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x958A4A8F); break;
                            case 2: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x99B507EA); break;
                            case 3: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xDD5DF8D9); break;
                            case 4: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x1B06D571); break;
                            case 5: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xBFE256D4); break;
                            case 6: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x5EF9FEC4); break;
                            case 7: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x22D8FE39); break;
                            case 8: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x3656C8C1); break;
                            case 9: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x99AEEB3B); break;
                            case 10: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xBFD21232); break;
                            case 11: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x88374054); break;
                            case 12: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xD205520E); break;
                            case 13: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x83839C4); break;
                            case 14: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x47757124); break;
                            case 15: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xDC4DB296); break;
                            case 16: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xC1B3C3D1); break;
                            case 17: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xCB96392F); break;
                            case 18: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x97EA20B8); break;
                            case 19: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x13532244); break;
                            case 20: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x2BE6766B); break;
                            case 21: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x78A97CD0); break;
                            case 22: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xEFE7E2DF); break;
                            case 23: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xA3D4D34); break;
                            case 24: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xDB1AA450); break;
                            case 25: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xBD248B55); break;
                            case 26: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x1D073A89); break;
                            case 27: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x555AF99A); break;
                            case 28: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x7846A318); break;
                            case 29: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xE284C527); break;
                            case 30: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x9D61E50F); break;
                            case 31: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xA89CB99E); break;
                            case 32: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x3AABBBAA); break;
                            case 33: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xEF951FBB); break;
                            case 34: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x12E82D3D); break;
                            case 35: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xBFEFFF6D); break;
                            case 36: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x394F415C); break;
                            case 37: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x83BF0278); break;
                            case 38: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xFAD1F1C9); break;
                            case 39: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xAF113F99); break;
                            case 40: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xC0A3098D); break;
                            case 41: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x969C3D67); break;
                            case 42: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x7F229F94); break;
                            case 43: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x84D6FAFD); break;
                            case 44: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x624FE830); break;
                            case 45: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x9D07F764); break;
                            case 46: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x7FD62962); break;
                            case 47: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xDBBD7280); break;
                            case 48: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x61012683); break;
                            case 49: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x5FC3C11); break;
                            case 50: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xC472FE2); break;
                            case 51: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xA914799); break;
                            case 52: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xC734385A); break;
                            case 53: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x6A6C02E0); break;
                            case 54: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xB1CA77B1); break;
                            case 55: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xA284510B); break;
                            case 56: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x42BF8A85); break;
                            case 57: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x7F7497E5); break;
                            case 58: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x6D544C99); break;
                            case 59: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x63AB0442); break;
                            case 60: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x781FE4A); break;
                            case 61: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x93E220BD); break;
                            case 62: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xA0973D5E); break;
                            case 63: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xFDBC8A50); break;
                            case 64: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x497FACC3); break;
                            case 65: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x24B17070); break;
                            case 66: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x2C3731D9); break;
                            case 67: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xAB564B93); break;
                            case 68: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x787F0BB); break;
                            case 69: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xBA45E8B8); break;
                            case 70: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x23C9F95C); break;
                            case 71: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x34A67B97); break;
                            case 72: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0x60EC506); break;
                            case 73: WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776); break;
                            }
                        }
                    }
                    e_elements::end_child();
                    break;

                case weapons2:
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                        ImGui::Text(skCrypt("Coming Soon!"));
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 476, 16 });
                    e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                        ImGui::Text(skCrypt("Coming Soon!"));
                    }
                    e_elements::end_child();
                    break;
                }

                break;

             /// Configs

            case configs:
                draw->AddText(subtab_title, 15.0f, ImVec2(pos.x + 72, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.4f), skCrypt("Deep")); //cheatname

                ImGui::SetCursorPos({ 57, 86 });
                ImGui::BeginGroup(); {
                    if (elements::subtab(skCrypt("Config"), subtab == configs1)) { subtab = configs1; }
                    if (elements::subtab(skCrypt("Profile"), subtab == configs2)) { subtab = configs2; }
                }
                ImGui::EndGroup();

                switch (subtab) {
                case configs1:
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                        FILE* p_stream;

                        static int style_idx = 0;
                        const char* configs[] = { skCrypt("BAKIM / MAINTANCE") };

                        ImGui::ListBox(skCrypt("##cfg"), &style_idx, configs, IM_ARRAYSIZE(configs), 5);

                        ImGui::InputText(skCrypt("##cfg"), Settings::misc::cfg_name, IM_ARRAYSIZE(Settings::misc::cfg_name));
                        ImGui::Spacing();
                        if (ImGui::Button(skCrypt("Load CFG")))
                        {
                            // declare file stream
                            FILE* p_stream4;

                            // open file for (extended) reading
                            fopen_s(&p_stream4, Settings::misc::cfg_name, "r+");

                            // set file pointer to 0
                            fseek(p_stream4, 0, SEEK_SET);

                            // read a value... 
                            fread_s(&Settings::Player::Godmod, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::Semi_Godmod, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::NoClip, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::BackwardHotkey, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::ForwardHotkey, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::HotkeyArmor, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::HotkeyHealth, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::EnableFovChange, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::FovValue, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::noclipspeed, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::NoClipSpeed_bool, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::AntiHS, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::Invisible, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::Gravity, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Player::SuperJump, sizeof(bool), sizeof(bool), 1, p_stream4);

                            //Misc
                            fread_s(&Settings::misc::Hitsound, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::misc::NoclipKey, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::misc::PanicKey, sizeof(bool), sizeof(bool), 1, p_stream4);

                            //Weapon

                            fread_s(&Settings::Weapon::DamageBoost, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Weapon::DamageBoost_value, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Weapon::ReloadSpeed, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Weapon::NoRangeLimit, sizeof(bool), sizeof(bool), 1, p_stream4);
                            //fread_s(&Settings::Weapon::FastNoReload, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Weapon::InfiniteAmmo, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Weapon::RecoilManipulation, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Weapon::No_Recoil, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Weapon::No_Spread, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Weapon::InstantHitAmmo, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Weapon::AmmoAmount, sizeof(bool), sizeof(bool), 1, p_stream4);

                            //Aimbot
                            fread_s(&Settings::Aimbot::Aimbot, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::TriggerBot, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::VisibleCheck, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::Draw_Fov, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::AimbotFov, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::DistanceAimbotLimit, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::AimbotSmooth, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::TypeOfTarget, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::AimbotType, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::AimbotTypes, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::MagicDamage, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::AimbotPrediction, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Aimbot::crosshair, sizeof(bool), sizeof(bool), 1, p_stream4);

                            //Visuals

                            fread_s(&Settings::Visuals::extendedesp, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::Box, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::Skeleton, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::Corner, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::HealthBar, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::HealthBar_idx, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::ArmorBar, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::Snapline, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::BoneThick, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::NameEsp, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::Weapon, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::Distance, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::DistanceESP, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::IgnorePlayers, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::Draw_Ped, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::Visuals::Draw_LocalPlayer, sizeof(bool), sizeof(bool), 1, p_stream4);

                            fread_s(&Settings::Visuals::Watermark, sizeof(bool), sizeof(bool), 1, p_stream4);

                            fread_s(&Settings::Visuals::SkelColor, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                            fread_s(&Settings::Visuals::SkelColorVisible, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                            fread_s(&Settings::Visuals::BoxColor, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                            fread_s(&Settings::Visuals::BoxColorVisible, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                            fread_s(&Settings::Visuals::CornerColor, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                            fread_s(&Settings::Visuals::CrosshairColor, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                            fread_s(&Settings::Visuals::FovColor, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                            fread_s(&Settings::Visuals::FovColored, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                            fread_s(&Settings::Visuals::SkelColorVisible, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);


                            //Vehicle
                            fread_s(&Settings::vehicle::godmode, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::vehicle::warp_vehicle, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::vehicle::unlockcar, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::vehicle::fastspeed, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::vehicle::vehicle_speed_bool, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::vehicle::vehicle_speed_value, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::vehicle::secondary_color, sizeof(bool), sizeof(bool), 1, p_stream4);
                            fread_s(&Settings::vehicle::primary_color, sizeof(bool), sizeof(bool), 1, p_stream4);


                            fclose(p_stream4);
                        }

                        ImGui::SameLine();

                        if (ImGui::Button(skCrypt("Save CFG")))
                        {
                            // declare file stream
                            FILE* p_stream4;

                            // open file for (extended) writing
                            fopen_s(&p_stream4, Settings::misc::cfg_name, "w+");

                            // set file pointer to 0
                            fseek(p_stream4, 0, SEEK_SET);

                            //PLAYER ::
                            fwrite(&Settings::Player::Godmod, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::Semi_Godmod, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::NoClip, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::BackwardHotkey, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::ForwardHotkey, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::HotkeyArmor, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::HotkeyHealth, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::EnableFovChange, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::FovValue, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::noclipspeed, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::NoClipSpeed_bool, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::AntiHS, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::Invisible, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::Gravity, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Player::SuperJump, sizeof(bool), 1, p_stream4);


                            //Misc
                            fwrite(&Settings::misc::Hitsound, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::misc::NoclipKey, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::misc::PanicKey, sizeof(bool), 1, p_stream4);

                            //Weapon

                            fwrite(&Settings::Weapon::DamageBoost, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Weapon::DamageBoost_value, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Weapon::ReloadSpeed, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Weapon::NoRangeLimit, sizeof(bool), 1, p_stream4);
                            //fwrite(&Settings::Weapon::FastNoReload, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Weapon::InfiniteAmmo, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Weapon::RecoilManipulation, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Weapon::No_Recoil, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Weapon::No_Spread, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Weapon::InstantHitAmmo, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Weapon::AmmoAmount, sizeof(bool), 1, p_stream4);

                            //Aimbot
                            fwrite(&Settings::Aimbot::Aimbot, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::TriggerBot, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::VisibleCheck, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::Draw_Fov, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::AimbotFov, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::DistanceAimbotLimit, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::AimbotSmooth, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::TypeOfTarget, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::AimbotType, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::AimbotTypes, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::MagicDamage, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::AimbotPrediction, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Aimbot::crosshair, sizeof(bool), 1, p_stream4);

                            //Visuals

                            fwrite(&Settings::Visuals::extendedesp, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::Box, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::Skeleton, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::Corner, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::HealthBar, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::HealthBar_idx, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::ArmorBar, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::Snapline, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::BoneThick, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::NameEsp, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::Weapon, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::Distance, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::DistanceESP, sizeof(bool), 1, p_stream4);
                            
                            fwrite(&Settings::Visuals::IgnorePlayers, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::Draw_Ped, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::Visuals::Draw_LocalPlayer, sizeof(bool), 1, p_stream4);

                            fwrite(&Settings::Visuals::Watermark, sizeof(bool), 1, p_stream4);

                            fwrite(&Settings::Visuals::SkelColor, sizeof(ImColor), 1, p_stream4);
                            fwrite(&Settings::Visuals::SkelColorVisible, sizeof(ImColor), 1, p_stream4);
                            fwrite(&Settings::Visuals::BoxColor, sizeof(ImColor), 1, p_stream4);
                            fwrite(&Settings::Visuals::BoxColorVisible, sizeof(ImColor), 1, p_stream4);
                            fwrite(&Settings::Visuals::CornerColor, sizeof(ImColor), 1, p_stream4);
                            fwrite(&Settings::Visuals::CrosshairColor, sizeof(ImColor), 1, p_stream4);
                            fwrite(&Settings::Visuals::FovColor, sizeof(ImColor), 1, p_stream4);
                            fwrite(&Settings::Visuals::FovColored, sizeof(ImColor), 1, p_stream4);
                            fwrite(&Settings::Visuals::SkelColorVisible, sizeof(ImColor), 1, p_stream4);


                            //Vehicle
                            fwrite(&Settings::vehicle::godmode, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::vehicle::warp_vehicle, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::vehicle::unlockcar, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::vehicle::fastspeed, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::vehicle::vehicle_speed_bool, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::vehicle::vehicle_speed_value, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::vehicle::secondary_color, sizeof(bool), 1, p_stream4);
                            fwrite(&Settings::vehicle::primary_color, sizeof(bool), 1, p_stream4);


                            fclose(p_stream4);
                        }
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 476, 16 });
                    e_elements::begin_child(skCrypt("About"), ImVec2(240, 430)); {
                        ImGui::Text(skCrypt("Version : "));
                        ImGui::SameLine();
                        ImGui::Text(Core::project_version);

                        ImGui::Text(skCrypt("Update Date : "));
                        ImGui::SameLine();
                        ImGui::Text(Core::updated_date);
                        
                        ImGui::Text(skCrypt("IP: "));
                        ImGui::SameLine();
                        auto serverip = CFX::GET_CURRENT_SERVER_ENDPOINT();
                        ImGui::Text(serverip);

                        //const char* gameversion = (const char*)CFX::GET_GAME_BUILD_NUMBER();
                       // ImGui::Text(gameversion);

                        //ImGui::Text(RAIDOS::GameVersion);
                    }
                    e_elements::end_child();
                    break;

                case configs2:
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                        ImGui::Text(skCrypt("Coming Soon!"));
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 476, 16 });
                    e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                        ImGui::Text(skCrypt("Coming Soon!"));
                    }
                    e_elements::end_child();
                    break;
                }

                break;

            /// Scripts

            case scripts:
                draw->AddText(subtab_title, 15.0f, ImVec2(pos.x + 72, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.4f), skCrypt("Deep"));

                ImGui::SetCursorPos({ 57, 86 });
                ImGui::BeginGroup(); {
                    if (elements::subtab(skCrypt("Stopper"), subtab == scripts1)) { subtab = scripts1; }
                    if (elements::subtab(skCrypt("Executor"), subtab == scripts2)) { subtab = scripts2; }
                }
                ImGui::EndGroup();

                switch (subtab) {
                case scripts1:

                    if (numResources > 2)
                    {
                        ImGui::SetCursorPos({ 226, 16 });

                        e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430));
                        {
                            static ImGuiTextFilter resourcelist;
                            resourcelist.Draw(skCrypt("R"), 240);

                            if (ImGui::ListBoxHeader(skCrypt("##ResourceList"), ImVec2(210, 330)))
                            {
                                for (int i = 0; i < numResources; i++) {

                                    auto resource = CFX::GET_RESOURCE_BY_FIND_INDEX(i);
                                    if (resource == NULL) 
                                    {}
                                    else
                                    {
                                        bool is_selected = (selected_resource == i);

                                        if (resourcelist.PassFilter(resource))
                                        {
                                            if (ImGui::Selectable(resource, is_selected))
                                            {
                                                selected_resource = i;
                                            }
                                        }
                                    }

                                }

                                ImGui::ListBoxFooter();
                            }
                        }
                        e_elements::end_child();

                        ImGui::SetCursorPos({ 476, 16 });
                        e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430));
                        {

                            const char* selecting_resource = CFX::GET_RESOURCE_BY_FIND_INDEX(selected_resource);
                            const char* isWorkingResource = CFX::GET_RESOURCE_STATE(selecting_resource);

                            ImGui::Text(skCrypt("Selected : "));
                            ImGui::SameLine();
                            ImGui::Text(selecting_resource);

                            ImGui::Text(skCrypt("Status : "));
                            ImGui::SameLine();
                            ImGui::Text(isWorkingResource);
                            ImGui::Text(skCrypt("Features is Testing, Please Wait Update."));
                            if (ImGui::Button(skCrypt("Start // Updating")))
                            {

                            }
                            ImGui::SameLine();
                            if (ImGui::Button(skCrypt("Stop // Updating")))
                            {
                            
                            }
                        }
                        e_elements::end_child();
                    }
                    else
                    {
                        ImGui::SetCursorPos({ 226, 16 });
                        e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                            ImGui::Text(skCrypt("Only works in-game."));
                        }
                        e_elements::end_child();

                        ImGui::SetCursorPos({ 476, 16 });
                        e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                            ImGui::Text(skCrypt("Only works in-game."));
                        }
                        e_elements::end_child();
                    }

                    break;


                case scripts2:
                    ImGui::SetCursorPos({ 226, 16 });
                    e_elements::begin_child(skCrypt("Details"), ImVec2(240, 430)); {
                        if (ImGui::ListBoxHeader(skCrypt("##dedesinki"), ImVec2(210, 330)))
                        {
                            /*
                            hk_World* World = (hk_World*)*(uint64_t*)(RAIDOS::World);
                            if (!World)	return;

                            hk_Ped* LocalPlayer = World->LocalPlayer();
                            if (!LocalPlayer) return;

                            hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(RAIDOS::ReplayInterface);
                            if (!ReplayInterface) return;

                            hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
                            if (!PedInterface) return;


                            ImGui::Text("PedMaximum: %d \n", PedInterface->PedMaximum());

                            for (uint64_t i = 0; i < PedInterface->PedMaximum(); i++)
                            {
                                ImGui::Text("[%d]Ped Coordinate: x:%f y:%f z:%f \n", i,
                                    PedInterface->PedList()->Ped(i)->GetCoordinate().x,
                                    PedInterface->PedList()->Ped(i)->GetCoordinate().y,
                                    PedInterface->PedList()->Ped(i)->GetCoordinate().z
                                );
                            }*/

                            ImGui::ListBoxFooter();
                        }
                    }
                    e_elements::end_child();

                    ImGui::SetCursorPos({ 476, 16 });
                    e_elements::begin_child(skCrypt("Settings"), ImVec2(240, 430)); {
                        ImGui::Text(skCrypt("It is being updated due to risk.")); // Only works in-game.

                    }
                    e_elements::end_child();
                    break;
                }

            break;
        }
    }
    ImGui::End();

    ImGui::PopStyleVar();

    // Rendering
    ImGui::EndFrame();


}

//bool Core::Init()

DWORD WINAPI Core::Init(LPVOID lpvParam)
{
	std::this_thread::sleep_for(std::chrono::seconds(10)); // paste from hx 

    if (!DirectX::OverlayHooked) //while
    {

        if (Hook::Init())
        {
			DirectX::OverlayHooked = Hook::Present((void**)&DirectX::OriginalPresent, PresentHook);
        }
    }

	return true;
}
