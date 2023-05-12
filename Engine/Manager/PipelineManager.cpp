#include "PipelineManager.h"

#include <assert.h>
#include <array>

#include "Manager/TextureManager.h"
#include "System/DirectXCommon.h"
#include "System/Screen.h"

PipelineManager::PipelineManager()
{
	// Device�擾
	device_ = DirectXCommon::GetInstance().Device;
	cmd_list_ = DirectXCommon::GetInstance().CmdList;

#pragma region Gbuffer
	{
		// �u�����h�f�X�N�ėp
		D3D12_RENDER_TARGET_BLEND_DESC render_target_blend_desc{};
		render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
		render_target_blend_desc.BlendEnable = true;
		render_target_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		render_target_blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;

		D3D12_BLEND_DESC blend_desc;
		for (int i = 0; i < 8; ++i)
		{
			blend_desc.RenderTarget[0] = render_target_blend_desc;
		}

		// �X�^�e�B�b�N�T���v���[
		CD3DX12_STATIC_SAMPLER_DESC sampler_desc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);


		// ���_���C�A�E�g
		D3D12_INPUT_ELEMENT_DESC input_layout[] =
		{
			{ // xy���W
				"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // �@���x�N�g��
				"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // �ڐ�
				"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // uv���W
				"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // �J���[
				"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // �{�[���e���x
				"WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // �{�[��
				"BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},

		};

		// �`��t�H�[�}�b�g
		DXGI_FORMAT rtv_format[] =
		{
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_R16G16B16A16_FLOAT,

		};

		// �f�X�N���v�^�����W(�e�N�X�`�����l���܂�)
		CD3DX12_DESCRIPTOR_RANGE desc_range_srv_albedo = {};
		CD3DX12_DESCRIPTOR_RANGE desc_range_srv_nomal = {};
		CD3DX12_DESCRIPTOR_RANGE desc_range_srv_ms = {};
		desc_range_srv_albedo.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 �x�[�X�J���[
		desc_range_srv_nomal.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); // t1 �m�[�}��
		desc_range_srv_ms.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2); // t2 ���^���ƃ��t�l�X

		// ���[�g�p�����[�^
		CD3DX12_ROOT_PARAMETER rootparams[6]{};
		// �J����CBV
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		// ���b�V��CBV
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
		// �T�u�Z�b�gCBV
		rootparams[2].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`���j
		rootparams[3].InitAsDescriptorTable(1, &desc_range_srv_albedo, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[4].InitAsDescriptorTable(1, &desc_range_srv_nomal, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[5].InitAsDescriptorTable(1, &desc_range_srv_ms, D3D12_SHADER_VISIBILITY_ALL);



		// �p�C�v���C������
		if (!CreatePipeline(
			"Gbuffer",
			CreateGraphicsPipelineState(
				blend_desc,
				input_layout,
				sizeof(input_layout) / sizeof(input_layout[0]),
				rtv_format,
				sizeof(rtv_format) / sizeof(rtv_format[0])
			),
			CreateRootSignatureDesc(
				rootparams,
				sizeof(rootparams) / sizeof(rootparams[0]),
				sampler_desc)))
		{
		}
	}
#pragma endregion


#pragma region Deferred
	{
		// �u�����h�f�X�N�ėp
		D3D12_RENDER_TARGET_BLEND_DESC render_target_blend_desc_defferd{};
		render_target_blend_desc_defferd.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
		render_target_blend_desc_defferd.BlendEnable = true;
		render_target_blend_desc_defferd.BlendOp = D3D12_BLEND_OP_ADD;
		render_target_blend_desc_defferd.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		render_target_blend_desc_defferd.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		render_target_blend_desc_defferd.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		render_target_blend_desc_defferd.SrcBlendAlpha = D3D12_BLEND_ONE;
		render_target_blend_desc_defferd.DestBlendAlpha = D3D12_BLEND_ZERO;

		D3D12_BLEND_DESC blend_desc_defferd;
		for (int i = 0; i < 8; ++i)
		{
			blend_desc_defferd.RenderTarget[0] = render_target_blend_desc_defferd;
		}

		// �X�^�e�B�b�N�T���v���[
		CD3DX12_STATIC_SAMPLER_DESC sampler_desc_defferd = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);


		// ���_���C�A�E�g
		D3D12_INPUT_ELEMENT_DESC input_layout_deffered[] = {
			{
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			}
		};

		DXGI_FORMAT rtv_format_deffered[]
		{
			DXGI_FORMAT_R8G8B8A8_UNORM,
		};



		// �f�X�N���v�^�����W
		const int kRegisterMax{ 6 };
		CD3DX12_DESCRIPTOR_RANGE desc_range_srv_deffered[kRegisterMax]{};
		for (int i = 0; i < kRegisterMax; ++i)
		{

			desc_range_srv_deffered[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i); // t���W�X�^

		}

		// ���[�g�p�����[�^
		CD3DX12_ROOT_PARAMETER rootparams_deffered[2]{};
		// �摜�f�[�^
		rootparams_deffered[0].InitAsDescriptorTable(_countof(desc_range_srv_deffered), desc_range_srv_deffered, D3D12_SHADER_VISIBILITY_ALL);
		// ���C�g�p�o�b�t�@
		rootparams_deffered[1].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);


		/*for (int i = 0; i <= 10000;++i)
		{
			int a = i+i;
		}*/
		// �p�C�v���C������
		if (CreatePipeline(
			"Deferred",
			CreateGraphicsPipelineState(
				blend_desc_defferd,
				input_layout_deffered,
				sizeof(input_layout_deffered) / sizeof(input_layout_deffered[0]),
				rtv_format_deffered,
				sizeof(rtv_format_deffered) / sizeof(rtv_format_deffered[0])
			),
			CreateRootSignatureDesc(
				rootparams_deffered,
				sizeof(rootparams_deffered) / sizeof(rootparams_deffered[0]),
				sampler_desc_defferd
			)))
		{

		}
	}


#pragma endregion



}

bool PipelineManager::CreatePipeline(
	const std::string &pipelineName,
	D3D12_GRAPHICS_PIPELINE_STATE_DESC &graphicsPipelineState,
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC &rootSignatureDesc)
{
	HRESULT result = S_FALSE;
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g


	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	const std::string vs_filename = "Shader/" + pipelineName + "VS.hlsl";
	std::wstring vs_filename__w = std::wstring(vs_filename.begin(), vs_filename.end());
	result = D3DCompileFromFile(
		vs_filename__w.c_str(),	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	const std::string ps_filename = "Shader/" + pipelineName + "PS.hlsl";
	std::wstring ps_filename_w = std::wstring(ps_filename.begin(), ps_filename.end());

	result = D3DCompileFromFile(
		ps_filename_w.c_str(),	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	graphicsPipelineState.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	graphicsPipelineState.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	// ���[�g�V�O�l�`���̐���
	result = device_->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(piplines_[pipelineName].root_signature.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	graphicsPipelineState.pRootSignature = piplines_[pipelineName].root_signature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device_->CreateGraphicsPipelineState(
		&graphicsPipelineState,
		IID_PPV_ARGS(piplines_[pipelineName].pipline_state.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	return true;
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineManager::CreateGraphicsPipelineState(
	const D3D12_BLEND_DESC &blendDesc,
	D3D12_INPUT_ELEMENT_DESC inputLayout[],
	int inputLayoutNum,
	DXGI_FORMAT renderTargetViewFormats[],
	int numRenderTarget
)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphics_pipeline_state{};

	// �T���v���}�X�N
	graphics_pipeline_state.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// ���X�^���C�U�X�e�[�g
	graphics_pipeline_state.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	graphics_pipeline_state.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�f�v�X�X�e���V���X�e�[�g
	graphics_pipeline_state.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	//�[�x�e�X�g���s��
	graphics_pipeline_state.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;		//��ɏ㏑��
	graphics_pipeline_state.DepthStencilState.DepthEnable = false;

	// �u�����h�X�e�[�g�Z�b�g
	graphics_pipeline_state.BlendState = blendDesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	graphics_pipeline_state.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	;
	graphics_pipeline_state.InputLayout.pInputElementDescs = inputLayout;
	graphics_pipeline_state.InputLayout.NumElements = inputLayoutNum;

	// �v���~�e�B�u�ݒ�
	graphics_pipeline_state.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// �`��ݒ�
	graphics_pipeline_state.NumRenderTargets = numRenderTarget;
	for (int i = 0; i < numRenderTarget; ++i)
	{
		graphics_pipeline_state.RTVFormats[i] = renderTargetViewFormats[i];
	}
	graphics_pipeline_state.SampleDesc.Count = 1;	// 1�s�N�Z���ɂ�1�T���v�����O


	return graphics_pipeline_state;
}

CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC PipelineManager::CreateRootSignatureDesc(
	CD3DX12_ROOT_PARAMETER rootparams[],
	int numRootParams,
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc
)
{
	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC root_signature_desc{};
	root_signature_desc.Init_1_0(numRootParams, rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	return root_signature_desc;
}


void PipelineManager::SetPipeline(const std::string &pipelineName)
{
	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmd_list_->SetPipelineState(piplines_[pipelineName].pipline_state.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmd_list_->SetGraphicsRootSignature(piplines_[pipelineName].root_signature.Get());
}

