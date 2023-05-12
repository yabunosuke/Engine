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
    
    // �I�u�W�F�N�g�`��

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
    case ChangeType::kNone: // �X�V���Ȃ��ꍇ�̓X�L�b�v
        return;
        
    case ChangeType::kPush: // �ǉ�����
        // �V�[������o�Ȃ��ꍇ�͏I������
        if (scene_stack_.size() != 0)
        {
            scene_stack_.top()->Finalize();
        }

        // �X�^�b�N���N���A����
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

    case ChangeType::kPop:  // �߂鏈��
        scene_stack_.top()->Finalize();
        scene_stack_.pop();

        break;

    default:
        break;
    }

    // �ύX�p�f�[�^��������
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

