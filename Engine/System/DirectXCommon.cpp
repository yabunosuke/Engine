#include "DirectXCommon.h"

#include <cassert>
#include "Screen.h"



bool DirectXCommon::ResourceBarrierWriting()
{
    //�����_�[�^�[�Q�b�g���������݉\��
    back_buffer_index_ = swapchain_->GetCurrentBackBufferIndex();
    cmd_list_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(back_buffers_[back_buffer_index_].Get(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    //�����_�[�^�[�Q�b�g�ݒ�
    rtv_heap_handle_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtv_heaps_->GetCPUDescriptorHandleForHeapStart(),
        back_buffer_index_,
        device_->GetDescriptorHandleIncrementSize(rtv_heap_desc_.Type)
    );
    //�[�x�X�e���V���r���[
    dsv_heap_handle_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsv_heap_->GetCPUDescriptorHandleForHeapStart());
    //�����_�[�^�[�Q�b�g�Z�b�g
    cmd_list_->OMSetRenderTargets(1, &rtv_heap_handle_, false, &dsv_heap_handle_);


    //�r���[�|�[�g�̐ݒ�
    cmd_list_->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(Screen::GetInstance().Width), static_cast<float>(Screen::GetInstance().Height)));
    //�V�U�[��`�̐ݒ�
    cmd_list_->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, static_cast<LONG>(Screen::GetInstance().Width), static_cast<LONG>(Screen::GetInstance().Height)));

    // �X�N���[���N���A
    float color[] = { 0.0f,0.0f,1.0f,0.0f };
    cmd_list_->ClearRenderTargetView(rtv_heap_handle_, color, 0, nullptr);
    cmd_list_->ClearDepthStencilView(dsv_heap_handle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


    return true;
}

bool DirectXCommon::PlayCommandList()
{
    //���\�[�X�o���A�ύX
    cmd_list_->ResourceBarrier(1,
        &CD3DX12_RESOURCE_BARRIER::Transition(back_buffers_[back_buffer_index_].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT)
    );

    // ���߂̃N���[�Y
    cmd_list_->Close();

    // �R�}���h���X�g�̎��s
    ID3D12CommandList *cmd_lists[] = { cmd_list_.Get() }; // �R�}���h���X�g�̔z��
    cmd_queue_->ExecuteCommandLists(1, cmd_lists);
    
    // �R�}���h���X�g�̎��s������҂�
    cmd_queue_->Signal(fence.Get(), ++fence_val_);
    if (fence->GetCompletedValue() != fence_val_) {
        HANDLE event = CreateEvent(nullptr, false, false, nullptr);
        if (event) {
            fence->SetEventOnCompletion(fence_val_, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
    }

    cmd_allocator_->Reset(); // �L���[���N���A
    cmd_list_->Reset(cmd_allocator_.Get(), nullptr);  // �ĂуR�}���h���X�g�𒙂߂鏀��

    // �o�b�t�@���t���b�v�i���\�̓��ւ��j
    swapchain_->Present(1, 0);

    return true;
}

bool DirectXCommon::StartUp()
{
    DebugLayerON();         // �f�o�b�O���C���[���I��
    CreateDevice();         // �O���t�B�b�N�A�_�v�^�[�̑I���E����
    CreateCommand();        // �R�}���h�n����
    CreateSwapchainDesc();  // �e��ݒ�����ăX���b�v�`�F�[���𐶐�
    CreateRenderTarget();   // �����_�[�^�[�Q�b�g����
    CreateDepthStencil();   // �[�x����
    CreateFence();          // �t�F���X����

    return true;
}

bool DirectXCommon::DebugLayerON()
{
#ifdef _DEBUG
    //�f�o�b�O���C���[���I����
    Microsoft::WRL::ComPtr<ID3D12Debug> debug_controller;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
    {
        debug_controller->EnableDebugLayer();
    }
#endif


    return true;
}

bool DirectXCommon::CreateDevice()
{
    HRESULT result;
    result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factory_));
    Microsoft::WRL::ComPtr<IDXGIAdapter1> tmp_adapter;
    assert(SUCCEEDED(result));

    // �A�_�v�^�[�̗񋓗p
    std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter1>> adapters;
    // �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
    for (int i = 0;
        dxgi_factory_->EnumAdapters1(i, &tmp_adapter) != DXGI_ERROR_NOT_FOUND; i++)
    {
        adapters.push_back(tmp_adapter); // ���I�z��ɒǉ�����
    }

    for (int i = 0; i < adapters.size(); i++)
    {
        DXGI_ADAPTER_DESC1 adesc;
        adapters[i]->GetDesc1(&adesc);  // �A�_�v�^�[�̏����擾

        // �\�t�g�E�F�A�f�o�C�X�����
        if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        std::wstring strDesc = adesc.Description;   // �A�_�v�^�[��
        // Intel UHD Graphics�i�I���{�[�h�O���t�B�b�N�j�����
        if (strDesc.find(L"Intel") == std::wstring::npos)
        {
            tmp_adapter = adapters[i];   // �̗p
            break;
        }
    }

    // �Ή����x���̔z��
    D3D_FEATURE_LEVEL levels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    D3D_FEATURE_LEVEL featureLevel;

    for (int i = 0; i < _countof(levels); i++)
    {
        // �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
        result = D3D12CreateDevice(tmp_adapter.Get(), levels[i], IID_PPV_ARGS(&device_));
        if (result == S_OK)
        {
            // �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
            featureLevel = levels[i];
            break;
        }
    }
    
    return true;
}

bool DirectXCommon::CreateCommand()
{
    HRESULT result;

    // �R�}���h�A���P�[�^�𐶐�
    result = device_->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&cmd_allocator_));
    assert(SUCCEEDED(result));
    cmd_allocator_->SetName(L"Command Allocator");


    // �R�}���h���X�g�𐶐�
    result = device_->CreateCommandList(
        0,D3D12_COMMAND_LIST_TYPE_DIRECT,
        cmd_allocator_.Get(), nullptr,
        IID_PPV_ARGS(&cmd_list_));
    assert(SUCCEEDED(result));
    cmd_list_->SetName(L"Command List");

    // �R�}���h�L���[����
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
    device_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmd_queue_));
    cmd_queue_->SetName(L"Command Queue");


    return true;

}

bool DirectXCommon::CreateSwapchainDesc()
{
    DXGI_SWAP_CHAIN_DESC1 swapchain_desc{};
    swapchain_desc.Width = Screen::GetInstance().Width;
    swapchain_desc.Height = Screen::GetInstance().Height;
    swapchain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;         // �F���̏���
    swapchain_desc.SampleDesc.Count = 1;                        // �}���`�T���v�����Ȃ�
    swapchain_desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;        // �o�b�N�o�b�t�@�p
    swapchain_desc.BufferCount = 2;                             // �o�b�t�@�����Q�ɐݒ�
    swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;  // �t���b�v��͔j��
    swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1;
    dxgi_factory_->CreateSwapChainForHwnd(
        cmd_queue_.Get(),
        Screen::GetInstance().Hwnd,
        &swapchain_desc,
        nullptr,
        nullptr,
        &swapchain1);
    swapchain1.As(&swapchain_);

    return true;
}

bool DirectXCommon::CreateRenderTarget()
{
    HRESULT result;

    rtv_heap_desc_.NumDescriptors = kFrameCount;             // ���\�̂Q��
    rtv_heap_desc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;    // �����_�[�^�[�Q�b�g�r���[
    result = device_->CreateDescriptorHeap(
        &rtv_heap_desc_,IID_PPV_ARGS(&rtv_heaps_));
    assert(SUCCEEDED(result));
    rtv_heaps_->SetName(L"RTV Heap");


    back_buffers_.resize(kFrameCount);
    for (int i = 0; i < 2; i++)
    {
        // �X���b�v�`�F�[������o�b�t�@���擾
        result = swapchain_->GetBuffer(i, IID_PPV_ARGS(&back_buffers_[i]));
        assert(SUCCEEDED(result));

        // �f�X�N���v�^�q�[�v�̃n���h�����擾
        CD3DX12_CPU_DESCRIPTOR_HANDLE handle =
            CD3DX12_CPU_DESCRIPTOR_HANDLE(rtv_heaps_->GetCPUDescriptorHandleForHeapStart(),
                i,  //�f�B�X�N���v�^�̔ԍ�
                DirectXCommon::device_->GetDescriptorHandleIncrementSize(rtv_heap_desc_.Type)//�f�B�X�N���v�^1���̃T�C�Y
            );
        // �����_�[�^�[�Q�b�g�r���[�̐���
        DirectXCommon::device_->CreateRenderTargetView(
            back_buffers_[i].Get(),
            nullptr,
            handle);
    }

    return true;
}

bool DirectXCommon::CreateDepthStencil()
{
    HRESULT result;

    //���\�[�X�ݒ�
    CD3DX12_RESOURCE_DESC depth_resdesc =
        CD3DX12_RESOURCE_DESC::Tex2D(
            DXGI_FORMAT_D32_FLOAT,
            Screen::GetInstance().Width,
            Screen::GetInstance().Height,
            1, 0,
            1, 0,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
        );

    //���\�[�X����
    result = DirectXCommon::device_->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depth_resdesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
        IID_PPV_ARGS(&depth_buffer_)
    );
    assert(SUCCEEDED(result));
    depth_buffer_->SetName(L"De@th Buffer");



    // �r���[����
    D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc = {};
    D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};

    dsv_heap_desc.NumDescriptors = 1;	//�[�x�r���[
    dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    result = DirectXCommon::device_->CreateDescriptorHeap(
        &dsv_heap_desc,
        IID_PPV_ARGS(&dsv_heap_));
    assert(SUCCEEDED(result));

    dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
    dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    DirectXCommon::device_->CreateDepthStencilView(
        depth_buffer_.Get(), &dsv_desc,
        dsv_heap_->GetCPUDescriptorHandleForHeapStart()
    );
    return true;

}

bool DirectXCommon::CreateFence()
{
    HRESULT
    result = DirectXCommon::device_->CreateFence(fence_val_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    assert(SUCCEEDED(result));

    return true;
    
}
