#include "SceneManager.h"


bool AbstractScene::Initialize()
{
    return false;
}

bool AbstractScene::Update()
{
    return false;
}

bool AbstractScene::FixedUpdate()
{
    return false;
}

const bool AbstractScene::Draw()
{
    
    // オブジェクト描画

    return false;
}

bool AbstractScene::Finalize()
{
    return false;
}

bool SceneManager::LoadScene(const std::string &sceneName,bool clearFlag, int parameta)
{
    scene_change_data_.type = ChangeType::kPush;
    scene_change_data_.scene_name = sceneName;
    scene_change_data_.is_clear = false;
    scene_change_data_.parameter = 0;
    return false;
}

void SceneManager::SceneChanger()
{
    switch (scene_change_data_.type)
    {
    case ChangeType::kNone: // 更新がない場合はスキップ
        return;
        
    case ChangeType::kPush: // 追加処理
        // シーンが空出ない場合は終了処理
        if (scene_stack_.size() != 0)
        {
            scene_stack_.top()->Finalize();
        }

        // スタックをクリア処理
        if (scene_change_data_.is_clear)
        {
            while (!scene_stack_.empty())
            {
                scene_stack_.pop();
                scene_stack_.top()->Finalize();
            }
        }
        scene_stack_.push(
            std::make_unique<AbstractScene>(
                scene_change_data_.scene_name, scene_change_data_.parameter)
        );

        scene_stack_.top()->Initialize();

        break;

    case ChangeType::kPop:  // 戻る処理
        scene_stack_.top()->Finalize();
        scene_stack_.pop();

        break;

    default:
        break;
    }

    // 変更用データを初期化
    scene_change_data_.type = ChangeType::kNone;
    scene_change_data_.scene_name = "";
    scene_change_data_.is_clear = false;
    scene_change_data_.parameter = 0;


}

void SceneManager::Update()
{
    _RPT0(_CRT_WARN, "Update\n");
    scene_stack_.top()->Update();
}

void SceneManager::FixedUpdate()
{

    _RPT0(_CRT_WARN, "FixedUpdate\n");
    scene_stack_.top()->FixedUpdate();
}

