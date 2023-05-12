#include "TextureManager.h"

#include <DirectXTex.h>

#include "System/DirectXCommon.h"
#include "System/Screen.h"

bool TextureManager::StartUp()
{
	CreateDescriptorHeap();

	HRESULT result = S_FALSE;
	ID3D12Device *dev = DirectXCommon::GetInstance().Device;

	// �}���`�����_�[�p�̉摜����
	DXGI_FORMAT mulutirender_format_list_[static_cast<int>(SystemHandle::kScreen)]
	{
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		//DXGI_FORMAT_R32G32B32A32_FLOAT,		// skybox
	};

	float clear_color[4]{ 0.0f,0.0f,1.0f,1.0f };

	for (int i = static_cast<int>(SystemHandle::kColor);
		i != static_cast<int>(SystemHandle::kScreen);
		++i)
	{
		// ���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC texres_desc =
			CD3DX12_RESOURCE_DESC::Tex2D(
				mulutirender_format_list_[i],
				Screen::GetInstance().Width,
				static_cast<UINT>(Screen::GetInstance().Height),
				1, 0, 1, 0,
				D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		// �e�N�X�`���o�b�t�@����
		result = dev->CreateCommittedResource(	//GPU���\�[�X�̐���
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texres_desc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,	//�e�N�X�`���p�w��
			&CD3DX12_CLEAR_VALUE(mulutirender_format_list_[i], clear_color),
			IID_PPV_ARGS(&textures_[i].resource_));
		assert(SUCCEEDED(result));

		// �h��Ԃ����摜�𐶐�
		size_t texels = Screen::GetInstance().Width * Screen::GetInstance().Height;
		UINT *img = new UINT[texels];
		for (size_t j = 0; j < texels; ++j) {
			img[j] = 0xffffffff;	// ���F
		}

		// �e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
		result = textures_[i].resource_->WriteToSubresource(
			0, nullptr,
			img, Screen::GetInstance().Width, Screen::GetInstance().Height
		);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	return true;
}

uint32_t TextureManager::LoadTexture(const wchar_t *filename)
{
	int32_t handle{ -1 };

	// �摜�����ɑ��݂��邩�`�F�b�N
	for(uint32_t index = static_cast<uint32_t>(SystemHandle::kTextures); index < textures_.size(); ++index)
	{
		if (textures_[index].file_path_ == filename)
		{
			// �摜�����݂��Ă���΃��[�h�����Ƀn���h����Ԃ�
			handle = index;
			return handle;
		}
		else if( handle == -1 && textures_[index].file_path_ == L"")
		{
			// �󂫂�����΃n���h����ۑ�����
			handle = index;
		}
	}
	if (handle == -1)
	{
		assert(0);

	}
	
	HRESULT result = S_FALSE;
	ID3D12Device *dev = DirectXCommon::GetInstance().Device;

	// �����J�n
	Texture &texture = textures_.at(handle);
	texture.file_path_ = filename;
	texture.handle_ = handle;


	//�e�N�X�`���̃��[�h
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);
	assert(SUCCEEDED(result));

	const DirectX::Image *img = scratchImg.GetImage(0, 0, 0);

	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texres_desc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels);

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heap_properties =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);



	// �e�N�X�`���o�b�t�@����
	result = dev->CreateCommittedResource(	//GPU���\�[�X�̐���
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&texres_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,	//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texture.resource_));
	assert(SUCCEEDED(result));

	// �e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	result = texture.resource_.Get()->WriteToSubresource(
		0,
		nullptr,	//�S�̈�փR�s�[
		img->pixels,	//���f�[�^�A�h���X
		(UINT)img->rowPitch,	//1���C���T�C�Y
		(UINT)img->slicePitch	//�S�T�C�Y
	);
	assert(SUCCEEDED(result));


	//�V�F�[�_���\�[�X�r���[�ݒ�

	//D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};	//�ݒ�\����
	//srv_desc.Format = metadata.format;	//RGBA
	//srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	//srv_desc.Texture1D.MipLevels = 1;


	texture.cpu_desc_handle_srv_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), handle, descriptor_size_);
	texture.gpu_desc_handle_srv_ = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		descriptor_heap_->GetGPUDescriptorHandleForHeapStart(), handle, descriptor_size_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};	//�ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texture.resource_->GetDesc();
	srv_desc.Format = resDesc.Format;	//RGBA
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srv_desc.Texture1D.MipLevels = 1;

	//�V�F�[�_�[���\�[�X�r���[�쐬

	//dev->CreateShaderResourceView(
	//	texture_buffer_.back().Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
	//	&srv_desc,	//�e�N�X�`���ݒ���
	//	CD3DX12_CPU_DESCRIPTOR_HANDLE(
	//		descriptor_heap_->GetCPUDescriptorHandleForHeapStart(),
	//		texture_buffer_.size()-1,
	//		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	//);

	dev->CreateShaderResourceView(
		texture.resource_.Get(),
		&srv_desc,
		texture.cpu_desc_handle_srv_
	);

	
	return handle;
}

Texture *TextureManager::GetTexture(const std::wstring &filepath)
{
	if (filepath == L"") return nullptr;
	for (auto &tex : textures_)
	{
		if (tex.file_path_ == filepath)
		{
			return &tex;
		}
	}
	assert(0);
	return nullptr;
}

Texture *TextureManager::GetTexture(uint32_t handle)
{
	if (handle < 0 || kMaxDescripotrSize < handle) return nullptr;
	for (auto &tex : textures_)
	{
		if (tex.handle_ == handle)
		{
			return &tex;
		}
	}
	assert(0);
	return nullptr;
}

Texture *TextureManager::GetTexture(TextureManager::SystemHandle handleName)
{
	uint32_t handle = static_cast<uint32_t>(handleName);

	return &textures_[handle];
}


uint32_t MakeTexture(DWORD color, DirectX::XMFLOAT2 size)
{
	return 1;

}

bool TextureManager::CreateDescriptorHeap()
{
	HRESULT result;
	ID3D12Device *dev = DirectXCommon::GetInstance().Device;
	// �f�X�N���v�^�̃T�C�Y���擾
	descriptor_size_ =
		dev->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�f�X�N���v�^�q�[�v�̐���
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = kMaxDescripotrSize;
	result = dev->
		CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptor_heap_));
	assert(SUCCEEDED(result));
	descriptor_heap_->SetName(L"SRV Descriptor Heap");


	// �e�N�X�`��������
	for (size_t i = 0; i < kMaxDescripotrSize; i++) {
		textures_[i].resource_.Reset();
		textures_[i].cpu_desc_handle_srv_.ptr = 0;
		textures_[i].gpu_desc_handle_srv_.ptr = 0;
		textures_[i].file_path_.clear();
	}


	return true;
}
