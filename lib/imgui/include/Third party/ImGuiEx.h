#pragma once

#include <ImGui/imgui.h>
#include <string>
#include <array>

namespace ImGuiEx
{
	IMGUI_API bool Filter(
		const char *label, 
		char *buf, 
		int bufSize, 
		float width = -1, 
		bool setKeyboardFocus = false);

	IMGUI_API bool InputText(
		const char *label,
		std::string &buf,
		const int &bufSize,
		ImGuiInputTextFlags flags = 0
	);
}