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

    yEngine::Property<int> Width			// 横幅
    {
        nullptr,
        [&]() -> int {return resource_->GetDesc().Width; }
    };

    yEngine::Property<int> Height			// 横幅
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
    CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle_srv_;  // CPUハンドル
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle_srv_;  // GPUハンドル
    std::wstring file_path_;                            // ファイルパス

    float mipmap_bias_; // ミップマップのバイアス
    int mipmap_count_;  // ミップマップレベル

    uint32_t handle_;   // ハンドル


};
