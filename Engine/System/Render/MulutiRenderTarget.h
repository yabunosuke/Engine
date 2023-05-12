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
	
	static const int kNumMulutiRenderTarget{ 6 };	// レンダーの数

private:

	ID3D12Device *device_;						// デバイス取得用
	ID3D12GraphicsCommandList *cmd_list_;		// コマンドリスト取得用


	// RTVデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriputor_heap_RTV_;
	// DSVデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriputor_heap_DSV_;
	// 深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depth_buffer_;

	const float clear_color_[4]	{0.0f,0.0f,1.0f,1.0f};

};