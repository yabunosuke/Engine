#pragma once

#include <array>

#include "Common/Singleton.h"
#include "Object/Texture.h"


class TextureManager final
    : public Singleton<TextureManager>
{
public:
    friend class Singleton<TextureManager>; // インスタンス作成を許可

    enum class SystemHandle
    {
        kColor,
        kNormal,
        kMetalRoughuness,
        kDepth,
        kWorldPosition,
        kObjectID,
        kScreen,            // 最終出力画面
        kImGui,
        
        kTextures
    };


    /// <summary>
    /// テクスチャロード
    /// </summary>
    /// <param name="filePath">読み込むファイルパス</param>
    /// <returns></returns>
    uint32_t LoadTexture(const wchar_t *filePath);

    /// <summary>
    /// メイクテクスチャ
    /// </summary>
    /// <param name="color">RGBA</param>
    /// <param name="size">縦幅と横幅</param>
    /// <returns></returns>
    uint32_t MakeTexture(DWORD color = 0xffffffff, DirectX::XMFLOAT2 size = { 8,8 });

    
    /// <summary>
    /// ファイルパスからテクスチャデータを取得する。
    /// </summary>
    /// <param name="filepath"></param>
    /// <returns></returns>
    Texture *GetTexture(const std::wstring &filepath);

    Texture *GetTexture(uint32_t handle);

    Texture *GetTexture(SystemHandle handleName);



    static const int kMaxDescripotrSize{ 512 };    // デスクリプタの数

	yEngine::ReadOnlyProperty<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> Heap
    {
        &descriptor_heap_
    };

private:

    TextureManager() :
        descriptor_size_(0)
    {};

    bool StartUp() override;

    /// <summary>
    /// デスクリプタ生成
    /// </summary>
    bool CreateDescriptorHeap();

    UINT descriptor_size_;                                          // デスクリプタサイズ
    /// <summary>
    /// デスクリプタヒープ
    /// </summary>
    /// <param name="0">読み込むファイルパス</param>
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
    std::array<Texture, kMaxDescripotrSize> textures_;              // テクスチャデータ


};

