#include "Deferred.hlsli"


Texture2D color_texture_2d				: register(t0);
Texture2D normal_texture_2d				: register(t1);
Texture2D metal_roughness_texture_2d	: register(t2);
Texture2D depth_texture_2d				: register(t3);
Texture2D position_texture_2d			: register(t4);
Texture2D id_texture_2d					: register(t5);
//Texture2D panorama_sky_box				: register(t6);

SamplerState texture_sampler : register(s0);


///////////////////////////////////////////
// 関数宣言
///////////////////////////////////////////
float3 CalcLambertDiffuse(float3 light_direction, float3 light_color, float3 normal);
float3 CalcPhongSpecular(float3 light_direction, float3 light_color, float3 world_pos, float3 normal, float3 eye_position);

float4 main(VSOutput input) : SV_TARGET
{
	float4 output_color;

	float4 color_tex =				color_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 normal_tex =				normal_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 metal_roughness_tex =	metal_roughness_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 depth_tex =				depth_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 position_tex =			position_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 id_tex =					id_texture_2d.Sample(texture_sampler, input.texcoord);
	//float4 sky_box =				panorama_sky_box.Sample(texture_sampler, input.texcoord);

	/// 仮計算
	if (id_tex.r != 0) {


		float3 diffuse, specular;


		// Lightの計算
		for(int i =0;i<LIGHT_MAX;++i)
		{
			if(light[i].is_active == 0.0f)
			{
				break;
			}

			// DirectionalLight
			if (light[i].light_type.y != 0)
			{
				float t = max(0.0f,dot(normal_tex.xyz, light[i].direction_.xyz));

				float3 diffuse_directonal = light[i].color.rgb * t;
				diffuse += diffuse_directonal * light[i].intensity;

				float3 specular_directonal = CalcPhongSpecular(
					light[i].direction_.xyz,
					light[i].color.xyz,
					position_tex.xyz,
					normal_tex.xyz,
					eye_pos.xyz
				);
				specular += specular_directonal;
			}

			// PointLight
			if (light[i].light_type.z != 0)
			{
				// 光の入射角を求める
				float3 light_dir = position_tex.xyz - light[i].position.xyz;
				light_dir = normalize(light_dir);

				// 減衰なしのLambert拡散光を計算
				float3 diffuse_point = CalcLambertDiffuse(
					light_dir,
					light[i].color.xyz,
					normalize(normal_tex.xyz)
				);

				// 減衰なしのPhon鏡面反射光を計算
				float3 specular_point = CalcPhongSpecular(
					light_dir,
					light[i].color.xyz,
					position_tex.xyz,
					normal_tex.xyz,
					eye_pos.xyz
				);
				// ポイントライトとの距離を計算する
				float distance = length(position_tex. xyz - light[i].position.xyz);
				// 影響率の計算
				float affect = 1.0f - distance / light[i].range;
				if (affect < 0.0f)
				{
					affect = 0.0f;
				}
				// 指数関数化
				affect = pow(affect, 3);

				diffuse_point *= affect;
				specular_point *= affect;

				// 加算
				diffuse += diffuse_point * light[i].intensity;
				specular += specular_point * light[i].intensity;
			}
		}


		float3 light = diffuse + specular ;

		output_color = float4(color_tex.rgb * light + (color_tex.rgb * float3(0.5f, 0.5f, 0.5f)), color_tex.w);
		

		return output_color;
	}


	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}


/// <summary>
/// Lambert拡散反射光の計算
/// </summary>
float3 CalcLambertDiffuse(float3 light_direction, float3 light_color, float3 normal)
{
	// ピクセルの法線とライトの方向の内積を計算する
	float t = saturate(dot(normal, light_direction) * -1.0f);

	// 内積の値を0以上の値にする
	t = max(0.0f, t);

	// 拡散反射光を計算する
	return light_color * t;
	
}

/// <summary>
/// Phong拡散反射光の計算
/// </summary>
float3 CalcPhongSpecular(float3 light_direction, float3 light_color, float3 world_pos, float3 normal,float3 eye_position)
{
	// 反射ベクトルを求める
	float3 reflection_vec = reflect(light_direction, normal);

	// 光が当たったサーフェイスから視点に伸びるベクトルを求める
	float3 to_eye = eye_position - world_pos;
	to_eye = normalize(to_eye);

	// 鏡面反射の強さを求める
	float t = dot(reflection_vec, to_eye);

	// 鏡面反射の強さを0以上の数値にする
	t = max(0.0f, t);

	// 鏡面反射の強さを絞る
	t = pow(t, 5.0f);

	// 鏡面反射光を求める
	return light_color * t;
}
