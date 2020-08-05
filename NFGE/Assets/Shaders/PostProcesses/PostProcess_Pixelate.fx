//====================================================================================================
// Filename:	PostProcess_Pixelate.fx
// Created by:	Mingzhuo Zhang
// Description:	Shader for applying pixelate on the NDC quad.
//====================================================================================================

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

cbuffer OptionBuffer : register(b0)
{
	float pixelPower;
}

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
	output.position = float4(input.position.xyz, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float2 uv = input.texCoord;
	float u = (int)(uv.x * (int)pixelPower) / pixelPower;
	float v = (int)(uv.y * (int)pixelPower) / pixelPower;
    //float u = uv.x;
    //float v = uv.y;
	float4 color = diffuseMap.Sample(textureSampler, float2(u,v));
	return color;
}