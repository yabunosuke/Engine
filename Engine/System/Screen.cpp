#include "Screen.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

Screen::Screen() : 
	window_name_(L"ヤブナカエンジン"),
	hwnd_(nullptr),
	width_(1280),
	height_(720)
{
	// ウィンドウクラスの設定
	wnd_class_.cbSize = sizeof(WNDCLASSEX);
	wnd_class_.lpfnWndProc = (WNDPROC)Screen::StaticWndProc;		//ウィンドウプロ―ジャを設定
	wnd_class_.cbClsExtra = 0;					// メモリの動的確保
	wnd_class_.cbWndExtra = 0;					// インスタンス生成時の確保領域
	wnd_class_.hInstance = GetModuleHandle(nullptr);	//ウィンドウハンドル
	wnd_class_.lpszClassName = window_name_;	//ウィンドウクラス名
	wnd_class_.hCursor = LoadCursor(NULL, IDC_ARROW);	//カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&wnd_class_);

	//ウィンドウサイズ{x,y,横幅,縦幅}
	RECT wrc = { 0,0,width_,height_ };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd_ = CreateWindow(
		wnd_class_.lpszClassName,
		window_name_,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wnd_class_.hInstance,
		nullptr
	);
}

bool Screen::StartUp()
{
	//ウィンドウ表示
	ShowWindow(hwnd_, SW_SHOW);
    
	return true;
}


LRESULT CALLBACK Screen::StaticWndProc(
	HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	RECT clientRect{};

	// ウィンドウ破棄検出
	switch (msg) {
	case WM_SIZE:		// ウィンドウのサイズ変更検知
		//縦横を更新
		GetClientRect(hwnd, &clientRect);
		Screen::GetInstance().Width = clientRect.right - clientRect.left;
		Screen::GetInstance().Height = clientRect.bottom - clientRect.top;

		break;
	case WM_DESTROY:	// ウィンドウが破棄された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		break;
	default:
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

bool Screen::WindowMessage()
{
	MSG msg{};
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // プロシージャにメッセージを送る
	}

	// ✖ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT) {
		return false;
	}

	return true;
}