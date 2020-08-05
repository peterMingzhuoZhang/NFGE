//====================================================================================================
// Filename:	Particle.fx
// Created by:	Peter Chan
// Description:	Shader for rendering point sprite.
//====================================================================================================

//====================================================================================================
// Constant Buffers
//====================================================================================================

cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
	vector cameraPosition;
	vector cameraUp;
}

//====================================================================================================
// Textures and Samplers
//====================================================================================================

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

//====================================================================================================
// Structs
//====================================================================================================

struct A2V
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

//----------------------------------------------------------------------------------------------------

struct V2G
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

//----------------------------------------------------------------------------------------------------

struct G2P
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

//====================================================================================================
// Vertex Shader
//====================================================================================================

V2G VS(A2V input)
{
	V2G output = (V2G)0;
	output.position = input.position.xyz;
	output.normal = input.normal;
	output.color = input.color;
	return output;
}

//====================================================================================================
// Geometry Shader
//====================================================================================================

[maxvertexcount(4)]
void GS(point V2G input[1], inout TriangleStream<G2P> outputStream)
{
	float3 look = normalize(cameraPosition.xyz - input[0].position);
	float3 right = normalize(cross(cameraUp.xyz, look));
	float3 up = normalize(cross(look, right));
	float3 dimension = input[0].normal;

	float4 v[4];
	v[0] = float4(input[0].position.xyz + (right * dimension.x) - (up * dimension.y), 1.0f);
	v[1] = float4(input[0].position.xyz + (right * dimension.x) + (up * dimension.y), 1.0f);
	v[2] = float4(input[0].position.xyz - (right * dimension.x) - (up * dimension.y), 1.0f);
	v[3] = float4(input[0].position.xyz - (right * dimension.x) + (up * dimension.y), 1.0f);

	float2 uv[4];
	uv[0] = float2(0.0f, 1.0f);
	uv[1] = float2(0.0f, 0.0f);
	uv[2] = float2(1.0f, 1.0f);
	uv[3] = float2(1.0f, 0.0f);

	G2P output = (G2P)0;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		output.position = mul(v[i], wvp);
		output.color = input[0].color;
		output.texCoord = uv[i];
		outputStream.Append(output);
	}
}

//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS(G2P input) : SV_Target
{
	float4 texColor = diffuseMap.Sample(textureSampler, input.texCoord);
	return input.color * texColor;
}