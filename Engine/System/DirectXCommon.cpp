#include "DirectXCommon.h"

#include <cassert>
#include "Screen.h"



bool DirectXCommon::ResourceBarrierWriting()
{
    //レンダーターゲットを書き込み可能に
    back_buffer_index_ = swapchain_->GetCurrentBackBufferIndex();
    cmd_list_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(back_buffers_[back_buffer_index_].Get(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    //レンダーターゲット設定
    rtv_heap_handle_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtv_heaps_->GetCPUDescriptorHandleForHeapStart(),
        back_buffer_index_,
        device_->GetDescriptorHandleIncrementSize(rtv_heap_desc_.Type)
    );
    //深度ステンシルビュー
    dsv_heap_handle_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsv_heap_->GetCPUDescriptorHandleForHeapStart());
    //レンダーターゲットセット
    cmd_list_->OMSetRenderTargets(1, &rtv_heap_handle_, false, &dsv_heap_handle_);


    //ビューポートの設定
    cmd_list_->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(Screen::GetInstance().Width), static_cast<float>(Screen::GetInstance().Height)));
    //シザー矩形の設定
    cmd_list_->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, static_cast<LONG>(Screen::GetInstance().Width), static_cast<LONG>(Screen::GetInstance().Height)));

    // スクリーンクリア
    float color[] = { 0.0f,0.0f,1.0f,0.0f };
    cmd_list_->ClearRenderTargetView(rtv_heap_handle_, color, 0, nullptr);
    cmd_list_->ClearDepthStencilView(dsv_heap_handle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


    return true;
}

bool DirectXCommon::PlayCommandList()
{
    //リソースバリア変更
    cmd_list_->ResourceBarrier(1,
        &CD3DX12_RESOURCE_BARRIER::Transition(back_buffers_[back_buffer_index_].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT)
    );

    // 命令のクローズ
    cmd_list_->Close();

    // コマンドリストの実行
    ID3D12CommandList *cmd_lists[] = { cmd_list_.Get() }; // コマンドリストの配列
    cmd_queue_->ExecuteCommandLists(1, cmd_lists);
    
    // コマンドリストの実行完了を待つ
    cmd_queue_->Signal(fence.Get(), ++fence_val_);
    if (fence->GetCompletedValue() != fence_val_) {
        HANDLE event = CreateEvent(nullptr, false, false, nullptr);
        if (event) {
            fence->SetEventOnCompletion(fence_val_, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
    }

    cmd_allocator_->Reset(); // キューをクリア
    cmd_list_->Reset(cmd_allocator_.Get(), nullptr);  // 再びコマンドリストを貯める準備

    // バッファをフリップ（裏表の入替え）
    swapchain_->Present(1, 0);

    return true;
}

bool DirectXCommon::StartUp()
{
    DebugLayerON();         // デバッグレイヤーをオン
    CreateDevice();         // グラフィックアダプターの選択・生成
    CreateCommand();        // コマンド系生成
    CreateSwapchainDesc();  // 各種設定をしてスワップチェーンを生成
    CreateRenderTarget();   // レンダーターゲット生成
    CreateDepthStencil();   // 深度生成
    CreateFence();          // フェンス生成

    return true;
}

bool DirectXCommon::DebugLayerON()
{
#ifdef _DEBUG
    //デバッグレイヤーをオンに
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

    // アダプターの列挙用
    std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter1>> adapters;
    // ここに特定の名前を持つアダプターオブジェクトが入る
    for (int i = 0;
        dxgi_factory_->EnumAdapters1(i, &tmp_adapter) != DXGI_ERROR_NOT_FOUND; i++)
    {
        adapters.push_back(tmp_adapter); // 動的配列に追加する
    }

    for (int i = 0; i < adapters.size(); i++)
    {
        DXGI_ADAPTER_DESC1 adesc;
        adapters[i]->GetDesc1(&adesc);  // アダプターの情報を取得

        // ソフトウェアデバイスを回避
        if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        std::wstring strDesc = adesc.Description;   // アダプター名
        // Intel UHD Graphics（オンボードグラフィック）を回避
        if (strDesc.find(L"Intel") == std::wstring::npos)
        {
            tmp_adapter = adapters[i];   // 採用
            break;
        }
    }

    // 対応レベルの配列
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
        // 採用したアダプターでデバイスを生成
        result = D3D12CreateDevice(tmp_adapter.Get(), levels[i], IID_PPV_ARGS(&device_));
        if (result == S_OK)
        {
            // デバイスを生成できた時点でループを抜ける
            featureLevel = levels[i];
            break;
        }
    }
    
    return true;
}

bool DirectXCommon::CreateCommand()
{
    HRESULT result;

    // コマンドアロケータを生成
    result = device_->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&cmd_allocator_));
    assert(SUCCEEDED(result));
    cmd_allocator_->SetName(L"Command Allocator");


    // コマンドリストを生成
    result = device_->CreateCommandList(
        0,D3D12_COMMAND_LIST_TYPE_DIRECT,
        cmd_allocator_.Get(), nullptr,
        IID_PPV_ARGS(&cmd_list_));
    assert(SUCCEEDED(result));
    cmd_list_->SetName(L"Command List");

    // コマンドキュー生成
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
    swapchain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;         // 色情報の書式
    swapchain_desc.SampleDesc.Count = 1;                        // マルチサンプルしない
    swapchain_desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;        // バックバッファ用
    swapchain_desc.BufferCount = 2;                             // バッファ数を２つに設定
    swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;  // フリップ後は破棄
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

    rtv_heap_desc_.NumDescriptors = kFrameCount;             // 裏表の２つ
    rtv_heap_desc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;    // レンダーターゲットビュー
    result = device_->CreateDescriptorHeap(
        &rtv_heap_desc_,IID_PPV_ARGS(&rtv_heaps_));
    assert(SUCCEEDED(result));
    rtv_heaps_->SetName(L"RTV Heap");


    back_buffers_.resize(kFrameCount);
    for (int i = 0; i < 2; i++)
    {
        // スワップチェーンからバッファを取得
        result = swapchain_->GetBuffer(i, IID_PPV_ARGS(&back_buffers_[i]));
        assert(SUCCEEDED(result));

        // デスクリプタヒープのハンドルを取得
        CD3DX12_CPU_DESCRIPTOR_HANDLE handle =
            CD3DX12_CPU_DESCRIPTOR_HANDLE(rtv_heaps_->GetCPUDescriptorHandleForHeapStart(),
                i,  //ディスクリプタの番号
                DirectXCommon::device_->GetDescriptorHandleIncrementSize(rtv_heap_desc_.Type)//ディスクリプタ1つ分のサイズ
            );
        // レンダーターゲットビューの生成
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

    //リソース設定
    CD3DX12_RESOURCE_DESC depth_resdesc =
        CD3DX12_RESOURCE_DESC::Tex2D(
            DXGI_FORMAT_D32_FLOAT,
            Screen::GetInstance().Width,
            Screen::GetInstance().Height,
            1, 0,
            1, 0,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
        );

    //リソース生成
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



    // ビュー生成
    D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc = {};
    D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};

    dsv_heap_desc.NumDescriptors = 1;	//深度ビュー
    dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    result = DirectXCommon::device_->CreateDescriptorHeap(
        &dsv_heap_desc,
        IID_PPV_ARGS(&dsv_heap_));
    assert(SUCCEEDED(result));

    dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
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
