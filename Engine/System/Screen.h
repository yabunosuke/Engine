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

	yEngine::Property<int> Width			// ����
	{
		&width_
	};

	yEngine::Property<int> Height			// ����
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
	/// �R���X�g���N�^
	/// </summary>
	Screen();

	bool StartUp() override;

	static LRESULT CALLBACK StaticWndProc(
		HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam
	);

	HWND hwnd_;					// �E�B���h�E�n���h��
	WNDCLASSEX wnd_class_;

	wchar_t window_name_[32];	// �E�B���h�E��
	int width_;		// ����
	int height_;	// �c��


};

