// Copyright (C) NeoAxis Group Ltd. This is part of NeoAxis 3D Engine SDK.

#define FXAA_PC 1

#ifdef CG_COMPILER
	#define FXAA_CG_OPENGL 1
#else
	#define FXAA_HLSL_3 1
#endif

#include "Fxaa3_9.h"

float4 luma_fp(
	uniform float4 viewportSize,
	float2 texCoord : TEXCOORD0,
	sampler rt : register(s0)
	) : COLOR
{
	float3 color = tex2D(rt, texCoord).rgb;
	float luma = dot(sqrt(color.rgb), float3(0.299, 0.587, 0.114)); // compute luma
	//float luma = dot(color.rgb, float3(0.299, 0.587, 0.114)); // compute luma
	return float4(color, luma);
}

float4 final_fp(
	uniform float4 viewportSize,
	float2 texCoord : TEXCOORD0,
	sampler rt : register(s0)
	) : COLOR
{
	return FxaaPixelShader(texCoord, float4(0,0,0,0), rt, viewportSize.zw, float4(0,0,0,0));
}
