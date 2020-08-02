//====================================================================================================
// Filename:	Effect_ColorMesh.fx
// Created by:	Mingzhuo Zhang
// Description: Shader that that do transform on ColorMesh.
//====================================================================================================

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

VS_OUTPUT VS(float3 Pos : POSITION, float4 Color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(float4(Pos.xyz, 1.0f), World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Color = Color;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.Color;
}
