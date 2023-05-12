#include "Looper.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#include "DirectXCommon.h"
#include "Input.h"
#include "Common/EntityFolders.h"
#include "Manager/PipelineManager.h"
#include "Manager/SceneManager.h"
#include "Manager/TextureManager.h"
#include "Screen.h"
#include "Time.h"

//#include "AudioManager.h"	//音声管理
//#include "Scene/Manager/CameraManager.h"
////#include "KeyboardInput.h"
//#include "Input.h"
//#include "PrimitiveRenderer.h"
//#include "Time/Time.h"
//#include "Collision/CheckCollision.h"
////シーン
//#include "TitleScene.h"
//#include "GameScene.h"
//
//#include "Object/GameObject/GameObject.h"
//#include "Object/Component/Camera.h"
//#include "Object/Component/Light.h"
//#include "Object/Component/ModelRenderComponent.h"
//#include "Texture.h"
//
//
//#include "Singleton/Singleton.h"
//
//
//#include <cereal/types/memory.hpp>
//#include <cereal/types/polymorphic.hpp>
//
//#include "ImGui/Third party/ImGuiEx.h"
//#include "Common/EntityFolders.h"
//
//
//Sprite *sprite;
//int sprite_num;

Looper::Looper()
{
	SceneManager::CreateSingleton();				// シーンマネージャー生成
	SceneManager::GetInstance().LoadScene("Test");	// スタートシーン読み込み

	//sprite_num = TextureManager::LoadTexture(DirectXCommon::dev.Get(), L"Assets/2d/Panorama-V01.png");
	//sprite = Sprite::Create(DirectXCommon::dev, sprite_num);
	//sprite->SetSize({ 1280 ,720 });
	//sprite->SetPosition({ 1280 / 2 ,720 / 2 });


	//post_effect_ = std::make_unique<PostEffect>();
	//post_effect_->InitializePostEffect(DirectXCommon::dev);

	//muluti_render_target_ = std::make_unique<MulutiRenderTarget>();
	//muluti_render_target_->InitializeMulutiRenderTarget(DirectXCommon::dev);

	ImGuiInitialize();												// ImGui初期化

	yEngine::EntityFolders folders;

	TextureManager::GetInstance().LoadTexture(L"Asset/ahoge.png");
}

bool Looper::Run()
{
	SceneManager::GetInstance().SceneChanger();

	// 計測開始
	//Time::GetInstance().InstrumentationStart();

	//GuiBeginFrame();

	//// エディタ描画
	//editor.Draw();


	//// 追加処理
	//scene_stack_.top()->GetGameObjectManager()->AddObjects();

	//キーボード更新
	Input::GetInstance().Update();

	// ImGui開始
	ImGuiBeginFrame();

	//if(play_) {
		// 物理挙動アップデート
	{
		//		for (int i = 0; Singleton<Time>::GetInstance().CheckFixedUpdate(); ++i)
		//		{

		//			// 当たり判定
		//			CheckCollision::CheckColliders(scene_stack_.top()->GetGameObjectManager()->game_objects_);

		//			// 実際の移動
		SceneManager::GetInstance().FixedUpdate();

		//			// 経過時間を減少させる
		//			Singleton<Time>::GetInstance().SubFixedTimer();
		//			//CheckCollision::PenaltyCalc();
		//			// 2回処理して改善しなければ強制的に離脱
		//			if (i > 0)
		//			{
		//				Singleton<Time>::GetInstance().ClearFixedTimer();
		//				break;
		//			}
		//		}
	}
	//	// 各種初期化
	//	PrimitiveRenderer::GetInstance().FrameInitialize();	// プリミティブのバッファインデックス初期化


		//シーンの更新
	SceneManager::GetInstance().Update();
	//}

	//// 削除処理
	//Object::Destroyer();


	
	////シーンの描画コマンドを発行
	//muluti_render_target_->PreDrawScene(DirectXCommon::dev, DirectXCommon::cmdList);
	//scene_stack_.top()->Draw();																	// 描画コマンド発行
	//muluti_render_target_->PostDraw(DirectXCommon::cmdList);
	//// ここまでの描画はマルチレンダーターゲットの対象

	//post_effect_->PreDraw(DirectXCommon::cmdList);
	//// スカイボックス描画
	//sprite->Draw(DirectXCommon::dev, DirectXCommon::cmdList, "SkyBox");
	//muluti_render_target_->DrawRenderTarget(
	//	DirectXCommon::dev, DirectXCommon::cmdList,
	//	scene_stack_.top()->GetLightManager().get()
	//);
	//post_effect_->PostDraw(DirectXCommon::cmdList);

	////バッファクリア
	//DirectXCommon::ResourceBarrierWriting();
	//DirectXCommon::ScreenClear();
	//

	//// ポストエフェクトの描画
	//post_effect_->DrawPostEffect(DirectXCommon::cmdList);

	//
	//imguiManager::GetIns()->Draw();
	///*PrimitiveRenderer::GetInstance().DrawBox(
	//	DirectXCommon::cmdList,
	//	PrimitiveRenderer::Box{ {0,10,0},{15,15,15},{20,20,20} },
	//	Camera::main.r_->viewMatrix.r_ * Camera::main.r_->projectionMatrix.r_
	//);*/

	muluti_render_target_.PreDraw();
	muluti_render_target_.PostDraw();


	//バッファクリア
	DirectXCommon::GetInstance().ResourceBarrierWriting();

	/*auto tex_data = TextureManager::GetInstance().GetTexture(L"Asset/ahoge.png");
	ImGui::Begin("Test");
	ImGui::Image(ImTextureID(tex_data->GetCPUHandle().ptr), { static_cast<float>(tex_data->Width),static_cast<float>(tex_data->Height) });
	ImGui::End();*/

	

	engine_ui_.Draw();
	ImGuiDraw();										// ImGuiコマンド発行
	DirectXCommon::GetInstance().PlayCommandList();		// 全コマンド実行


	//// 計測終了
	//Singleton<Time>::GetInstance().InstrumentationEnd();
	//if (play_)
	//{
	//	Singleton<Time>::GetInstance().AddFixedTimer();
	//}

	//return !exit_window_;
	return true;
}

void Looper::Finalize()
{
	ImGui_ImplDX12_Shutdown();
}

//void Looper::GuiBeginFrame() const
//{
//	ImGui_ImplDX12_NewFrame();
//	ImGui_ImplWin32_NewFrame();
//	ImGui::NewFrame();
//	ImGuizmo::BeginFrame();
//
//}
//
//void Looper::GuiEndFrame()
//{
//
//	ImGuiWindowFlags flags =
//		ImGuiWindowFlags_NoTitleBar |
//		ImGuiWindowFlags_NoResize |
//		ImGuiWindowFlags_NoMove | 
//		ImGuiWindowFlags_NoSavedSettings |
//		ImGuiWindowFlags_NoBringToFrontOnFocus | 
//		ImGuiWindowFlags_NoNavFocus |
//		ImGuiWindowFlags_NoDocking;
//
//	// ウィンドウサイズが有効なときだけ描画
//	if (true)
//	{
//
//
//		ImGui::Begin("playtest");
//		ImGui::Checkbox("IsPlay", &play_);
//		ImGui::End();
//
//		/*ImGui::Begin("MainDockspace", nullptr, flags);
//		ImGui::PopStyleVar();
//		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
//		ImGui::DockSpace(dockspace_id, ImVec2(0, 0));
//		ImGui::End();*/
//		MainMenu();			// メインメニュー
//
//		OnHierarchyGUI();	// ヒエラルキー描画
//		OnInspectorGUI();	// インスペクター描画
//	}
//
//
//	////ウィンドウ名定義
//	//ImGui::Begin("PostEffectShader");
//	//ImGui::SetWindowSize(
//	//	ImVec2(400, 500),
//	//	ImGuiCond_::ImGuiCond_FirstUseEver
//	//);
//	//for (int i = 0; i < _countof(PipelineManager::GetInstance()->posteffect_shader_list_); ++i)
//	//{
//	//	if (ImGui::Button(PipelineManager::GetInstance()->posteffect_shader_list_[i].c_str())) {
//	//		post_effect_->shader_name_ = PipelineManager::GetInstance()->posteffect_shader_list_[i];
//	//	}
//	//	if (i % 4 != 0 || i == 0)
//	//	{
//	//		ImGui::SameLine();
//
//	//	}
//	//}
//}
//
//
//void Looper::OnHierarchyGUI()
//{
//	//ウィンドウ名定義
//	if (ImGui::Begin("Hierarchy", &is_hierarchy_open_))
//	{
//		// シーン名
//		ImGui::Text(scene_stack_.top()->GetName().c_str());
//
//		// オブジェクト検索
//		static char filter[64] = "";
//		ImGuiEx::Filter("Filter", filter, sizeof(filter), 200, ImGui::IsWindowAppearing());
//
//		if (ImGui::BeginChild("entities"))
//		{
//			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
//			
//			// フィルターがないとき
//			if (filter[0] == '\0')
//			{
//				// ルートフォルダ描画
//
//
//
//
//				for (const auto &object : scene_stack_.top()->GetGameObjectManager()->game_objects_)
//				{
//					if (!object.lock()->GetPearent().expired()) continue;
//					FolderUI(object, 0);
//
//				}
//			}
//			// 検索結果があるとき
//			else
//			{
//				for (const auto &object : scene_stack_.top()->GetGameObjectManager()->game_objects_)
//				{
//
//					// 合致しない場合はコンテニュー
//					if (object.lock()->name->find(filter) == std::string::npos)
//					{
//						continue;
//					}
//
//					ImGui::PushID(object.lock()->GetInstanceID());
//
//					// テーブル設定
//					if (ImGui::Selectable(object.lock()->name->c_str(), object.lock()->GetInstanceID() == editor.selectID))
//					{
//						// 選択
//						editor.selectID = object.lock()->GetInstanceID();
//
//					}
//
//					ImGui::PopID();
//				}
//			}
//		}
//		ImGui::EndChild();
//	}
//
//	//終了
//	ImGui::End();
//
//}
//
//void Looper::OnInspectorGUI()
//{
//	ImGui::Begin("Inspector", &is_inspector_open_);
//
//	std::weak_ptr<GameObject> select_object = scene_stack_.top()->GetGameObjectManager()->GetGameObject(editor.selectID);
//
//	// IDに合う物がなければ未表示
//	if (select_object.expired()) {
//		ImGui::End();
//		return;
//	}
//
//
//	//インスペクター描画
//	ImGui::PushID(select_object.lock()->GetInstanceID());
//	select_object.lock()->DrawData();
//	ImGui::PopID();
//
//	// ポップアップウィンドウ
//	// コンポーネントの追加
//	if (ImGui::Button("Add Component"))
//	{
//		ImGui::OpenPopup("AddComponentPopup");
//	}
//	if (ImGui::BeginPopup("AddComponentPopup", ImGuiWindowFlags_AlwaysAutoResize))
//	{
//
//		// コンポーネント検索欄
//		static char filter[64] = "";
//		ImGuiEx::Filter("Filter", filter, sizeof(filter), 200, ImGui::IsWindowAppearing());
//
//		// フィルターがなければ全コンポーネントを表示
//		if (filter[0] == '\0')
//		{
//			// テストボタン
//			if (ImGui::Button("Camera"))
//			{
//				select_object.lock()->AddComponent<Camera>();
//			}
//			if (ImGui::Button("OBB Collider"))
//			{
//				select_object.lock()->AddComponent<OBBCollider>();
//			}
//			if (ImGui::Button("Sphere Collider"))
//			{
//				select_object.lock()->AddComponent<SphereCollider>();
//			}
//			if (ImGui::Button("Model Renderer"))
//			{
//				select_object.lock()->AddComponent<ModelRenderComponent>();
//			}
//		}
//		else
//		{
//
//		}
//
//		ImGui::EndPopup();
//	}
//
//	ImGui::End();
//}
//
//
//void Looper::FileMenu()
//{
//	if (!ImGui::BeginMenu("File")) return;
//	//上書き保存
//	if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
//
//	}
//	//名前を付けて保存
//	if (ImGui::MenuItem("Save Scene As..", "Ctrl+Shift+S")) {
//		scene_stack_.top()->SeveSceneData();
//	}
//
//	ImGui::EndMenu();
//
//}
//
//void Looper::MainMenu()
//{
//	if (ImGui::BeginMainMenuBar()) 
//	{
//		FileMenu();
//
//		ImGui::EndMainMenuBar();
//	}
//}
//
//void Looper::FolderUI(std::weak_ptr<GameObject> gameObject, yEngine::u32 level)
//{
//	ImGui::PushID(gameObject.lock()->GetInstanceID());
//
//	// 非表示用チェックボックス
//	ImGui::Checkbox("##Blind", &gameObject.lock()->isBlind);
//	ImGui::SameLine();
//
//	bool node_open{};
//	ImGuiTreeNodeFlags flags =
//		level == 0 ?
//		ImGuiTreeNodeFlags_DefaultOpen : 0;
//	flags |= ImGuiTreeNodeFlags_OpenOnArrow;	// 矢印で開く
//	if (gameObject.lock()->GetInstanceID() == editor.selectID) flags |= ImGuiTreeNodeFlags_Selected;
//	
//	node_open = ImGui::TreeNodeEx(gameObject.lock()->name->c_str(), flags);
//
//	if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered()) {
//		editor.selectID = gameObject.lock()->GetInstanceID();
//	}
//	// 右クリックメニュー
//	if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered()) {
//		ImGui::OpenPopup("folder_context_menu");
//	}
//	if (ImGui::BeginPopup("folder_context_menu"))
//	{
//		// 新規作成
//		if (ImGui::Selectable("New folder"))
//		{
//		}
//		// 削除
//		if (ImGui::Selectable("Delete"))
//		{
//			Object::Destroy(gameObject.lock().get());
//		}
//		ImGui::EndPopup();
//	}
//
//	if (!node_open)
//	{
//		ImGui::PopID();
//		return;
//	}
//
//	// 子の表示
//	for (auto &child_object : gameObject.lock()->GetChildren())
//	{
//		FolderUI(child_object, level + 1);
//	}
//
//
//	ImGui::TreePop();
//	ImGui::PopID();
//
//}
//
//void Looper::FolderUI(std::weak_ptr<yEngine::EntityFolders::Folder> folder, yEngine::u32 level)
//{
//	ImGui::PushID(folder.lock()->id);
//
//	//// 非表示用チェックボックス
//	//ImGui::Checkbox("##Blind", &gameObject.lock()->isBlind);
//	//ImGui::SameLine();
//
//	bool node_open{};
//	ImGuiTreeNodeFlags flags =
//		level == 0 ?
//		ImGuiTreeNodeFlags_DefaultOpen : 0;
//	flags |= ImGuiTreeNodeFlags_OpenOnArrow;	// 矢印で開く
//	if (folder.lock()->id == editor.selectID) flags |= ImGuiTreeNodeFlags_Selected;
//
//	node_open = ImGui::TreeNodeEx(folder.lock()->name.c_str(), flags);
//
//	if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered()) {
//		editor.selectID = folder.lock()->id;
//	}
//	// 右クリックメニュー
//	if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered()) {
//		ImGui::OpenPopup("folder_context_menu");
//	}
//	if (ImGui::BeginPopup("folder_context_menu"))
//	{
//		// 新規作成
//		if (ImGui::Selectable("New folder"))
//		{
//			//scene_stack_.top()->object_folder_.CreateFolder(folder);
//		}
//		// 削除
//		if (ImGui::Selectable("Delete folder"))
//		{
//		}
//		ImGui::EndPopup();
//	}
//
//	if (!node_open)
//	{
//		ImGui::PopID();
//		return;
//	}
//
//	// 子フォルダ描画
//	std::weak_ptr<yEngine::EntityFolders::Folder> chiled_folder = folder.lock()->child_folder;
//	while (!chiled_folder.expired())
//	{
//		FolderUI(chiled_folder, level + 1);
//		chiled_folder = chiled_folder.lock()->next_folder;
//	}
//
//	// エンティティ描画
//	std::weak_ptr<yEngine::EntityFolders::Entity> entity = folder.lock()->first_entity;
//	while (!entity.expired())
//	{
//		EntitiyUI(entity, level + 1);
//		entity = entity.lock()->next;
//	}
//
//	ImGui::TreePop();
//	ImGui::PopID();
//
//}
//
//void Looper::EntitiyUI(std::weak_ptr<yEngine::EntityFolders::Entity> entity, yEngine::u32 level)
//{
//	std::weak_ptr<GameObject> game_object = std::dynamic_pointer_cast<GameObject>(entity.lock()->object.lock());
//	ImGui::PushID(game_object.lock()->GetInstanceID());
//	// 非表示用チェックボックス
//	ImGui::Checkbox("##Blind", &game_object.lock()->isBlind);
//	ImGui::SameLine();
//
//	bool node_open{};
//	ImGuiTreeNodeFlags flags =
//		level == 0 ?
//		ImGuiTreeNodeFlags_DefaultOpen : 0;
//	if (game_object.lock()->GetInstanceID() == editor.selectID) flags |= ImGuiTreeNodeFlags_Selected;
//
//	node_open = ImGui::TreeNodeEx(game_object.lock()->name->c_str(), flags);
//
//	if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered()) {
//		editor.selectID = game_object.lock()->GetInstanceID();
//	}
//	// 右クリックメニュー
//	if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered()) {
//		ImGui::OpenPopup("folder_context_menu");
//	}
//	if (ImGui::BeginPopup("folder_context_menu"))
//	{
//		// 新規作成
//		if (ImGui::Selectable("New object"))
//		{
//			scene_stack_.top()->object_folder_.CreateEntity(entity.lock()->pearent_folder, GameObject::AddGameObjet());
//		}
//		// 削除
//		if (ImGui::Selectable("Delete object"))
//		{
//		}
//		ImGui::EndPopup();
//	}
//
//	ImGui::TreePop();
//	ImGui::PopID();
//
//
//}

bool Looper::ImGuiInitialize()
{

	ID3D12Device *dev = DirectXCommon::GetInstance().Device;

	if (ImGui::CreateContext() == nullptr) {
		assert(0);
		return false;
	}

	auto &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// ドッキング有効化
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	// マルチウィンドウ有効化

	//ウィンドウズ用初期化
	if (!ImGui_ImplWin32_Init(Screen::GetInstance().Hwnd))
	{
		assert(0);
		return false;
	}

	// ヒープを設定
	auto imgui_cpu_handle = TextureManager::GetInstance().Heap.r_->GetCPUDescriptorHandleForHeapStart();
	imgui_cpu_handle.ptr +=
		dev->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) 
		* static_cast<int>(TextureManager::SystemHandle::kImGui);
	auto imgui_gpu_handle = TextureManager::GetInstance().Heap.r_->GetGPUDescriptorHandleForHeapStart();
	imgui_gpu_handle.ptr += 
		dev->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		* static_cast<int>(TextureManager::SystemHandle::kImGui);
	if (
		!ImGui_ImplDX12_Init(
			dev,	//DirectX12デバイス
			3,
			DXGI_FORMAT_R8G8B8A8_UNORM,	//書き込み先のRTVフォーマット
			TextureManager::GetInstance().Heap.r_.Get(),
			imgui_cpu_handle,	//CPUハンドル
			imgui_gpu_handle	//GPUハンドル
		))
	{
		assert(0);
		return false;
	}


	return true;
}

void Looper::ImGuiBeginFrame() const
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ImGuizmo::BeginFrame();
}

void Looper::ImGuiDraw()
{
	ID3D12GraphicsCommandList *cmd_list = DirectXCommon::GetInstance().CmdList;
	ImGui::Render();
	cmd_list->SetDescriptorHeaps(
		1, TextureManager::GetInstance().Heap.r_.GetAddressOf()
	);

	ImGui_ImplDX12_RenderDrawData(
		ImGui::GetDrawData(),
		cmd_list
	);
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
	
}

