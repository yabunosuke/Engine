#include "GBuffer.hlsli"

Texture2D diffuse_t : register(t0);
Texture2D normal_t : register(t1);
Texture2D metal_roughness_t : register(t2);

SamplerState texture_sampler : register(s0);

PSOutput main(VSOutput input)
{
	PSOutput pout;

	// 拡散反射光の色
	float4 albedo = diffuse_t.Sample(texture_sampler, input.texcoord);
	// メタルなめらかさ
	float2 ms = metal_roughness_t.Sample(texture_sampler, input.texcoord).ra;
	// 法線
	float3 normal = normal_t.Sample(texture_sampler, input.texcoord).rgb;

	// テクスチャなしの画像が来たらマテリアルカラーに変更
	if (0 == albedo.r + albedo.g + albedo.b + albedo.a)
	{
		albedo = input.color;
	}
	albedo.a = 1.0f;

	// 法線マッピングとテクスチャチェック
	if (0 != normal.r + normal.g + normal.b)
	{
		// 回転行列
		float3x3 world_normal_matrix = {
			normalize(input.tangent),
			normalize(input.binormal),
			normalize(input.normal)
		};
		// ローカル空間に法線をワールドへ変換
		normal = normalize(mul(normal * 2.0f - 1.0f, world_normal_matrix));
	}
	else
	{
		normal = input.normal;
	}

	// 深度
	float depth = input.viewprojection_position.z / input.viewprojection_position.w;

	albedo = pow(albedo, 2.2f);
	pout.color = albedo;
	pout.normal = float4(normal, 1.0f);
	pout.metal_roughness = float4(ms.x, 1.0f - ms.y, 1, 1);
	pout.depth = float4(depth, depth, 1.0f, 1.0f);	// 奥が青色になる
	pout.position = input.world_position.xyzw;
	pout.id = float4(1, 0, 0, 1);	// 仮でID決め打ち

	return pout;
}