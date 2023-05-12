#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

class EngineUI
{
public:
	void Draw();

private:
	// ���C�����j���[
	void MainMenu();

	// �t�@�C���֌W�̃v���_�E�����j���[
	void FileMenu();

	void WindowMenu();

	/// <summary>
	/// �I�u�W�F�N�g�̐e�q�֌W�\��
	/// </summary>
	void Hierarchy();

	/// <summary>
	/// �I�u�W�F�N�g�̏��\��
	/// </summary>
	void Inspector();

};

