#pragma once

#include <array>

#include "Common/Singleton.h"
#include "Object/Texture.h"


class TextureManager final
    : public Singleton<TextureManager>
{
public:
    friend class Singleton<TextureManager>; // �C���X�^���X�쐬������

    enum class SystemHandle
    {
        kColor,
        kNormal,
        kMetalRoughuness,
        kDepth,
        kWorldPosition,
        kObjectID,
        kScreen,            // �ŏI�o�͉��
        kImGui,
        
        kTextures
    };


    /// <summary>
    /// �e�N�X�`�����[�h
    /// </summary>
    /// <param name="filePath">�ǂݍ��ރt�@�C���p�X</param>
    /// <returns></returns>
    uint32_t LoadTexture(const wchar_t *filePath);

    /// <summary>
    /// ���C�N�e�N�X�`��
    /// </summary>
    /// <param name="color">RGBA</param>
    /// <param name="size">�c���Ɖ���</param>
    /// <returns></returns>
    uint32_t MakeTexture(DWORD color = 0xffffffff, DirectX::XMFLOAT2 size = { 8,8 });

    
    /// <summary>
    /// �t�@�C���p�X����e�N�X�`���f�[�^���擾����B
    /// </summary>
    /// <param name="filepath"></param>
    /// <returns></returns>
    Texture *GetTexture(const std::wstring &filepath);

    Texture *GetTexture(uint32_t handle);

    Texture *GetTexture(SystemHandle handleName);



    static const int kMaxDescripotrSize{ 512 };    // �f�X�N���v�^�̐�

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
    /// �f�X�N���v�^����
    /// </summary>
    bool CreateDescriptorHeap();

    UINT descriptor_size_;                                          // �f�X�N���v�^�T�C�Y
    /// <summary>
    /// �f�X�N���v�^�q�[�v
    /// </summary>
    /// <param name="0">�ǂݍ��ރt�@�C���p�X</param>
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
    std::array<Texture, kMaxDescripotrSize> textures_;              // �e�N�X�`���f�[�^


};

