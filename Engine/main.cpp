#include <d3dx12.h>
#include "System/SystemMain.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �V�X�e������
	SystemMain system;
	
	// ������
	if (system.StartUp())
	{
		// ���s
		system.Run();
	}
	// �I������
	system.Finalize();

	return 0;
}