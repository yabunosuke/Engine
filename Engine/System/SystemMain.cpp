#include "SystemMain.h"

#include "Screen.h"
#include "DirectXCommon.h"
#include "Manager/TextureManager.h"
#include "Manager/PipelineManager.h"
#include "System/Time.h"
#include "System/Input.h"


bool SystemMain::StartUp()
{
    Screen::CreateSingleton();                  // �X�N���[������
    DirectXCommon::CreateSingleton();           // DirectX12����
    TextureManager::CreateSingleton();          // �e�N�X�`���}�l�[�W������
    PipelineManager::CreateSingleton();         // �p�C�v���C������
    // �I�[�f�B�I�}�l�[�W��
    // �^�C���N���X�̃C���X�^���X����
    Time::CreateSingleton();                    // ���Ԍ`����
    Input::GetInstance().CreateSingleton();		// �C���v�b�g������

    looper = std::make_unique<Looper>();
    return true;
}

bool SystemMain::Finalize() const
{
    SingletonFinalizer::Finalize();     // singleton���
	looper->Finalize();
    return true;
}

void SystemMain::Run() const
{
    while (Screen::GetInstance().WindowMessage())
    {
        if (!looper->Run())
        {
            break;
        }
    }
}
