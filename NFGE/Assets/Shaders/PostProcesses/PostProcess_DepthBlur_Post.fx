//====================================================================================================
// Filename:	PostProcess_DepthBlur_Post.fx
// Created by:	Mingzhuo Zhang
// Description: Shader that Blur the final picture from a NDC quad base on the depth texture.
//====================================================================================================
Texture2D diffuseMap : register(t0);
Texture2D depthMap : register(t1);
SamplerState textureSampler : register(s0);

cbuffer OptionsBuffer : register (b0)
{
	float focusDepth;
	float blurRange;
	float blurPower;
	float2 screenSize;
}

struct VSInput
{
	float3 position : POSITION;
	float2 texCoord	: TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 texCoord	: TEXCOORD;
};

float4 blur(VSOutput input)
{
    float2 uv = input.texCoord;
    float u = uv.x;
    float v = uv.y;
    float4 depth = depthMap.Sample(textureSampler, float2(u, v));

    float distanceToFD = abs(depth.x - focusDepth.x); // distance to focus depth
    float normalDisToFD = distanceToFD / blurRange.x;
    normalDisToFD = clamp((normalDisToFD), 0.0f, 1.0f);
    float currentPower = normalDisToFD * blurPower.x;

    int totalCountPixel = 0;
    float HorizUnit = 1.0f / screenSize.x;
    float VertiUnit = 1.0f / screenSize.y;
    float4 totalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    int loopCount = (int)currentPower;

	[unroll(10)]
    for (int i = 0; i < loopCount; ++i)
    {
        float currentU_0 = u - HorizUnit * (currentPower - (float) i);
        float currentU_1 = u + HorizUnit * (currentPower - (float) i);
        [unroll(10)]
        for (int j = 0; j < loopCount; ++j)
        {
            float currentV_0 = v - VertiUnit * (currentPower - (float) j);
            float currentV_1 = v + VertiUnit * (currentPower - (float)j);
			
            totalColor += diffuseMap.Sample(textureSampler, float2(currentU_0, currentV_0));
            ++totalCountPixel;
	
            totalColor += diffuseMap.Sample(textureSampler, float2(currentU_1, currentV_1));
            ++totalCountPixel;
        }
    }
	
    totalColor += diffuseMap.Sample(textureSampler, float2(u, v));
    ++totalCountPixel;

	//return totalColor / totalCountPixel;
	//return float4(currentPower / 100.0f, currentPower/ 100.0f, currentPower/ 100.0f, 1.0f);
    return totalColor / totalCountPixel;
}

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = float4(input.position.xyz, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
    return blur(input);

}

