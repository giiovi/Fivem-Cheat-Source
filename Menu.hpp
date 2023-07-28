#pragma once

#include "includes.hpp"

namespace Menu
{
	static bool Open = true;
	static bool FirstTime = true;
	static int Tab = 0;

	const char* title_text = "RAIDAIM V1.8";
	std::string project_version = "1.8";

	void Style(void);
	void Drawing(void);

	static ImFont* BiggestIcon;
	static ImFont* BiggestFont;
	static ImFont* littleFont;

}