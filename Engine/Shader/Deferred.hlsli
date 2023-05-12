cbuffer CONSTANT_BUFFER : register(b0)
{
    matrix mat; //3D変換行列
}

// ライトの最大数
#define LIGHT_MAX 128

struct Light
{
	/// <summary>
	/// ライトの種類
	/// </summary>
	/// <param name="x = 1">Spot Light</param>
	/// <param name="y = 1">Directional Light</param>
	/// <param name="z = 1">Point Light</param>
	float4 light_type;

	// 共通項目
	float4 position;	// ライトの座標
	float4 color;		// ライトの色

	float intensity;	// 強さ
	float range;		// 長さ
	float is_active;	// 有効
	float PADING;		// パディング

	// ディレクションライト用
	float4 direction_;	// ライトの向き
};

cbuffer LIGHT_CONSTANT_BUFFER : register(b0)
{
	Light light[LIGHT_MAX];	//ポイントライト128個
	float4 eye_pos;
}

// 頂点シェーダーからピクセルシェーダ―へのやり取りに使用する構造体
struct VSOutput {
	float4 svpos : SV_POSITION;	//システム用長点座標
	float2 texcoord : TEXCOORD;	//uv値
};