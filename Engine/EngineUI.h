#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

class EngineUI
{
public:
	void Draw();

private:
	// メインメニュー
	void MainMenu();

	// ファイル関係のプルダウンメニュー
	void FileMenu();

	void WindowMenu();

	/// <summary>
	/// オブジェクトの親子関係表示
	/// </summary>
	void Hierarchy();

	/// <summary>
	/// オブジェクトの情報表示
	/// </summary>
	void Inspector();

};

