// 頂点シェーダーに入力される値
struct VSInput
{
	float4 position     : POSITION;	// ワールド座標
	float3 normal		: NORMAL;	// 法線
	float3 tangent		: TANGENT;	// 接線
	float3 texcoord		: TEXCOORD;	// UV座標
	float4 color		: COLOR;	// 色情報
	float4 boneWeights : WEIGHTS;	// ボーン影響度
	uint4  boneIndices : BONES;		// ボーンインデックス
};

// 頂点シェーダーからピクセルシェーダーに受け渡される値
struct VSOutput
{
	float4 viewprojection_position : SV_POSITION;	// VP座標
	float4 world_position : W_POSITION;	// ワールド座標座標
	float2 texcoord : TEXCOORD;		// UV座標
	float4 color    : COLOR;		// 色情報
	float3 normal	: NROMAL;		// 法線
	float3 tangent	: TANGENT;		// 接線
	float3 binormal	: BINROMAL;		// 従法線

};

// マルチレンダリング用構造体
struct PSOutput
{
	float4 color				: SV_TARGET0;	// 色画像
	float4 normal				: SV_TARGET1;	// 法線画像
	float4 metal_roughness		: SV_TARGET2;	// メタルとラフネス画像
	float4 depth				: SV_TARGET3;	// 深度画像
	float4 position				: SV_TARGET4;	// ワールド座標画像
	float4 id					: SV_TARGET5;	// 処理変更用画像
};

// カメラバッファ
cbuffer CONSTANT_BUFFER_CAMERA : register(b0)
{
	float4 view_position;					// カメラ座標
	row_major float4x4	view_projection;	// ビュープロジェクション
	row_major float4x4	inv_view_projection;// ビュープロジェクション逆行列
};

// メッシュバッファ
#define MAX_BONES 256
cbuffer CONSTANT_BUFFER_MESH : register(b1)
{
	row_major float4x4	bone_transforms[MAX_BONES];
};

// サブセットバッファ
cbuffer CONSTANT_BUFFER_SUBSET : register(b2)
{
	float4 material_color;
	
};