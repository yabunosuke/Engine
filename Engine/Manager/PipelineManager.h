#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <map>
#include <string>

#include "Common/Property.h"
#include "Common/Singleton.h"

struct Pipeline
{
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipline_state{};	// �p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12RootSignature> root_signature{};// ���[�g�V�O�l�`��

};

class PipelineManager :
	public Singleton<PipelineManager>
{
public:
	friend class Singleton<PipelineManager>; // �C���X�^���X�쐬������

	void SetPipeline(const std::string &pipelineName);



private:
	PipelineManager();
	
	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	/// <param name="pipelineName"></param>
	/// <param name="graphicsPipelineState"></param>
	/// <param name="rootSignatureDesc"></param>
	bool CreatePipeline(
		const std::string &pipelineName, 
		D3D12_GRAPHICS_PIPELINE_STATE_DESC &graphicsPipelineState, 
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC &rootSignatureDesc);

	/// <summary>
	/// �p�C�v���C���X�e�[�g����
	/// </summary>
	/// <param name="blendDesc">�u�����h�ݒ�</param>
	/// <param name="inputLayout">���_���C�A�E�g</param>
	/// <param name="inputLayoutNum">���_���C�A�E�g�̐�</param>
	/// <param name="renderTargetViewFormats">�`��t�H�[�}�b�g</param>
	/// <param name="numRenderTarget">�g�p���郌���_�[�^�[�Q�b�g�̐�</param>
	/// <returns></returns>
	D3D12_GRAPHICS_PIPELINE_STATE_DESC CreateGraphicsPipelineState(
		const D3D12_BLEND_DESC &blendDesc,
		D3D12_INPUT_ELEMENT_DESC inputLayout[],
		int inputLayoutNum,
		DXGI_FORMAT renderTargetViewFormats[],
		int numRenderTarget
	);

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC CreateRootSignatureDesc(
		CD3DX12_ROOT_PARAMETER rootparams[],
		int numRootParams,
		CD3DX12_STATIC_SAMPLER_DESC samplerDesc
	);

	std::map<std::string, Pipeline> piplines_;	// �p�C�v���C���R���e�i

	ID3D12Device *device_;						// �f�o�C�X�擾�p
	ID3D12GraphicsCommandList *cmd_list_;		// �R�}���h���X�g�擾�p
};

