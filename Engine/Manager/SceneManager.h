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
	/// 初期化処理
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	bool Update();

	bool FixedUpdate();

	const bool Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <returns></returns>
	bool Finalize();
};

class SceneManager final
	: public Singleton<SceneManager>
{
public:
	friend class Singleton<SceneManager>; // インスタンス作成を許可
	friend class Looper;

	/// <summary>
	/// シーンをロードする
	/// </summary>
	/// <param name="sceneName">読み込みたいシーン名</param>
	/// <param name="parameta">パラメータ</param>
	/// <returns></returns>
	bool LoadScene(const std::string &sceneName, bool clearFlag = false, int parameta = 0);

	yEngine::ReadOnlyProperty<int> sceneCount			// 現在ロードされているシーンの総数
	{
		nullptr,
		[this]() -> int {return scene_stack_.size(); }
	};
private:

	/// <summary>
	/// シーン変更処理
	/// </summary>
	void SceneChanger();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 更新
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

	std::stack<std::unique_ptr<AbstractScene>> scene_stack_;		// 現在読み込まれているシーンスタック


};

