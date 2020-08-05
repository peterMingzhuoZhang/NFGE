//====================================================================================================
// Filename:	DoLighting.fx
// Created by:	Peter Chan
// Description: Shader that applies transformation matrices.
//====================================================================================================

cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix WVP;
	float4 ViewPosition;
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

struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(input.position, WVP);

	float4 ambient = LightAmbient * MaterialAmbient;

	float3 dirToLight = -LightDirection.xyz;
	float3 normalWorld = mul(input.normal, (float3x3)World);
	float d = saturate(dot(dirToLight, normalWorld));
	float4 diffuse = d * LightDiffuse * MaterialDiffuse;

	float3 dirToView = normalize(ViewPosition.xyz - mul(input.position, World).xyz);
	float3 r = reflect(LightDirection.xyz, normalWorld);
	float s = saturate(dot(dirToView, r));
	float4 specular = pow(s, MaterialPower.x) * LightSpecular * MaterialSpecular;

	output.color = ambient + diffuse + specular;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	return input.color;
}
