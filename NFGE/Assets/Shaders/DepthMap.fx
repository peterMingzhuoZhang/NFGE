//====================================================================================================
// Filename:	DoLighting.fx
// Created by:	Peter Chan
// Description: .
//====================================================================================================


cbuffer TransformBuffer : register(b0)
{
	matrix WVP;
}

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 positionNDC : TEXCOORD0;
};

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(float4(input.position, 1.0f), WVP);
	output.positionNDC = output.position;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	float depth = 1.0f - input.positionNDC.z / input.positionNDC.w;
	return float4(depth, 1.0f, 1.0f, 1.0f);
}
