//====================================================================================================
// Filename:	SimpleDraw.fx
// Created by:	Peter Chan
// Description: Shader that does SimpleDraw.
//====================================================================================================

//====================================================================================================
// Structs
//====================================================================================================


cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
}


struct VS_INPUT
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

//====================================================================================================
// Vertex Shader
//====================================================================================================

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(float4(input.Pos.xyz, 1.0f), wvp);
	output.Color = input.Color;
	return output;
}

//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.Color;
}
