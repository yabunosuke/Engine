#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>

#include "Object.h"
#include "Common/Property.h"

class Texture final:
    public Object
{
    friend class TextureManager;
public:

    yEngine::Property<int> Width			// ����
    {
        nullptr,
        [&]() -> int {return resource_->GetDesc().Width; }
    };

    yEngine::Property<int> Height			// ����
    {
        nullptr,
        [&]() -> int {return resource_->GetDesc().Height; }
    };

    ID3D12Resource *GetResource()
    {
        return resource_.Get();
    }

    CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUHandle()
    {
        return gpu_desc_handle_srv_;
    }
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUHandle()
    {
        return cpu_desc_handle_srv_;
    }

private:



    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle_srv_;  // CPU�n���h��
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle_srv_;  // GPU�n���h��
    std::wstring file_path_;                            // �t�@�C���p�X

    float mipmap_bias_; // �~�b�v�}�b�v�̃o�C�A�X
    int mipmap_count_;  // �~�b�v�}�b�v���x��

    uint32_t handle_;   // �n���h��


};
