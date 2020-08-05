 //====================================================================================================
// Filename:	DoLighting.fx
// Created by:	Peter Chan
// Description: Shader that applies transformation matrices.
//====================================================================================================
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap: register(t2);
Texture2D normalMap: register(t3);
SamplerState textureSampler : register(s0);

cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix WVP;
	float4 ViewPosition;
	float4 ViewDir;
	float4 CamerDepth;
}

cbuffer LightBuffer : register(b1)
{
	float4 LightDirection;
	float4 LightAmbient;
	float4 LightDiffuse;
	float4 LightSpecular;
}

cbuffer MaterialBuffer: register(b2)
{
	float4 MaterialAmbient;
	float4 MaterialDiffuse;
	float4 MaterialSpecular;
	float4 MaterialPower;
}

cbuffer OptionsBuffer : register (b3)
{
	float4 displacementWeight;
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
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float3 dirToLight : TEXCOORD0;
	float3 dirToView : TEXCOORD1;
	float2 texCoord : TEXCOORD2;
	float depth : DEPTH;
};

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;

	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float4 position = float4(input.position.xyz, 1.0f) + (float4(input.normal, 0.0f) * displacement * displacementWeight.x);

	float3 normal = mul(input.normal, (float3x3)World);
	float3 tangent = mul(input.tangent, (float3x3)World);
	float3 binormal = cross(normal, tangent);

	output.position = mul(float4(position.xyz, 1.0f), WVP);
	output.normal = normal;
	output.tangent = tangent;
	output.binormal = binormal;
	output.dirToLight = -LightDirection.xyz;
	output.dirToView = normalize(ViewPosition.xyz - mul(float4(position.xyz, 1.0f), World).xyz);
	output.texCoord = input.texCoord;
	//output.depth = dot(mul(position, World) - ViewPosition.xyz, ViewDir.xyz);
	output.depth = mul(mul(float4(position.xyz, 1.0f), World), View).z / CamerDepth;

	return output;
}

float4 PS(VSOutput input) : SV_Target
{

	return float4(input.depth,input.depth,input.depth, 1.0f);
}
