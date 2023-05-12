#pragma once
//#include "Scene/AbstractScene/AbstractScene.h"
//#include "IoChangedListener.h"
#include <stack>
#include <memory>
#include <string>
//#include "ImGui/imguiManager.h"
//#include "ImGui/ImGuizmo.h"
//#include "Editor.h"


//#include "2d/PostEffect.h"
//#include "MultiRenderTarget/MulutiRenderTarget.h"

//#include "Common/EntityFolders.h"

#include "EngineUI.h"
#include "Render/MulutiRenderTarget.h"


class Looper final
{	
public:
	Looper();
	bool Run();
	
	void Finalize();

	//void ExitWindow() override { exit_window_ = true; };
private:

	/// <summary>
	/// ImGuiの初期化
	/// </summary>
	/// <returns></returns>
	bool ImGuiInitialize();

	/// <summary>
	/// ImGui毎フレーム初期化
	/// </summary>
	void ImGuiBeginFrame() const;
	
	/// <summary>
	/// ImGui描画コマンド発行
	/// </summary>
	void ImGuiDraw();

	//SceneManager scene_manager_;


	//void OnHierarchyGUI();		// hierarchy描画
	//bool is_hierarchy_open_{ true };
	//void OnInspectorGUI();		// inspector描画
	//bool is_inspector_open_{ true };
	//void ComponentListUI();

	//bool exit_window_{ false };


	//// ポストエフェクト
	//std::unique_ptr<PostEffect> post_effect_;
	// マルチレンダーターゲット
	MulutiRenderTarget muluti_render_target_;

	EngineUI engine_ui_;

	//void FolderUI(std::weak_ptr<GameObject> gameObject, yEngine::u32 level);
	//void FolderUI(std::weak_ptr<yEngine::EntityFolders::Folder> folder, yEngine::u32 level);
	//void EntitiyUI(std::weak_ptr<yEngine::EntityFolders::Entity> entity, yEngine::u32 level);
	//Editor editor;
	//bool play_{ true };


	//yEngine::EntityFolders component_folder_;
};

