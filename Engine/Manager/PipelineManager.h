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
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipline_state{};	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12RootSignature> root_signature{};// ルートシグネチャ

};

class PipelineManager :
	public Singleton<PipelineManager>
{
public:
	friend class Singleton<PipelineManager>; // インスタンス作成を許可

	void SetPipeline(const std::string &pipelineName);



private:
	PipelineManager();
	
	/// <summary>
	/// パイプライン生成
	/// </summary>
	/// <param name="pipelineName"></param>
	/// <param name="graphicsPipelineState"></param>
	/// <param name="rootSignatureDesc"></param>
	bool CreatePipeline(
		const std::string &pipelineName, 
		D3D12_GRAPHICS_PIPELINE_STATE_DESC &graphicsPipelineState, 
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC &rootSignatureDesc);

	/// <summary>
	/// パイプラインステート生成
	/// </summary>
	/// <param name="blendDesc">ブレンド設定</param>
	/// <param name="inputLayout">頂点レイアウト</param>
	/// <param name="inputLayoutNum">頂点レイアウトの数</param>
	/// <param name="renderTargetViewFormats">描画フォーマット</param>
	/// <param name="numRenderTarget">使用するレンダーターゲットの数</param>
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

	std::map<std::string, Pipeline> piplines_;	// パイプラインコンテナ

	ID3D12Device *device_;						// デバイス取得用
	ID3D12GraphicsCommandList *cmd_list_;		// コマンドリスト取得用
};

