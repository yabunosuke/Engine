#include "GBuffer.hlsli"

VSOutput main(VSInput input)
{
	VSOutput vout;	// 出力情報

	float3 p = { 0, 0, 0 };	// 座標
	float3 n = { 0, 0, 0 };	// 法線
	float3 t = { 0, 0, 0 };	// 接線
	// ボーンの影響は四つまで
	for (int i = 0; i < 4; i++)
	{
		p += (input.boneWeights[i] * mul(input.position, bone_transforms[input.boneIndices[i]])).xyz;
		n += (input.boneWeights[i] * mul(float4(input.normal.xyz, 0), bone_transforms[input.boneIndices[i]])).xyz;
		t += (input.boneWeights[i] * mul(float4(input.tangent.xyz, 0), bone_transforms[input.boneIndices[i]])).xyz;
	}

	// ワールド変換行列
	vout.world_position = float4(p, 1.0f);
	vout.viewprojection_position = mul(float4(p, 1.0f), view_projection);

	// ボーンの影響を考慮したノーマルを返す
	vout.color.rgb = input.color.rgb * material_color.rgb;
	vout.color.a = input.color.a * material_color.a;
	vout.normal = normalize(n);
	vout.tangent = normalize(t);
	vout.binormal = cross(n, t);
	vout.texcoord = input.texcoord.xy;

	return vout;
}
