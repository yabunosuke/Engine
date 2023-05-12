#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")


class MulutiRenderTarget final
{
public:

	MulutiRenderTarget();

	void PreDraw();
	void PostDraw();
	
	static const int kNumMulutiRenderTarget{ 6 };	// �����_�[�̐�

private:

	ID3D12Device *device_;						// �f�o�C�X�擾�p
	ID3D12GraphicsCommandList *cmd_list_;		// �R�}���h���X�g�擾�p


	// RTV�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriputor_heap_RTV_;
	// DSV�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriputor_heap_DSV_;
	// �[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> depth_buffer_;

	const float clear_color_[4]	{0.0f,0.0f,1.0f,1.0f};

};