#include "MulutiRenderTarget.h"

#include "System/DirectXCommon.h"
#include "System/Screen.h"
#include "Manager/TextureManager.h"

MulutiRenderTarget::MulutiRenderTarget():
	device_(DirectXCommon::GetInstance().Device),
	cmd_list_(DirectXCommon::GetInstance().CmdList)
{
	HRESULT result = S_OK;

#pragma  region RTV
	// RTV
	D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc_RTV{};
	descriptor_heap_desc_RTV.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptor_heap_desc_RTV.NumDescriptors = kNumMulutiRenderTarget;	// �e�ʂ�6����
	result = device_->CreateDescriptorHeap(&descriptor_heap_desc_RTV, IID_PPV_ARGS(&descriputor_heap_RTV_));
	assert(SUCCEEDED((result)));

	for (int i = static_cast<int>(TextureManager::SystemHandle::kColor);
		i < static_cast<int>(TextureManager::SystemHandle::kScreen);
		++i)
	{
		device_->CreateRenderTargetView(
			TextureManager::GetInstance().GetTexture(
				static_cast<TextureManager::SystemHandle>(i))->GetResource(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descriputor_heap_RTV_->GetCPUDescriptorHandleForHeapStart(), i,
				device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))

		);

	}

	// �[�x�o�b�t�@
	CD3DX12_RESOURCE_DESC depth_resource_desc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			Screen::GetInstance().Width,
			static_cast<UINT>(Screen::GetInstance().Height),
			1, 0, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	// �e�N�X�`���o�b�t�@����
	result = device_->CreateCommittedResource(	//GPU���\�[�X�̐���
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depth_resource_desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//�e�N�X�`���p�w��
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depth_buffer_));
	assert(SUCCEEDED(result));
#pragma endregion  


#pragma region DSV
	// DSV
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descHeapDesc.NumDescriptors = 1;
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriputor_heap_DSV_));
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�q�[�v��DSV���쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; //�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depth_buffer_.Get(),
		&dsvDesc,
		descriputor_heap_DSV_->GetCPUDescriptorHandleForHeapStart());
#pragma endregion  
}

void MulutiRenderTarget::PreDraw()
{

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[kNumMulutiRenderTarget];

	for (int i = static_cast<int>(TextureManager::SystemHandle::kColor);
		i < static_cast<int>(TextureManager::SystemHandle::kScreen);
		++i)
	{
		// ���\�[�X�o���A�ύX
		cmd_list_->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				TextureManager::GetInstance().GetTexture(
					static_cast<TextureManager::SystemHandle>(i))->GetResource(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));

		//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descriputor_heap_RTV_->GetCPUDescriptorHandleForHeapStart(), i,
			device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descriputor_heap_DSV_->GetCPUDescriptorHandleForHeapStart();

	//�����_�[�^�[�Q�b�g���Z�b�g
	cmd_list_->OMSetRenderTargets(kNumMulutiRenderTarget, rtvHs, false, &dsvH);

	CD3DX12_VIEWPORT viewports[kNumMulutiRenderTarget];
	CD3DX12_RECT scissor_rects[kNumMulutiRenderTarget];

	for (int i = static_cast<int>(TextureManager::SystemHandle::kColor);
		i < static_cast<int>(TextureManager::SystemHandle::kScreen);
		++i)
	{
		viewports[i] = CD3DX12_VIEWPORT(
			0.0f, 0.0f, 
			static_cast<float>(Screen::GetInstance().Width), 
			static_cast<float>(Screen::GetInstance().Height));
		scissor_rects[i] = CD3DX12_RECT(0, 0, Screen::GetInstance().Width, Screen::GetInstance().Height);
	}

	//�r���[�|�[�g�̐ݒ�
	cmd_list_->RSSetViewports(kNumMulutiRenderTarget, viewports);
	//�V�U�����O��`�̐ݒ�
	cmd_list_->RSSetScissorRects(kNumMulutiRenderTarget, scissor_rects);


	for (int i = static_cast<int>(TextureManager::SystemHandle::kColor);
		i < static_cast<int>(TextureManager::SystemHandle::kScreen);
		++i)
	{
		//�S��ʃN���A
		cmd_list_->ClearRenderTargetView(rtvHs[i], clear_color_, 0, nullptr);
	}

	//�[�x�o�b�t�@�̃N���A
	cmd_list_->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void MulutiRenderTarget::PostDraw()
{

	// ���\�[�X�o���A�ύX
	for (int i = static_cast<int>(TextureManager::SystemHandle::kColor);
		i < static_cast<int>(TextureManager::SystemHandle::kScreen);
		++i)
	{
		cmd_list_->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				TextureManager::GetInstance().GetTexture(
					static_cast<TextureManager::SystemHandle>(i))->GetResource(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}
