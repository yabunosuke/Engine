#pragma once

#include <Windows.h>
#include "Common/Property.h"
#include "Common/Singleton.h"

class Screen final :
	public Singleton<Screen>
{
public:
	friend class Singleton<Screen>;


	/// <summary>
	/// </summary>
	bool WindowMessage();

	yEngine::Property<int> Width			// 横幅
	{
		&width_
	};

	yEngine::Property<int> Height			// 横幅
	{ 
		&height_
	};

	yEngine::ReadOnlyProperty<HWND> Hwnd
	{
		&hwnd_
	};


	yEngine::Property<WNDCLASSEX> WndClass
	{
		&wnd_class_
	};

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Screen();

	bool StartUp() override;

	static LRESULT CALLBACK StaticWndProc(
		HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam
	);

	HWND hwnd_;					// ウィンドウハンドル
	WNDCLASSEX wnd_class_;

	wchar_t window_name_[32];	// ウィンドウ名
	int width_;		// 横幅
	int height_;	// 縦幅


};

