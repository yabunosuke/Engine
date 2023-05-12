#include "EngineUI.h"

void EngineUI::Draw()
{
	MainMenu();
	Hierarchy();
	Inspector();
}

void EngineUI::MainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		FileMenu();

		ImGui::EndMainMenuBar();
	}
}

void EngineUI::FileMenu()
{
	if (!ImGui::BeginMenu("File")) return;


	// シーン
	if (ImGui::MenuItem("Scene", "N")) {

	}

	//上書き保存
	if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {

	}
	//名前を付けて保存
	if (ImGui::MenuItem("Save Scene As..", "Ctrl+Shift+S")) {

	}

	ImGui::EndMenu();

}

void EngineUI::WindowMenu()
{
	if (!ImGui::BeginMenu("Window")) return;


	ImGui::EndMenu();

}

void EngineUI::Hierarchy()
{
	ImGui::Begin("Hierarchy");

	ImGui::End();
	
}

void EngineUI::Inspector()
{
	ImGui::Begin("Inspector");

	ImGui::End();

}
