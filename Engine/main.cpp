#include <d3dx12.h>
#include "System/SystemMain.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// システム生成
	SystemMain system;
	
	// 初期化
	if (system.StartUp())
	{
		// 実行
		system.Run();
	}
	// 終了処理
	system.Finalize();

	return 0;
}