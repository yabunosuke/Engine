#include "SystemMain.h"

#include "Screen.h"
#include "DirectXCommon.h"
#include "Manager/TextureManager.h"
#include "Manager/PipelineManager.h"
#include "System/Time.h"
#include "System/Input.h"


bool SystemMain::StartUp()
{
    Screen::CreateSingleton();                  // スクリーン生成
    DirectXCommon::CreateSingleton();           // DirectX12生成
    TextureManager::CreateSingleton();          // テクスチャマネージャ生成
    PipelineManager::CreateSingleton();         // パイプライン生成
    // オーディオマネージャ
    // タイムクラスのインスタンス生成
    Time::CreateSingleton();                    // 時間形成性
    Input::GetInstance().CreateSingleton();		// インプット初期化

    looper = std::make_unique<Looper>();
    return true;
}

bool SystemMain::Finalize() const
{
    SingletonFinalizer::Finalize();     // singleton解放
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
