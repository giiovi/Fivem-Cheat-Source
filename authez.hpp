#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <io.h> 
#include <fcntl.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>
#include "xorstr.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <regex>

#pragma comment(lib, "libcurl.lib")

#define CURL_STATICLIB 

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

std::string license;



std::string hwid = GetProcessorId();

std::atomic<bool> auth_thread_running{ false };




void typeTextSlowly(const std::string& text, WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);

    for (char c : text) {
        std::cout << c;
        std::this_thread::sleep_for(std::chrono::milliseconds(65));
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}






const std::string _xauth_authenticate = "https://xauth.georgexv.ru/login";
const std::string _xauth_handshake = "https://xauth.georgexv.ru/handshake";
const std::string _xauth_session = "_xauth_session";


std::string extractSessionCookie(const cpr::Header& headers) {
    const auto& it = headers.find("Set-Cookie");
    if (it != headers.end()) {
        std::regex cookie_pattern(_xauth_session + R"(=([^;]+))");
        std::smatch match;

        if (std::regex_search(it->second, match, cookie_pattern)) {
            return _xauth_session + "=" + match[1].str();
        }
    }

    return "";
}


std::string authenticate(const std::string& username, const std::string& password, const std::string& hwid) {
    cpr::Response response = cpr::Post(cpr::Url{ _xauth_authenticate },
        cpr::Header{
     {"Content-Type", "application/json"},
     {"Authorization", "iYW67a2YVOHHCu7RKi96QmF3quYmiz4n5oijfDEr6v4="}
        },
        cpr::Body{ R"({"username":")" + username + R"(","password":")" + password + R"(","hwid":")" + hwid + R"("})" });

    if (response.status_code == 200) {
        std::string sessionCookie = extractSessionCookie(response.header);
        if (!sessionCookie.empty()) {

            //WriteToJson("login.json", "username", username, true, "password", password);
            return sessionCookie;
        }
        else {

            typeTextSlowly("		 [!] Invalid Credentials", FOREGROUND_RED);
            std::remove("login.json");
            Sleep(1500);
            exit(0);
        }
    }
    return "";
}

std::string register123(const std::string& username, const std::string& password)
{

    cpr::Response response = cpr::Get(
        cpr::Url{ "https://xauth.georgexv.ru/register" },
        cpr::Header{
            {"Content-Type", "application/json"},
            {"Authorization", "iYW67a2YVOHHCu7RKi96QmF3quYmiz4n5oijfDEr6v4="}
        },
        cpr::Body{ R"({"username":")" + username + R"(","password":")" + password + R"("})" });


    // Check for a successful request and print the response
    if (response.status_code == 200) {
        std::cout << "Response: " << response.text << std::endl;
    }
    else {
        std::cout << "Error: Status code " << response.status_code << std::endl;
    }

}


std::string redeem(const std::string& username, const std::string&license)
{


    cpr::Response response = cpr::Get(
        cpr::Url{ "https://xauth.georgexv.ru/users/add/sub" },
        cpr::Header{
            {"Content-Type", "application/json"},
            {"Authorization", "iYW67a2YVOHHCu7RKi96QmF3quYmiz4n5oijfDEr6v4="}
        },
        cpr::Parameters{
            {"username", username},
            {"license", license}
        }
    );

    // Check for a successful request and print the response
    if (response.status_code == 200) {
        std::cout << "Response: " << response.text << std::endl;
    }
    else {
        std::cout << "Error: Status code " << response.status_code << std::endl;
    }

}


void login(const std::string& username, const std::string& password)
{
    std::string sessionCookie = authenticate(username, password, hwid);

    if (!sessionCookie.empty()) {

        cpr::Response protectedResponse = cpr::Get(cpr::Url{ _xauth_handshake },
            cpr::Header{
                {"Cookie", sessionCookie},
                {"Authorization", "iYW67a2YVOHHCu7RKi96QmF3quYmiz4n5oijfDEr6v4="}
            });

        if (protectedResponse.status_code == 200) {
           // WriteToJson("login.json", "username", username, true, "password", password);
             //std::cout << "Handshake data:" << std::endl;
            nlohmann::json jsonData = nlohmann::json::parse(protectedResponse.text);
            nlohmann::json subscriptions = jsonData["subscriptions"];

            if (subscriptions.is_null()) {
                MessageBoxA(NULL, "No subscription meaning this is either a free account or subscription expired.", "MEDUSA", LB_OKAY);

                exit(0);
            }
            // std::string mpampis = protectedResponse.text;
             //printf(mpampis.c_str());
             //printJsonDataAsTable(jsonData);
            // printJsonDataAsTableArray(subscriptions);


            // Iterate through the subscriptions array and check the plans
            for (const auto& subscription : subscriptions) {
                int plan = subscription["Plan"];

                if (plan == 1) {
                    hasPlan1 = true;
                }
                else if (plan == 2) {
                    hasPlan2 = true;
                }
            }

            if (hasPlan1 && hasPlan2) {

            }
            else if (hasPlan1) {
                system("start https://www.youtube.com/watch?v=dQw4w9WgXcQ");
                MessageBoxA(NULL, "bro, you only have plan for spoofer not cheat :D ", "Medusa", LB_OKAY);
                //system("taskill /f /im fivem.exe 2>nul");
                Sleep(5000);
                exit(0);
            }
            else if (hasPlan2) {

            }
            else {
                system("start https://www.youtube.com/watch?v=dQw4w9WgXcQ");
                system("taskill /f /im fivem.exe 2>nul");
                exit(0);
            }



        }
        else {
            typeTextSlowly("		 [!] Skill Issue", FOREGROUND_RED);
            std::remove("login.json");
            Sleep(1500);
            exit(0);
        }
    }
    else {
        typeTextSlowly("		 [!] Invalid Credentials", FOREGROUND_RED);
        std::remove("login.json");
        Sleep(1500);
        exit(0);
    }
}





