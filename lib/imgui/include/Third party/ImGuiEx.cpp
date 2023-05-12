#include "ImGuiEx.h"
#include <math.h>
#include <vector>

using namespace ImGui;
namespace ImGuiEx 
{
	bool Filter(
		const char *label,
		char *buf, 
		int bufSize, 
		float width, 
		bool setKeyboardFocus
	)
	{
		assert(bufSize > 0);
		bool changed{ false };	//変更フラグ
		
		// 入力削除ボタン
		if (Button("x"))
		{
			buf[0] = '\0';
			changed = true;
		}
		SameLine();
		SetNextItemWidth(width);
		if (setKeyboardFocus)
		{
			SetKeyboardFocusHere();
		}
		changed = InputTextWithHint("##filter", label, buf, bufSize, ImGuiInputTextFlags_AutoSelectAll);
		return changed;
	}

	bool InputText(
		const char *label,
		std::string &buf,
		const int &bufSize,
		ImGuiInputTextFlags flags

	)
	{
		bool changed{ false };	//変更フラグ
	
		char *c_buf = new char[bufSize];
		std::char_traits<char>::copy(c_buf, buf.c_str(), buf.size() + 1);

		if (ImGui::InputText(label, c_buf, bufSize, flags))
		{
			buf = c_buf;
			changed = true;
		}

		delete[] c_buf;

		return changed;
	}
}