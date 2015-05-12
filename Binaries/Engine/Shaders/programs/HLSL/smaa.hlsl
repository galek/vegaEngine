uniform float4 viewportSize;

#define SMAA_PRESET_ULTRA 1
#define SMAA_PIXEL_SIZE (float2(viewportSize.zw))
#define SMAA_HLSL_3 1
#include "SMAA.h"




void EdgeDetection_VS(in float4 position : POSITION,
	out float4 oPosition : POSITION,
	uniform float4x4 wvp,
	inout float2 texcoord : TEXCOORD0,
	out float4 offset[3] : TEXCOORD1
	) {
	position.xy = sign(position.xy);

	texcoord = (float2(position.x, -position.y) + 1.0f) * 0.5f;

	SMAAEdgeDetectionVS(position, oPosition, texcoord, offset);

	oPosition = mul(wvp, position);
}

void BlendWeightCalculation_VS(in float4 position : POSITION,
	out float4 oPosition : POSITION,
	uniform float4x4 wvp,
	inout float2 texcoord : TEXCOORD0,
	out float2 pixcoord : TEXCOORD1,
	out float4 offset[3] : TEXCOORD2
	) {
	position.xy = sign(position.xy);

	texcoord = (float2(position.x, -position.y) + 1.0f) * 0.5f;

	SMAABlendingWeightCalculationVS(position, oPosition, texcoord, pixcoord, offset);
	oPosition = mul(wvp, position);
}

void NeighborhoodBlending_VS(in float4 position : POSITION,
	out float4 oPosition : POSITION,
	uniform float4x4 wvp,
	inout float2 texcoord : TEXCOORD0,
	out float4 offset[2] : TEXCOORD1
	) {
	position.xy = sign(position.xy);

	texcoord = (float2(position.x, -position.y) + 1.0f) * 0.5f;

	SMAANeighborhoodBlendingVS(position, oPosition, texcoord, offset);
	oPosition = mul(wvp, position);
}


float4 LumaEdgeDetection_PS(float4 position : POSITION,
	float2 texcoord : TEXCOORD0,
	float4 offset[3] : TEXCOORD1,
#if SMAA_PREDICATION == 1
	SMAATexture2D predicationTex : TEXUNIT1,
#endif
	uniform SMAATexture2D colorGammaTex : TEXUNIT0
	) : COLOR{
#if SMAA_PREDICATION == 1
	return SMAALumaEdgeDetectionPS(texcoord, offset, colorGammaTex, predicationTex);
#else
	return SMAALumaEdgeDetectionPS(texcoord, offset, colorGammaTex);
#endif
}

float4 ColorEdgeDetection_PS(float4 position : POSITION,
float2 texcoord : TEXCOORD0,
float4 offset[3] : TEXCOORD1,
#if SMAA_PREDICATION == 1
SMAATexture2D predicationTex : TEXUNIT1,
#endif
uniform SMAATexture2D colorGammaTex : TEXUNIT0) : COLOR{
#if SMAA_PREDICATION == 1
	return SMAAColorEdgeDetectionPS(texcoord, offset, colorGammaTex, predicationTex);
#else
	return SMAAColorEdgeDetectionPS(texcoord, offset, colorGammaTex);
#endif
}

float4 DepthEdgeDetection_PS(float4 position : POSITION,
float2 texcoord : TEXCOORD0,
float4 offset[3] : TEXCOORD1,
uniform SMAATexture2D depthTex : TEXUNIT0) : COLOR{
	return SMAADepthEdgeDetectionPS(texcoord, offset, depthTex);
}

float4 BlendWeightCalculation_PS(float4 position : POSITION,
float2 texcoord : TEXCOORD0,
float2 pixcoord : TEXCOORD1,
float4 offset[3] : TEXCOORD2,
uniform SMAATexture2D edgesTex : TEXUNIT0,
uniform SMAATexture2D areaTex : TEXUNIT1,
uniform SMAATexture2D searchTex : TEXUNIT2) : COLOR{
	return SMAABlendingWeightCalculationPS(texcoord, pixcoord, offset, edgesTex, areaTex, searchTex, 0);
}

float4 NeighborhoodBlending_PS(float4 position : POSITION,
float2 texcoord : TEXCOORD0,
float4 offset[2] : TEXCOORD1,
uniform SMAATexture2D colorTex : TEXUNIT0,
uniform SMAATexture2D blendTex : TEXUNIT1) : COLOR{
	return SMAANeighborhoodBlendingPS(texcoord, offset, colorTex, blendTex);
}
