//====================================================================================================
// Filename:	Effect_TextureMesh.fx
// Created by:	Mingzhuo Zhang
// Description:	Simple shader for applying diffuse texture to TextureMesh.
//====================================================================================================

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position	: POSITION;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(float4(input.position.xyz, 1.0f), World);
	output.position = mul(output.position, View);
	output.position = mul(output.position, Projection);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	return diffuseMap.Sample(textureSampler, input.texCoord);
}
