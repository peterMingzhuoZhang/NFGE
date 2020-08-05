//====================================================================================================
// Filename:	Terrain.fx
// Created by:	Peter Chan
// Description:	Simple shader for terrain.
//====================================================================================================

//====================================================================================================
// Constant Buffers
//====================================================================================================

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix WVP;
	float3 ViewPosition;
	vector LightDirection;
	vector LightAmbient;
	vector LightDiffuse;
	vector LightSpecular;
	float LightPower;
}

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

//====================================================================================================
// Structs
//====================================================================================================

struct VS_INPUT
{
	float4 position	: POSITION;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position		: SV_POSITION;
	float3 normal		: TEXCOORD0;
	float3 tangent		: TEXCOORD1;
	float3 dirToView	: TEXCOORD2;
	float2 texCoord		: TEXCOORD3;
};

//====================================================================================================
// Vertex Shader
//====================================================================================================

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(input.position, WVP);
	output.normal = mul(input.normal.xyz, (float3x3)World);
	output.tangent = mul(input.tangent.xyz, (float3x3)World);
	output.dirToView = ViewPosition - mul(input.position.xyz, (float3x3)World);
	output.texCoord = input.texCoord * 10.0f;
	return output;
}

//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 normal = normalize(input.normal);
	float3 dirToLight = normalize(-LightDirection.xyz);
	float3 dirToView = normalize(input.dirToView);

	float4 ambient = LightAmbient;
	float4 diffuse = LightDiffuse * saturate(dot(normal, dirToLight));
	float4 specular = LightSpecular * pow(saturate(dot(normal, (dirToLight + dirToView) * 0.5)), LightPower);

	float4 textureColor = diffuseMap.Sample(textureSampler, input.texCoord);

	return (ambient + diffuse) * textureColor + specular;
}
