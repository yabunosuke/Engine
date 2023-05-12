#include "Deferred.hlsli"
VSOutput main(float4 pos : POSITION, float2 texcoord : TEXCOORD)
{
	VSOutput output;	//ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos);
	output.texcoord = texcoord;
	return output;
}