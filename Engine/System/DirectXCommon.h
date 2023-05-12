#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

#include <vector>
#include <string>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

//ComPtr
#include <wrl.h>

#include "Common/Singleton.h"
#include "Common/Property.h"

class DirectXCommon final :
	public Singleton<DirectXCommon>
{
public:
	friend class Singleton<DirectXCommon>; // インスタンス作成を許可

	/// <summary>
	/// アクセッサ
	/// </summary>
	yEngine::ReadOnlyProperty<ID3D12Device*> Device
	{
		nullptr,
		[&]() -> ID3D12Device *{ return device_.Get(); }
	};
	yEngine::Property<ID3D12GraphicsCommandList*> CmdList
	{
		nullptr,
		[&]() -> ID3D12GraphicsCommandList *{ return cmd_list_.Get(); }
	};

	yEngine::Property<ID3D12DescriptorHeap *> DescriputorHeapRTV
	{
		nullptr,
		[&]() -> ID3D12DescriptorHeap *{ return rtv_heaps_.Get(); }
	};


	yEngine::Property<ID3D12DescriptorHeap *> DescriputorHeapDSV
	{
		nullptr,
		[&]() -> ID3D12DescriptorHeap *{ return dsv_heap_.Get(); }
	};

	

	/// <summary>
	/// 毎フレーム描画前処理
	/// </summary>
	/// <returns></returns>
	bool ResourceBarrierWriting();


	/// <summary>
	/// コマンドリストの実行と終了処理
	/// </summary>
	/// <returns></returns>
	bool PlayCommandList();
private:

	bool StartUp() override;

	/// <summary>
	/// デバッグレイヤ―起動
	/// </summary>
	/// <returns></returns>
	bool DebugLayerON();

	/// <summary>
	/// DXGIファクトリーの生成
	/// </summary>
	/// <returns></returns>
	bool CreateDevice();

	/// <summary>
	/// コマンドアロケータの生成
	/// </summary>
	/// <returns></returns>
	bool CreateCommand();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	/// <returns></returns>
	bool CreateSwapchainDesc();

	/// <summary>
	/// レンダーターゲット生成
	/// </summary>
	bool CreateRenderTarget();

	/// <summary>
	/// 深度の生成
	/// </summary>
	/// <returns></returns>
	bool CreateDepthStencil();

	/// <summary>
	/// フェンスの生成
	/// </summary>
	/// <returns></returns>
	bool CreateFence();

	Microsoft::WRL::ComPtr<ID3D12Device> device_;						// デバイス
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgi_factory_;			// デバイスファクトリー
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmd_allocator_;	// コマンドアロケーター
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list_;	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmd_queue_;			// コマンドキュー
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain_;				// スワップチェーン
	const UINT kFrameCount = 2;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtv_heaps_;			// RTV用ヒープ
	D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc_;
	CD3DX12_CPU_DESCRIPTOR_HANDLE DirectXCommon::rtv_heap_handle_;		// RTV用ハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> back_buffers_;
	Microsoft::WRL::ComPtr<ID3D12Resource> depth_buffer_;				// 深度バッファ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsv_heap_;				// 深度用ヒープ
	CD3DX12_CPU_DESCRIPTOR_HANDLE DirectXCommon::dsv_heap_handle_;		// 深度用ハンドル
	UINT back_buffer_index_;
	
	Microsoft::WRL::ComPtr<ID3D12Fence>DirectXCommon::fence;		// フェンス
	UINT fence_val_;

};
