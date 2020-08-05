//====================================================================================================
// Filename:	DoLighting.fx
// Created by:	Peter Chan
// Description: Shader that applies transformation matrices.
//====================================================================================================
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap: register(t2);
Texture2D normalMap: register(t3);
Texture2D aoMap: register(t4);
Texture2D depthMap: register(t5);
SamplerState textureSampler : register(s0);

static const float4 Zero4 = float4(0.0f, 0.0f, 0.0f, 0.0f);
static const float4 One4 = float4(1.0f, 1.0f, 1.0f, 1.0f);
static const float4 Half4 = float4(0.5f, 0.5f, 0.5f, 0.5f);

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

cbuffer OptionsBuffer : register (b3)
{
	float displacementWeight : packoffset(c0.x);
	float notimportant : packoffset(c0.y);
	float brightness : packoffset(c0.z);
	bool useShadow : packoffset(c0.w);

}

cbuffer ShadowBufer : register(b4)
{
	matrix WVPLight;
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
	float4 positionNDC : TEXCOORD4; // new
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
	if (useShadow)
	{
		output.positionNDC = mul(float4(position.xyz, 1.0f), WVPLight);
	}

	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
	if (normalColor.x == Zero4.x && normalColor.y == Zero4.y && normalColor.z == Zero4.z&& normalColor.w == Zero4.w)
	{
		normalColor = Half4;
	}

	float3 sampledNormal = float3((2.0f * normalColor.xy) - 1.0f, normalColor.z);

	float3 n = normalize(input.normal);
	float3 b = normalize(input.binormal);
	float3 t = normalize(input.tangent);
	float3x3 tbnw = float3x3(t, b, n);

	float3 normal = normalize(mul(sampledNormal, tbnw));
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float4 ao = aoMap.Sample(textureSampler, input.texCoord);
	if (ao.x == Zero4.x && ao.y == Zero4.y && ao.z == Zero4.z&& ao.w == Zero4.w)
	{
		ao = One4;
	}

	float4 ambient = LightAmbient * MaterialAmbient * ao;

	float d = saturate(dot(dirToLight, normal));
	float4 diffuse = d * LightDiffuse * MaterialDiffuse;

	float3 r = reflect(LightDirection.xyz, normal);
	float s = saturate(dot(dirToView, r));
	float4 specular = pow(s, MaterialPower.x) * LightSpecular * MaterialSpecular;

	float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);
	if (diffuseColor.x == Zero4.x && diffuseColor.y == Zero4.y && diffuseColor.z == Zero4.z&& diffuseColor.w == Zero4.w)
	{
		diffuseColor = One4;
	}

	float4 textureSpecular = specularMap.Sample(textureSampler, input.texCoord);

	float useBrightness = brightness;
	if (brightness == 0.0f)
	{
		useBrightness = 1.0f;
	}

	float4 color = ((ambient + diffuse) * diffuseColor * useBrightness + specular * textureSpecular.x);

	if (useShadow)
	{
		float actualDepth = 1.0f - input.positionNDC.z / input.positionNDC.w; // flip it
		float2 shadowUV = input.positionNDC.xy / input.positionNDC.w;
		shadowUV = (shadowUV + 1.0f) * 0.5f;
		shadowUV.y = 1.0f - shadowUV.y;

		float depthBias = 0.0003; // twik witt imgui
		if (saturate(shadowUV.x) == shadowUV.x && saturate(shadowUV.y) == shadowUV.y)
		{
			float saveDepth = depthMap.Sample(textureSampler, shadowUV).x;

			if (saveDepth > actualDepth + depthBias) // flip it 
				color = ambient * diffuseColor;
		}
		
	}

	return color;
}
