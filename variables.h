#pragma once
#include <string>
#include "skCrpyt.h"
#include "auth.hpp"

using namespace KeyAuth;

namespace Api {
	std::string name = (std::string)skCrypt("TestCheat"); // application name. right above the blurred text aka the secret on the licenses tab among other tabs
	std::string ownerid = (std::string)skCrypt("V7T8m7JS8c"); // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
	std::string secret = (std::string)skCrypt("4d6591330c6e4e31c053c5ee70f6d21833c9eb358252bfa87456c5ddea156387"); // app secret, the blurred text on licenses tab and other tabs
	std::string version = (std::string)skCrypt("1.0"); // leave alone unless you've changed version on website
	std::string url = (std::string)skCrypt("https://keyauth.win/api/1.2/"); // change if you're self-hosting
	std::string pin = (std::string)skCrypt("ssl pin key (optional)");

	api KeyAuthApp(name, ownerid, secret, version, url, pin);
}