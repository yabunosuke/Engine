#pragma once

#include <stack>
#include <string>
#include <memory>

#include "Common/Singleton.h"
#include "Common/Property.h"
#include "System/Looper.h"

class AbstractScene
{
public:
	AbstractScene(const std::string &sceneName, int parameter) {};
	
	/// <summary>
	/// ����������
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	bool Update();

	bool FixedUpdate();

	const bool Draw();

	/// <summary>
	/// �I������
	/// </summary>
	/// <returns></returns>
	bool Finalize();
};

class SceneManager final
	: public Singleton<SceneManager>
{
public:
	friend class Singleton<SceneManager>; // �C���X�^���X�쐬������
	friend class Looper;

	/// <summary>
	/// �V�[�������[�h����
	/// </summary>
	/// <param name="sceneName">�ǂݍ��݂����V�[����</param>
	/// <param name="parameta">�p�����[�^</param>
	/// <returns></returns>
	bool LoadScene(const std::string &sceneName, bool clearFlag = false, int parameta = 0);

	yEngine::ReadOnlyProperty<int> sceneCount			// ���݃��[�h����Ă���V�[���̑���
	{
		nullptr,
		[this]() -> int {return scene_stack_.size(); }
	};
private:

	/// <summary>
	/// �V�[���ύX����
	/// </summary>
	void SceneChanger();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �X�V
	/// </summary>
	void FixedUpdate();

	enum class ChangeType
	{
		kNone,
		kPush,
		kPop
	};

	struct SceneChangeData
	{
		ChangeType type;
		std::string scene_name;
		bool is_clear;
		int parameter;
	}scene_change_data_{};

	std::stack<std::unique_ptr<AbstractScene>> scene_stack_;		// ���ݓǂݍ��܂�Ă���V�[���X�^�b�N


};

