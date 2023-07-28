#include "Menu.hpp"

#include "includes.hpp"
#include "Settings.hpp"
#include <Fonts.hpp>
#include "imgui_animated.h"
#include <imguinotify.hpp>
#include <tchar.h>
#include <vector>
#include "skCrpyt.h"
#include <string>
#include "Weapon.hpp"
#include <windows.h>
#pragma comment(lib, "ntdll.lib")
#include "Executor.hpp"

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
    PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);

int currentresource = 0;
template<class T, class U>
static T clamp(const T& in, const U& low, const U& high)
{
    if (in <= low)
        return low;

    if (in >= high)
        return high;

    return in;
}

//const char* weaponType[]{ "Throwable", "Misc", "Heavy", "Sniper", "Pistol", "Melee", "Light", "SMG", "Rifle", "Shotgun", "Unknown" };

ImFont* font;

static int tab = 0;
static int tab2 = 0;

#include <math.h>
//#include <imgui_demo.cpp>

#define MAX_RGB 255.0
#define HELPMARKER(str) ImGui::SameLine(); ImGui::TextColored(ImColor(220, 190, 0, 255), "(?)"); if (ImGui::IsItemHovered()) ImGui::SetTooltip(str)

ImVec4 ToVec4(float r, float g, float b, float a)
{
    return ImVec4(r / MAX_RGB, g / MAX_RGB, b / MAX_RGB, a / MAX_RGB);
}

static ImVec4 active = ToVec4(51, 124, 176, 175);
static ImVec4 inactive = ToVec4(125, 125, 125, 85);

static const ImWchar ranges[] =
{
    0x0020, 0x00FF, // Basic Latin + Latin Supplement
    0x0400, 0x044F, // Cyrillic
    0,
};

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

void CenterTextEx(const char* text, float width, int lineId, bool separator)
{
    if (text == nullptr)
        return;

    ImGui::Spacing();
    ImGui::SameLine((width / 2) - (ImGui::CalcTextSize(text).x / 2));
    ImGui::Text(text);
    ImGui::Spacing();
}
void CenterText(const char* text, int lineId, bool separator)
{
    if (text == nullptr)
        return;

    ImGui::Spacing();
    ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(text).x / 2));
    ImGui::Text(text);
    ImGui::Spacing();
}
void Menu::Style(void)
{
    ImGuiStyle* Style = &ImGui::GetStyle();
    ImVec4* Colors = Style->Colors;
    Style->WindowPadding = { 10.f, 10.f };
    Style->PopupRounding = 0.f;
    Style->FramePadding = { 8.f, 4.f };
    Style->ItemSpacing = { 10.f, 8.f };
    Style->ItemInnerSpacing = { 6.f, 6.f };
    Style->TouchExtraPadding = { 0.f, 0.f };
    Style->IndentSpacing = 21.f;
    Style->ScrollbarSize = 15.f;
    Style->GrabMinSize = 8.f;
    Style->WindowBorderSize = 1.f;
    Style->ChildBorderSize = 0.f;
    Style->PopupBorderSize = 1.f;
    Style->FrameBorderSize = 0.f;
    Style->TabBorderSize = 0.f;
    Style->WindowRounding = 6.f;
    Style->ChildRounding = 0.f;
    Style->FrameRounding = 0.f;
    Style->ScrollbarRounding = 0.f;
    Style->GrabRounding = 0.f;
    Style->TabRounding = 0.f;
    Style->WindowTitleAlign = { 0.5f, 0.5f };
    Style->ButtonTextAlign = { 0.5f, 0.5f };
    Style->DisplaySafeAreaPadding = { 3.f, 3.f };

    Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    Colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 0.90f, 0.19f, 1.00f);
    Colors[ImGuiCol_WindowBg] = ImVec4((ImColor(0, 18, 32, 240)));
    Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_PopupBg] = ImVec4((ImColor(0, 18, 32, 240)));
    Colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
    Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
    Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.78f);
    Colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.27f, 0.27f, 0.54f);
    Colors[ImGuiCol_TitleBg] = ImVec4(ImColor(14, 26, 42));
    Colors[ImGuiCol_TitleBgActive] = ImVec4(ImColor(14, 26, 42));
    Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(ImColor(14, 26, 42, 130));
    Colors[ImGuiCol_MenuBarBg] = ImVec4((ImColor(0, 18, 32, 240)));
    Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    Colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
    Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.39f, 0.38f, 0.38f, 1.00f);
    Colors[ImGuiCol_Button] = ImVec4(0.41f, 0.41f, 0.41f, 0.74f);
    Colors[ImGuiCol_ButtonHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.78f);
    Colors[ImGuiCol_ButtonActive] = ImVec4(0.41f, 0.41f, 0.41f, 0.87f);
    Colors[ImGuiCol_Header] = ImVec4(0.37f, 0.37f, 0.37f, 0.31f);
    Colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.38f, 0.38f, 0.37f);
    Colors[ImGuiCol_HeaderActive] = ImVec4(0.37f, 0.37f, 0.37f, 0.51f);
    Colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.38f, 0.38f, 0.50f);
    Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.50f);
    Colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.64f);
    Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    Colors[ImGuiCol_Tab] = ImVec4(ImColor(11, 26, 39));
    Colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.27f, 0.27f, 0.86f);
    Colors[ImGuiCol_TabActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.86f);
    Colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 0.97f);
    Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Menu::Drawing(void)
{
    //if (Settings::misc::security_1)
    //{
        if (Settings::misc::security_2)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(650, 500));

            ImGui::PopStyleVar();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); // Background color
            ImGui::RenderNotifications(); // <-- Here we render all notifications
            ImGui::PopStyleVar(1); // Don't forget to Pop()
            ImGui::PopStyleColor(1);

            ImGui::Begin("RAIDAIM", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
            {
                ImGui::Text(Menu::title_text);
                if (ImGui::BeginTabBar("Cheat"))
                {
                    if (ImGui::BeginTabItem("Aimbot"))
                    {
                        ImGui::PushFont(font);
                        {
                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(15);
                            ImGui::BeginChild(skCrypt("General"), ImVec2(300, 205), false);
                            {

                                ImGui::Text("General");

                                ImGui::Separator();

                                ImGui::Checkbox(skCrypt("Enable Aimbot"), &Settings::Aimbot::Aimbot);

                                ImGui::SameLine();

                                ImGui::Text(" ");

                                ImGui::SameLine();

                                ImGui::Raidkey(" ", &Settings::Aimbot::Hotkey, ImVec2(125, 25));


                                ImGui::Separator();

                                // if (Settings::Aimbot::Aimbot == false)
                                    //ImGui::BeginDisabled();
                                //ImGui::Text(skCrypt("Aimbot:"));

                                ImGui::Checkbox((skCrypt("Draw Fov")), &Settings::Aimbot::Draw_Fov);
                                ImGui::Checkbox("Draw Crosshair", &Settings::Aimbot::crosshair);
                                ImGui::Checkbox((skCrypt("Visible Only")), &Settings::Aimbot::VisibleCheck);


                                // if (Settings::Aimbot::Aimbot == false)
                                    //ImGui::EndDisabled();
                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();
                        }
                        /*
                        {
                            ImGui::SetCursorPosY(255);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(15);
                            ImGui::BeginChild("Other", ImVec2(300, 180), false);
                            {

                                ImGui::Text("Other");

                                ImGui::Separator();


                                ImGui::Checkbox((skCrypt("Draw Fov Circle")), &Settings::Aimbot::Draw_Fov);

                                ImGui::Checkbox((skCrypt("Visible Check")), &Settings::Aimbot::VisibleCheck);

                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();
                        }
                        */
                        {
                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(340);
                            ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                            {

                                ImGui::Text("Settings");

                                ImGui::Separator();

                                if (Settings::Aimbot::Aimbot == false)
                                    ImGui::BeginDisabled();
                         
                                    ImGui::Combo(skCrypt(("Aim Type")), &Settings::Aimbot::AimbotTypes, Settings::Aimbot::AimbotType, IM_ARRAYSIZE(Settings::Aimbot::AimbotType), ImGuiComboFlags_NoArrowButton);
                                    ImGui::Combo(skCrypt(("Target Type")), &Settings::Aimbot::TypeOfTarget, Settings::Aimbot::AimbotTarget, IM_ARRAYSIZE(Settings::Aimbot::AimbotTarget), ImGuiComboFlags_NoArrowButton);
                                    ImGui::Combo(skCrypt(("Target Bone")), &Settings::Aimbot::AimbotBone, Settings::Aimbot::AimbotBones, IM_ARRAYSIZE(Settings::Aimbot::AimbotBones), ImGuiComboFlags_NoArrowButton);
                                    
                                    ImGui::Text(" ");
                                    ImGui::Text("Details");
                                    ImGui::Separator();

                                    ImGui::SliderInt((skCrypt("FOV")), &Settings::Aimbot::AimbotFov, 1, 1200 / 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                    ImGui::SliderInt((skCrypt("Smooth")), &Settings::Aimbot::AimbotSmooth, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput); ;
                                    ImGui::SliderInt((skCrypt("Distance")), &Settings::Aimbot::DistanceAimbotLimit, 1, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);


                                //if (Settings::Aimbot::AimbotTypes == 0)
                                    //{
                                    //ImGui::Checkbox(skCrypt("Prediction // TriggerBot"), &Settings::Aimbot::TriggerBot);
                                //}


                                if (!Settings::Aimbot::Aimbot)
                                    ImGui::EndDisabled();
                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();
                            ImGui::PopFont();
                        }
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Visuals"))
                    {
                        ImGui::PushFont(font);
                        {
                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(15);
                            ImGui::BeginChild(skCrypt("General"), ImVec2(300, 385), false);
                            {

                                ImGui::Text("General");

                                ImGui::Separator();

                                ImGui::Checkbox("Enable ESP", &Settings::Visuals::visualsenable);

                                if (!Settings::Visuals::visualsenable)
                                    ImGui::BeginDisabled();

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
                                ImGui::Checkbox((skCrypt("Corner Box ESP")), &Settings::Visuals::Corner);
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
                                ImGui::Checkbox((skCrypt("Skeleton ESP")), &Settings::Visuals::Skeleton);
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

                                    if (ImGui::Combo("Type", &Settings::Visuals::Skeleton_idx, "Simple\0Detailled\0"))
                                    {
                                        switch (Settings::Visuals::Skeleton_idx)
                                        {
                                        case 0: Settings::Visuals::Skeleton_Detailled = false; break;
                                        case 1: Settings::Visuals::Skeleton_Detailled = true; break;
                                        }
                                    }
                                }
                                ImGui::Checkbox((skCrypt("HealthBar")), &Settings::Visuals::HealthBar);
                                if (Settings::Visuals::HealthBar) {

                                    if (ImGui::Combo("Location", &Settings::Visuals::HealthBar_idx, "Right\0Left\0Down\0Up"))
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
                                ImGui::Checkbox((skCrypt("ArmorBar")), &Settings::Visuals::ArmorBar);
                                ImGui::Checkbox((skCrypt("Snapline")), &Settings::Visuals::Snapline);
                                ImGui::Checkbox((skCrypt("Flags")), &Settings::Visuals::flags);
                                ImGui::Checkbox((skCrypt("Static ESP (Beta)")), &Settings::Visuals::StaticESP);
                                if (!Settings::Visuals::visualsenable)
                                    ImGui::EndDisabled();

                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();
                        }
                        {
                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(340);
                            ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                            {
                                ImGui::Text("Settings");

                                ImGui::Separator();


                                if (!Settings::Visuals::visualsenable)
                                    ImGui::BeginDisabled();

                                ImGui::SliderInt((skCrypt("Distance")), &Settings::Visuals::Distance, 1, 1000, NULL, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                ImGui::SliderInt((skCrypt("FillBox Alpha")), &Settings::Visuals::FillBoxAlpha, 1, 255, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                ImGui::SliderFloat((skCrypt("Box Size")), &Settings::Visuals::BoxSize, 0.5, 2, "%.1f", ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                ImGui::SliderFloat((skCrypt("Corner Size")), &Settings::Visuals::CornerSize, 0.5, 2, "%.1f", ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                ImGui::Checkbox((skCrypt(" ")), &Settings::Visuals::PlayerName);
                                ImGui::SameLine();
                                ImGui::Text("Player Name");
                                ImGui::SameLine();
                                ImGui::Checkbox((skCrypt(" ")), &Settings::Visuals::Weapon);
                                ImGui::SameLine();
                                ImGui::Text("Weapon Name");
                                

                                ImGui::Checkbox((skCrypt(" ")), &Settings::Visuals::DistanceESP);
                                ImGui::SameLine();
                                ImGui::Text("Distance");
                                ImGui::SameLine();
                                ImGui::Checkbox((skCrypt(" ")), &Settings::Visuals::Draw_LocalPlayer);
                                ImGui::SameLine();
                                ImGui::Text("Localplayer");

                                //ImGui::Checkbox((skCrypt(" ")), &Settings::Visuals::Draw_Dead);
                                //ImGui::SameLine();
                               // ImGui::Text("Distance");
                                //ImGui::SameLine();

                                ImGui::Checkbox((skCrypt(" ")), &Settings::Visuals::Draw_Ped);
                                ImGui::SameLine();
                                ImGui::Text("Draw Peds");
                                ImGui::SameLine();
                                ImGui::Checkbox((skCrypt(" ")), &Settings::Visuals::Draw_Animals);
                                ImGui::SameLine();
                                ImGui::Text("Only Animals");

                                if (!Settings::Visuals::visualsenable)
                                    ImGui::EndDisabled();
                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();
                            ImGui::PopFont();
                        }

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Self"))
                    {
                        ImGui::PushFont(font);
                        {
                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(15);
                            ImGui::BeginChild(skCrypt("General"), ImVec2(300, 205), false);
                            {
                                ImGui::Text("General");
                                ImGui::Separator();

                                ImGui::Checkbox(skCrypt("Godmod"), &Settings::Player::Godmod);

                                ImGui::Checkbox(skCrypt("Semi-Godmod"), &Settings::Player::Semi_Godmod);

                                ImGui::Checkbox(skCrypt("Invisible"), &Settings::Player::Invisible);

                                ImGui::Checkbox(skCrypt("No Ragdoll"), &Settings::Player::NoRagDoll);

                                ImGui::Checkbox(skCrypt("Anti Headshot"), &Settings::Player::AntiHS);
                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();
                        }
                        {
                            ImGui::SetCursorPosY(275); // 300
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(15);
                            ImGui::BeginChild("Other", ImVec2(300, 180), false);
                            {
                                ImGui::Text("Other");

                                ImGui::Separator();
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
                                if (ImGui::Button(skCrypt("TP Waypoint")))
                                {
                                    Settings::Player::TpToWaypoint = true;
                                }
                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();
                        }
                        {
                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(340);
                            ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                            {

                                ImGui::Text("Settings");

                                ImGui::Separator();

                                ImGui::Checkbox(skCrypt("Enable NoClip (Risky)"), &Settings::Player::NoClip);
                                
                                if (Settings::Player::NoClip)
                                {
                                    ImGui::Raidkey(skCrypt("Noclip Forward  "), &Settings::Player::ForwardHotkey);
                                    ImGui::Raidkey(skCrypt("Noclip Backward"), &Settings::Player::BackwardHotkey);
                                    ImGui::Checkbox(skCrypt("Noclip Speed"), &Settings::Player::NoClipSpeed_bool);
                                    if (Settings::Player::NoClipSpeed_bool)
                                    {
                                        ImGui::SliderInt((skCrypt("Speed")), &Settings::Player::Speed, 0, 40, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                    }
                                }

                                ImGui::SliderFloat((skCrypt(" ")), &Settings::Player::FovValue, 50, 120, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                ImGui::SameLine();
                                if (ImGui::Button(skCrypt("Set FOV")))
                                {
                                    Settings::Player::EnableFovChange = true;
                                }                 

                                //ImGui::Checkbox(skCrypt("No Gravity"), &Settings::Player::Gravity);
                                
                                //ImGui::Checkbox(skCrypt("No Stamina"), &Settings::Player::Infinite_stamina);

                                ImGui::Checkbox(skCrypt("Fast Run"), &Settings::Player::fastrun);

                                if (Settings::Player::fastrun)
                                {
                                    ImGui::SliderInt((skCrypt("RunSpeed")), &Settings::Player::RunSpeedToSet, 0.5, 5, "%.1f", ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                }

                                //   ImGui::SetCursorPosX(5);
                               // ImGui::Checkbox(skCrypt("Super Jump"), &Settings::Player::SuperJump);

                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();
                            ImGui::PopFont();
                        }
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Weapon"))
                    {
                        ImGui::PushFont(font);
                        {
                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(15);
                            ImGui::BeginChild(skCrypt("General"), ImVec2(300, 400), false);
                            {
                                ImGui::Text("General");
                                ImGui::Separator();
                                ImGui::Checkbox(skCrypt("Infinite Ammo"), &Settings::Weapon::InfiniteAmmo);
                                ImGui::Checkbox(skCrypt("Damage Boost"), &Settings::Weapon::DamageBoost);
                                if (Settings::Weapon::DamageBoost)
                                {
                                    ImGui::SliderFloat((skCrypt("Damage")), &Settings::Weapon::DamageBoost_value, 0.5, 100, "%.1f", ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                }
                                
                                ImGui::Checkbox(skCrypt("No Reload"), &Settings::Weapon::FastNoReload);
                                // bura baþlar
                                ImGui::Checkbox(skCrypt("Reload Speed"), &Settings::Weapon::NoReload);
                                if (Settings::Weapon::NoReload)
                                {
                                    ImGui::SliderInt(skCrypt("Reload"), &Settings::Weapon::ReloadSpeed, 0, 1000 / 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                }
                                ImGui::Checkbox(skCrypt("Recoil Manipulation"), &Settings::Weapon::No_Recoil);
                                if (Settings::Weapon::No_Recoil)
                                {
                                    ImGui::SliderInt(skCrypt("Recoil"), &Settings::Weapon::RecoilManipulation, 0, 5, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                }
                                ImGui::Checkbox(skCrypt("Enable No Spread"), &Settings::Weapon::No_Spread);
                                ImGui::Checkbox(skCrypt("Enable No Range limit"), &Settings::Weapon::NoRangeLimit);                              
                                //
                                ImGui::SliderInt(skCrypt("  "), &Settings::Weapon::AmmoAmount, 1, 9999, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                ImGui::SameLine();
                                if (ImGui::Button(skCrypt("Set Ammo")))
                                {
                                    Settings::Weapon::SetAmmo = true;
                                }

                                ImGui::Checkbox(skCrypt("Instant Rocket Hit"), &Settings::Weapon::rapidfire);
                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();

                            ImGui::SetCursorPosY(65);
                            ImGui::BeginGroup();
                            ImGui::SetCursorPosX(340);
                            ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                            {
                                ImGui::Text("Settings");
                                ImGui::Separator();
                                ImGui::ListBox(skCrypt("##WeaponList"), &Settings::Weapon::weapon_current, Settings::Weapon::weapon_list, IM_ARRAYSIZE(Settings::Weapon::weapon_list), 10);

                                if (ImGui::Button(skCrypt("Give")))
                                {
                                    switch (Settings::Weapon::weapon_current)
                                    {
                                        case 0: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x92A27487, (int)999, (bool)false, (bool)true); break;
                                        case 1: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x958A4A8F, (int)999, (bool)false, (bool)true); break;
                                        case 2: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x99B507EA, (int)999, (bool)false, (bool)true); break;
                                        case 3: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDD5DF8D9, (int)999, (bool)false, (bool)true); break;

                                        case 4: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x1B06D571, (int)999, (bool)false, (bool)true); break;
                                        case 5: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFE256D4, (int)999, (bool)false, (bool)true); break;
                                        case 6: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x5EF9FEC4, (int)999, (bool)false, (bool)true); break;
                                        case 7: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x22D8FE39, (int)999, (bool)false, (bool)true); break;

                                        case 8: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x3656C8C1, (int)999, (bool)false, (bool)true); break;
                                        case 9: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x99AEEB3B, (int)999, (bool)false, (bool)true); break;
                                        case 10: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFD21232, (int)999, (bool)false, (bool)true); break;
                                        case 11: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x88374054, (int)999, (bool)false, (bool)true); break;
                                        case 12: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xD205520E, (int)999, (bool)false, (bool)true); break;
                                        case 13: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x83839C4, (int)999, (bool)false, (bool)true); break;
                                        case 14: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x47757124, (int)999, (bool)false, (bool)true); break;
                                        case 15: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDC4DB296, (int)999, (bool)false, (bool)true); break;
                                        case 16: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC1B3C3D1, (int)999, (bool)false, (bool)true); break;
                                        case 17: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xCB96392F, (int)999, (bool)false, (bool)true); break;
                                        case 18: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x97EA20B8, (int)999, (bool)false, (bool)true); break;
                                        case 19: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x13532244, (int)999, (bool)false, (bool)true); break;
                                        case 20: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x2BE6766B, (int)999, (bool)false, (bool)true); break;
                                        case 21: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x78A97CD0, (int)999, (bool)false, (bool)true); break;

                                        case 22: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xEFE7E2DF, (int)999, (bool)false, (bool)true); break;
                                        case 23: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA3D4D34, (int)999, (bool)false, (bool)true); break;
                                        case 24: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDB1AA450, (int)999, (bool)false, (bool)true); break;
                                        case 25: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBD248B55, (int)999, (bool)false, (bool)true); break;
                                        case 26: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x1D073A89, (int)999, (bool)false, (bool)true); break;
                                        case 27: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x555AF99A, (int)999, (bool)false, (bool)true); break;
                                        case 28: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7846A318, (int)999, (bool)false, (bool)true); break;
                                        case 29: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xE284C527, (int)999, (bool)false, (bool)true); break;
                                        case 30: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x9D61E50F, (int)999, (bool)false, (bool)true); break;
                                        case 31: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA89CB99E, (int)999, (bool)false, (bool)true); break;
                                        case 32: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x3AABBBAA, (int)999, (bool)false, (bool)true); break;
                                        case 33: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xEF951FBB, (int)999, (bool)false, (bool)true); break;
                                        case 34: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x12E82D3D, (int)999, (bool)false, (bool)true); break;
                                        case 35: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFEFFF6D, (int)999, (bool)false, (bool)true); break;
                                        case 36: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x394F415C, (int)999, (bool)false, (bool)true); break;
                                        case 37: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x83BF0278, (int)999, (bool)false, (bool)true); break;
                                        case 38: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFAD1F1C9, (int)999, (bool)false, (bool)true); break;
                                        case 39: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xAF113F99, (int)999, (bool)false, (bool)true); break;
                                        case 40: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC0A3098D, (int)999, (bool)false, (bool)true); break;
                                        case 41: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x969C3D67, (int)999, (bool)false, (bool)true); break;
                                        case 42: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7F229F94, (int)999, (bool)false, (bool)true); break;
                                        case 43: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x84D6FAFD, (int)999, (bool)false, (bool)true); break;
                                        case 44: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x624FE830, (int)999, (bool)false, (bool)true); break;
                                        case 45: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x9D07F764, (int)999, (bool)false, (bool)true); break;
                                        case 46: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7FD62962, (int)999, (bool)false, (bool)true); break;
                                        case 47: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDBBD7280, (int)999, (bool)false, (bool)true); break;
                                        case 48: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x61012683, (int)999, (bool)false, (bool)true); break;

                                        case 49: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x5FC3C11, (int)999, (bool)false, (bool)true); break;
                                        case 50: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC472FE2, (int)999, (bool)false, (bool)true); break;
                                        case 51: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA914799, (int)999, (bool)false, (bool)true); break;
                                        case 52: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC734385A, (int)999, (bool)false, (bool)true); break;
                                        case 53: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x6A6C02E0, (int)999, (bool)false, (bool)true); break;
                                        case 54: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xB1CA77B1, (int)999, (bool)false, (bool)true); break;
                                        case 55: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA284510B, (int)999, (bool)false, (bool)true); break;
                                        case 56: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x42BF8A85, (int)999, (bool)false, (bool)true); break;
                                        case 57: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7F7497E5, (int)999, (bool)false, (bool)true); break;
                                        case 58: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x6D544C99, (int)999, (bool)false, (bool)true); break;

                                        case 59: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x63AB0442, (int)999, (bool)false, (bool)true); break;
                                        case 60: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x781FE4A, (int)999, (bool)false, (bool)true); break;
                                        case 61: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x93E220BD, (int)999, (bool)false, (bool)true); break;
                                        case 62: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA0973D5E, (int)999, (bool)false, (bool)true); break;
                                        case 63: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFDBC8A50, (int)999, (bool)false, (bool)true); break;
                                        case 64: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x497FACC3, (int)999, (bool)false, (bool)true); break;
                                        case 65: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x24B17070, (int)999, (bool)false, (bool)true); break;
                                        case 66: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x2C3731D9, (int)999, (bool)false, (bool)true); break;
                                        case 67: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xAB564B93, (int)999, (bool)false, (bool)true); break;
                                        case 68: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x787F0BB, (int)999, (bool)false, (bool)true); break;

                                        case 69: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBA45E8B8, (int)999, (bool)false, (bool)true); break;
                                        case 70: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x23C9F95C, (int)999, (bool)false, (bool)true); break;

                                        case 71: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x34A67B97, (int)999, (bool)false, (bool)true); break;
                                        case 72: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x60EC506, (int)999, (bool)false, (bool)true); break;
                                        case 73: WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, (int)999, (bool)false, (bool)true); break;
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button(skCrypt("Remove")))
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


                                if (ImGui::Button(skCrypt("Give All")))
                                {
                                    for (int i = 0; i < std::size(weaponsList); i++)
                                    {
                                        WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), weaponsList[i], (int)999, (bool)false, (bool)true);
                                        Sleep(1); //0.001  seconds
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button(skCrypt("Remove All")))
                                {
                                    WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::PLAYER_PED_ID(), true);
                                    Sleep(1); //0.001  seconds
                                }
                                
                            }
                            ImGui::EndChild();
                            ImGui::EndGroup();
                        }
                        ImGui::PopFont();
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Vehicle"))
                    {
                        ImGui::PushFont(font);

                        ImGui::SetCursorPosY(65);
                        ImGui::BeginGroup();
                        ImGui::SetCursorPosX(15);
                        ImGui::BeginChild(skCrypt("General"), ImVec2(300, 205), false);
                        {

                            ImGui::Text("General");
                            ImGui::Separator();


                            ImGui::Checkbox(E("Enable Vehicle Option"), &Settings::vehicle::vehicleenable);

                            if (!Settings::vehicle::vehicleenable)
                                ImGui::BeginDisabled();

                            ImGui::Checkbox(skCrypt("Auto Start Engine"), &Settings::vehicle::autostart_vehicle);
                            ImGui::Checkbox(skCrypt("Force Unlock Car"), &Settings::vehicle::unlockcar);
                            ImGui::Checkbox(skCrypt("Warp Vehicle"), &Settings::vehicle::warp_vehicle);
                            ImGui::Checkbox(skCrypt("Sticky car"), &Settings::vehicle::stickycar);
                            ImGui::Checkbox(skCrypt("Name ESP"), &Settings::vehicle::NameESP);

                            if (!Settings::vehicle::vehicleenable)
                                ImGui::EndDisabled();

                        }
                        ImGui::EndChild();
                        ImGui::EndGroup();

                        ImGui::SetCursorPosY(275); // 300
                        ImGui::BeginGroup();
                        ImGui::SetCursorPosX(15);
                        ImGui::BeginChild("Other", ImVec2(300, 180), false);
                        {
                            ImGui::Text("Other");

                            ImGui::Separator();
                            if (Settings::vehicle::on_car)
                            {
                                ImGui::Checkbox(skCrypt("GodMod"), &Settings::vehicle::godmode);
                                ImGui::Checkbox(skCrypt("Shift Boost"), &Settings::vehicle::shift_boost);

                                ImGui::Checkbox(skCrypt("Vehicle Speed"), &Settings::vehicle::vehicle_speed_bool);
                                if (Settings::vehicle::vehicle_speed_bool)
                                {
                                    ImGui::SliderInt((skCrypt("Speed multiplier")), &Settings::vehicle::vehicle_speed_value, 0, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                }
                                if (ImGui::Button(skCrypt("Repair")))
                                {
                                    Settings::vehicle::repaire = true;
                                }
                            }
                            else 
                            {
                                ImGui::Spacing();
                                ImGui::Text(skCrypt("You are not on a vehicle."));
                            }
                        }
                        ImGui::EndChild();
                        ImGui::EndGroup();

                        ImGui::SetCursorPosY(65);
                        ImGui::BeginGroup();
                        ImGui::SetCursorPosX(340);
                        ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                        {
                            ImGui::Text("Settings");

                            ImGui::Separator();
                            if (Settings::vehicle::on_car)
                            {
                                ImGui::SliderInt((skCrypt("Primary")), &Settings::vehicle::primary_color, 0, 160, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                ImGui::SliderInt((skCrypt("Secondary")), &Settings::vehicle::secondary_color, 0, 160, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                if (ImGui::Button(skCrypt("Update Colors")))
                                {
                                    Settings::vehicle::update_colors = true;
                                }

                                ImGui::Separator();

                                ImGui::SetCursorPosX(5);
                                ImGui::InputText(skCrypt("Plate Text"), Settings::vehicle::plate_text, IM_ARRAYSIZE(Settings::vehicle::plate_text));

                                if (ImGui::Button(skCrypt("Update Plate")))
                                {

                                    Settings::vehicle::update_plate = true;

                                }

                                ImGui::Separator();

                                if (ImGui::Combo(skCrypt("Vehicle Mod"), &Settings::vehicle::mod_idx, "Spoilers\0Front Bumper\0Rear Bumper\0Side Skirt\0Exhaust\0Frame\0Grille\0Hood\0Fender\0Right Fender\0Roof\0Engine\0Brakes\0Transmission\0Horns\0Suspension\0Armor\0"))
                                {
                                    switch (Settings::vehicle::mod_idx)
                                    {
                                    }
                                }

                                ImGui::SliderInt((skCrypt("Mod")), &Settings::vehicle::add_idx, 0, 40, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                                if (ImGui::Button(skCrypt("Update Mods")))
                                {
                                    Settings::vehicle::max_current_vehicle = true;
                                }
                            }
                            else {

                                ImGui::SetCursorPosX(5);
                                ImGui::Text(skCrypt("You are not on a vehicle."));
                            }
                        }
                        ImGui::EndChild();
                        ImGui::EndGroup();
                        ImGui::PopFont();

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Misc"))
                    {
                        ImGui::PushFont(font);

                        ImGui::SetCursorPosY(65);
                        ImGui::BeginGroup();
                        ImGui::SetCursorPosX(15);
                        ImGui::BeginChild(skCrypt("General"), ImVec2(300, 400), false);
                        {

                            ImGui::Text("General");

                            ImGui::Separator();

                            ImGui::Checkbox("Watermark", &Settings::Visuals::Watermark);

                            ImGui::Checkbox("Panic Mode", &Settings::misc::isPanic);

                            //ImGui::Checkbox("Stream Proof (Beta)", &Settings::Visuals::streamproof);

                            ImGui::SetCursorPosX(5);

                            ImGui::Text("Extensions");

                            ImGui::Separator();

                            //ImGui::MenuItem("Menu", NULL, &Settings::misc::menu);
                            //ImGui::MenuItem("Players List // Risky - Crash", NULL, &Settings::misc::PlayerList);
                            //ImGui::MenuItem("Vehicles List // Risky - Crash", NULL, &Settings::misc::VehicleList);
                            //ImGui::MenuItem("Lua Executor // Coming Soon", NULL, &Settings::misc::LuaExecutor);

                            ImGui::Checkbox("Players List", &Settings::misc::PlayerList);
                            ImGui::Checkbox("Vehicles List", &Settings::misc::VehicleList);
                            ImGui::Checkbox("Lua Executor // Coming Soon", &Settings::misc::LuaExecutor);

                            ImGui::Separator();
                        }
                        ImGui::EndChild();
                        ImGui::EndGroup();

                        ImGui::SetCursorPosY(65);
                        ImGui::BeginGroup();
                        ImGui::SetCursorPosX(340);
                        ImGui::BeginChild("Settings", ImVec2(350, 400), false);
                        {
                            ImGui::Text("Hotkeys");

                            ImGui::Separator();
                            ImGui::Spacing();

                            ImGui::Raidkey("Health : ", &Settings::Player::HotkeyHealth);
                            ImGui::Raidkey("Armor  : ", &Settings::Player::HotkeyArmor);
                            ImGui::Raidkey("Noclip : ", &Settings::misc::NoclipKey);
                            ImGui::Raidkey("Panic : ", &Settings::misc::PanicKey);

                            ImGui::Spacing();
                            ImGui::Spacing();

                            ImGui::Raidkey("Menu   : ", &Settings::misc::Menu_Hotkey);

                        }
                        ImGui::EndChild();
                        ImGui::EndGroup();
                        ImGui::PopFont();



                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Config"))
                    {
                        ImGui::PushFont(font);

                        ImGui::SetCursorPosY(65);
                        ImGui::BeginGroup();
                        ImGui::SetCursorPosX(15);
                        ImGui::BeginChild(skCrypt("General"), ImVec2(300, 400), false); //205 * 2 = 410
                        {
                            ImGui::Text(skCrypt("CFG : "));

                            FILE* p_stream;

                            static int style_idx = 0;
                            const char* configs[] = { "BAKIM / MAINTANCE" };

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
                                fread_s(&Settings::Player::Infinite_stamina, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Player::Gravity, sizeof(bool), sizeof(bool), 1, p_stream4);

                                //Misc
                                fread_s(&Settings::misc::NoclipKey, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::misc::PanicKey, sizeof(bool), sizeof(bool), 1, p_stream4);

                                fread_s(&Settings::misc::PlayerList, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::misc::VehicleList, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::misc::LuaExecutor, sizeof(bool), sizeof(bool), 1, p_stream4);

                                //Weapon

                                fread_s(&Settings::Weapon::DamageBoost, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Weapon::DamageBoost_value, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Weapon::ReloadV2, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Weapon::ReloadSpeed, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Weapon::NoRangeLimit, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Weapon::FastNoReload, sizeof(bool), sizeof(bool), 1, p_stream4);
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
                                fread_s(&Settings::Aimbot::AimbotPrediction, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Aimbot::crosshair, sizeof(bool), sizeof(bool), 1, p_stream4);

                                //Visuals

                                fread_s(&Settings::Visuals::Box, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::Skeleton, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::Corner, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::HealthBar, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::HealthBar_idx, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::ArmorBar, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::Snapline, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::Skeleton_Detailled, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::Skeleton_idx, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::NameEsp, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::Weapon, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::Distance, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::DistanceESP, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::Draw_Ped, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::FillBoxAlpha, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::Draw_Dead, sizeof(bool), sizeof(bool), 1, p_stream4);
                                fread_s(&Settings::Visuals::Draw_LocalPlayer, sizeof(bool), sizeof(bool), 1, p_stream4);

                                fread_s(&Settings::Visuals::Watermark, sizeof(bool), sizeof(bool), 1, p_stream4);

                                fread_s(&Settings::Visuals::SkelColor, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                                fread_s(&Settings::Visuals::SkelColorVisible, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                                fread_s(&Settings::Visuals::BoxColor, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                                fread_s(&Settings::Visuals::BoxColorVisible, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
                                fread_s(&Settings::Visuals::CornerColor, sizeof(ImColor), sizeof(ImColor), 1, p_stream4);
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
                                fwrite(&Settings::Player::Infinite_stamina, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Player::Gravity, sizeof(bool), 1, p_stream4);


                                //Misc
                                fwrite(&Settings::misc::NoclipKey, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::misc::PanicKey, sizeof(bool), 1, p_stream4);

                                fwrite(&Settings::misc::PlayerList, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::misc::VehicleList, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::misc::LuaExecutor, sizeof(bool), 1, p_stream4);

                                //Weapon

                                fwrite(&Settings::Weapon::DamageBoost, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Weapon::DamageBoost_value, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Weapon::ReloadV2, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Weapon::ReloadSpeed, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Weapon::NoRangeLimit, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Weapon::FastNoReload, sizeof(bool), 1, p_stream4);
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
                                fwrite(&Settings::Aimbot::AimbotPrediction, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Aimbot::crosshair, sizeof(bool), 1, p_stream4);

                                //Visuals

                                fwrite(&Settings::Visuals::Box, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::Skeleton, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::Corner, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::HealthBar, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::HealthBar_idx, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::ArmorBar, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::Snapline, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::Skeleton_Detailled, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::Skeleton_idx, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::NameEsp, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::Weapon, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::Distance, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::DistanceESP, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::Draw_Ped, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::FillBoxAlpha, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::Draw_Dead, sizeof(bool), 1, p_stream4);
                                fwrite(&Settings::Visuals::Draw_LocalPlayer, sizeof(bool), 1, p_stream4);

                                fwrite(&Settings::Visuals::Watermark, sizeof(bool), 1, p_stream4);

                                fwrite(&Settings::Visuals::SkelColor, sizeof(ImColor), 1, p_stream4);
                                fwrite(&Settings::Visuals::SkelColorVisible, sizeof(ImColor), 1, p_stream4);
                                fwrite(&Settings::Visuals::BoxColor, sizeof(ImColor), 1, p_stream4);
                                fwrite(&Settings::Visuals::BoxColorVisible, sizeof(ImColor), 1, p_stream4);
                                fwrite(&Settings::Visuals::CornerColor, sizeof(ImColor), 1, p_stream4);
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
                        ImGui::EndChild();
                        ImGui::PopFont();
                        ImGui::EndGroup();

                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();

                if (Settings::misc::informations)
                {
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 250));

                    if (!ImGui::Begin("Informations", &Settings::misc::informations))
                    {
                    }
                    else
                    {

                    }
                    ImGui::End();
                    ImGui::PopStyleVar();
                }

                if (Settings::misc::isPanic)
                {
                    static bool Open = NULL;
                    static bool FirstTime = NULL;

                    //misc
                    int HotkeyArmor = NULL;
                    int HotkeyHealth = NULL;

                    //noclip
                    int ForwardHotkey = NULL;
                    int BackwardHotkey = NULL;

                    bool Aimbot = false;
                    bool visualsenable = false;
                    bool playersenable = false;
                    bool vehicleenable = false;

                    //Disable Aimbot
                    bool Enable_Sniper;
                    bool Enable_Shotgun;
                    bool Enable_Pistol;
                    bool Enable_SMG;
                    bool Enable_Light;
                    bool Enable_Rifle;
                    bool crosshair = false;
                    bool silentAim = false;

                    //Self

                    bool NoClip = false;
                    int noclipspeed = false;
                    bool Gravity = false;
                    bool fastrun = false;
                    int RunSpeedToSet = 1;

                    bool SetHealth = false;
                    bool SetArmor = false;

                    //Weapon
                    bool InfiniteAmmo = false;
                    bool DamageBoost = false;
                    bool FastNoReload = false;
                    bool NoReload = false;
                    bool No_Recoil = false;
                    bool No_Spread = false;
                    bool NoRangeLimit = false;
                    bool rapidfire = false;

                    bool GiveWeapon = false;
                    bool GiveAllWeapon = false;
                    bool ReloadV2 = false;

                    bool Invisible = false;
                    bool AntiHS = false;
                    bool Infinite_stamina = false;

                    bool flymode = false;

                    bool EnableFovChange = false;
                    bool SuperJump = false;
                    bool TpToWaypoint = false;
                    bool NoRagDoll = false;
                    bool NoClipSpeed_bool = false;
                    int Speed = 0;
                    bool AutoShoot = false;
                    bool MagicLikeEAGLE = false;
                    bool SilentLikeEAGLE = false;
                    bool magic = false;
                    bool Aimmousewhilesilent = false;
                    bool Box = false;
                    bool Filled = false;
                    bool Corner = false;
                    bool TroiDBox = false;
                    bool Skeleton = false;
                    bool Snapline = false;
                    bool flags = false;
                    bool HealthBar = false;
                    bool ArmorBar = false;
                    bool Weapon = false;
                    bool DistanceESP = false;
                    bool NameEsp = false;
                    bool PreviewEsp = false;
                    bool tpped = false;
                    bool invisibleped = false;
                    bool killped = false;
                    bool PlayerName = false;
                    bool VisibleCheck = false;
                    float CornerSize = 1;
                    float BoxSize = 1;
                    int Skeleton_idx = 0;
                    int HealthBar_idx = 0;
                    bool HealthBar_Multicolor = false;
                    bool Skeleton_Detailled = false;
                    bool Keybinds = false;
                    bool Watermark = false;
                    bool streamproof = false;
                    bool Draw_LocalPlayer = false;
                    bool Draw_Dead = false;
                    bool Draw_Ped = false;
                    bool Draw_Animals = false;
                    bool StaticESP = false;
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
                    bool autodrive = false;
                    bool spawn_vehicle = false;
                    bool update_colors = false;
                    bool update_plate = false;
                    bool menu = NULL;
                    int Menu_Hotkey = NULL; //F5
                    int NoclipKey = NULL;
                    bool customization = false;
                    bool informations = false;
                    bool LuaExecutor = false;
                    bool PlayerList = false;
                    bool PedList = false;
                    bool VehicleList = false;
                    bool EnableChat = false;
                }
            }
            ImGui::End();
        }
    //}

}
