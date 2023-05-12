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
	friend class Singleton<DirectXCommon>; // �C���X�^���X�쐬������

	/// <summary>
	/// �A�N�Z�b�T
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
	/// ���t���[���`��O����
	/// </summary>
	/// <returns></returns>
	bool ResourceBarrierWriting();


	/// <summary>
	/// �R�}���h���X�g�̎��s�ƏI������
	/// </summary>
	/// <returns></returns>
	bool PlayCommandList();
private:

	bool StartUp() override;

	/// <summary>
	/// �f�o�b�O���C���\�N��
	/// </summary>
	/// <returns></returns>
	bool DebugLayerON();

	/// <summary>
	/// DXGI�t�@�N�g���[�̐���
	/// </summary>
	/// <returns></returns>
	bool CreateDevice();

	/// <summary>
	/// �R�}���h�A���P�[�^�̐���
	/// </summary>
	/// <returns></returns>
	bool CreateCommand();

	/// <summary>
	/// �X���b�v�`�F�[���̐���
	/// </summary>
	/// <returns></returns>
	bool CreateSwapchainDesc();

	/// <summary>
	/// �����_�[�^�[�Q�b�g����
	/// </summary>
	bool CreateRenderTarget();

	/// <summary>
	/// �[�x�̐���
	/// </summary>
	/// <returns></returns>
	bool CreateDepthStencil();

	/// <summary>
	/// �t�F���X�̐���
	/// </summary>
	/// <returns></returns>
	bool CreateFence();

	Microsoft::WRL::ComPtr<ID3D12Device> device_;						// �f�o�C�X
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgi_factory_;			// �f�o�C�X�t�@�N�g���[
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmd_allocator_;	// �R�}���h�A���P�[�^�[
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list_;	// �R�}���h���X�g
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmd_queue_;			// �R�}���h�L���[
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain_;				// �X���b�v�`�F�[��
	const UINT kFrameCount = 2;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtv_heaps_;			// RTV�p�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc_;
	CD3DX12_CPU_DESCRIPTOR_HANDLE DirectXCommon::rtv_heap_handle_;		// RTV�p�n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> back_buffers_;
	Microsoft::WRL::ComPtr<ID3D12Resource> depth_buffer_;				// �[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsv_heap_;				// �[�x�p�q�[�v
	CD3DX12_CPU_DESCRIPTOR_HANDLE DirectXCommon::dsv_heap_handle_;		// �[�x�p�n���h��
	UINT back_buffer_index_;
	
	Microsoft::WRL::ComPtr<ID3D12Fence>DirectXCommon::fence;		// �t�F���X
	UINT fence_val_;

};
