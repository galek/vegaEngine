float3 expand(float3 v) {
   return (v - 0.5) * 2;
}
void POM_Vert_Multilight(float4 position   : POSITION,
	float3 normal : NORMAL,
	float2 uv : TEXCOORD0,
	float3 tangent : TANGENT0,
	// outputs 
	out float4 oPosition : POSITION,
	out float2 oUv : TEXCOORD0,
	out float3 oEyeDir : TEXCOORD1,
	out float3 oEyeDirWS : TEXCOORD2,
	out float3 oNormal : TEXCOORD3,
	out float3 oAttenuation : TEXCOORD4,
	out float3 oLightDir1 : TEXCOORD5,
	out float3 oLightDir2 : TEXCOORD6,
	out float3 oLightDir3 : TEXCOORD7,
	// parameters 
	uniform float scale,
	uniform float4 lightPosition1,
	uniform float4 lightPosition2,
	uniform float4 lightPosition3,
	uniform float3 eyePosition,
	uniform float4x4 worldViewProj,
	uniform float4x4 world_matrix,
	uniform float4 lightAttenuation1,
	uniform float4 lightAttenuation2,
	uniform float4 lightAttenuation3)
{
	// calculate output position 
	oPosition = mul(worldViewProj, position);

	// pass the main uvs straight through unchanged 
	oUv = uv * scale;

	float Dist1 = length(lightPosition1 - position);
	float Dist2 = length(lightPosition2 - position);
	float Dist3 = length(lightPosition3 - position);
	oAttenuation = float3(1 / (lightAttenuation1.y + lightAttenuation1.z * Dist1 + lightAttenuation1.w * Dist1 * Dist1), 1 / (lightAttenuation2.y + lightAttenuation2.z * Dist2 + lightAttenuation2.w * Dist2 * Dist2), 1 / (lightAttenuation3.y + lightAttenuation3.z * Dist3 + lightAttenuation3.w * Dist3 * Dist3));

	// calculate tangent space light vector 
	// Get object space light direction 
	float3 lightDir1 = lightPosition1.xyz - (position * lightPosition1.w);
		float3 lightDir2 = lightPosition2.xyz - (position * lightPosition2.w);
		float3 lightDir3 = lightPosition3.xyz - (position * lightPosition3.w);

		oEyeDirWS = eyePosition - position.xyz;
	oNormal = normal;

	normal = normalize(normal);
	tangent = normalize(tangent);
	float3 binormal = cross(tangent, normal);

		// Form a rotation matrix out of the vectors 
		float3x3 rotation = float3x3(tangent, binormal, normal);

		// Transform the light vector according to this matrix 
		oLightDir1 = (mul(rotation, lightDir1));
	oLightDir2 = (mul(rotation, lightDir2));
	oLightDir3 = (mul(rotation, lightDir3));
	oEyeDir = (mul(rotation, oEyeDirWS));

}

void POM_Frag_Multilight(
	float2 uv   : TEXCOORD0,
	float3 eyeDir : TEXCOORD1,
	float3 eyeDirWS : TEXCOORD2,
	float3 iNormal : TEXCOORD3,
	float3 Attenuation : TEXCOORD4,
	float3 lightVec1 : TEXCOORD5,
	float3 lightVec2 : TEXCOORD6,
	float3 lightVec3 : TEXCOORD7,

	out float4 oColor : COLOR,

	uniform float4 lightDiffuse1,
	uniform float4 lightDiffuse2,
	uniform float4 lightDiffuse3,
	uniform float4 lightAmbient,
	uniform float4 lightSpecular1,
	uniform float4 lightSpecular2,
	uniform float4 lightSpecular3,
	uniform float spec_exponent,
	uniform float spec_factor,
	uniform float fHeightMapScale,

	uniform sampler2D normalHeightMap : register(s0),
	uniform sampler2D diffuseMap : register(s1)
	)
{
	// Compute the ray direction for intersecting the height field profile with 
	// current view ray. See the above paper for derivation of this computation. (Ati's comment)

	// Compute initial parallax displacement direction: (Ati's comment)
	float2 vParallaxDirection = normalize(eyeDir.xy);

		// The length of this vector determines the furthest amount of displacement: (Ati's comment)
		float fLength = length(eyeDir);
	float fParallaxLength = sqrt(fLength * fLength - eyeDir.z * eyeDir.z) / eyeDir.z;

	// Compute the actual reverse parallax displacement vector: (Ati's comment)
	float2 vParallaxOffsetTS = vParallaxDirection * fParallaxLength;

		// Need to scale the amount of displacement to account for different height ranges
		// in height maps. This is controlled by an artist-editable parameter: (Ati's comment)
		vParallaxOffsetTS *= fHeightMapScale;

	eyeDir = normalize(eyeDir);
	lightVec1 = normalize(lightVec1);
	lightVec2 = normalize(lightVec2);
	lightVec3 = normalize(lightVec3);
	float3 N = normalize(iNormal);
		eyeDirWS = normalize(eyeDirWS);
	float3 halfAngle1 = normalize(eyeDir + lightVec1);
		float3 halfAngle2 = normalize(eyeDir + lightVec2);
		float3 halfAngle3 = normalize(eyeDir + lightVec3);

		float2 newTexCoord = uv;

		float nMinSamples = 6;
	float nMaxSamples = 70;
	int nNumSamples = (int)(lerp(nMinSamples, nMaxSamples, 1 - dot(eyeDirWS, N)));   //In reference shader: int nNumSamples = (int)(lerp( nMinSamples, nMaxSamples, dot( eyeDirWS, N ) ));
	float fStepSize = 1.0 / (float)nNumSamples;
	float fCurrHeight = 0.0;
	float fPrevHeight = 1.0;
	float fNextHeight = 0.0;
	int    nStepIndex = 0;
	float2 vTexOffsetPerStep = fStepSize * vParallaxOffsetTS;
		float2 vTexCurrentOffset = uv;
		float  fCurrentBound = 1.0;
	float  fParallaxAmount = 0.0;

	float2 pt1 = 0;
		float2 pt2 = 0;

	while (nStepIndex < nNumSamples)
	{
		vTexCurrentOffset -= vTexOffsetPerStep;

		// Sample height map which in this case is stored in the alpha channel of the normal map: (Ati's comment)
		fCurrHeight = tex2Dlod(normalHeightMap, float4(vTexCurrentOffset, 0, 0)).a;

		fCurrentBound -= fStepSize;

		if (fCurrHeight > fCurrentBound)
		{
			pt1 = float2(fCurrentBound, fCurrHeight);
			pt2 = float2(fCurrentBound + fStepSize, fPrevHeight);

			nStepIndex = nNumSamples + 1;   //Exit loop
			fPrevHeight = fCurrHeight;
		}
		else
		{
			nStepIndex++;
			fPrevHeight = fCurrHeight;
		}
	}
	float fDelta2 = pt2.x - pt2.y;
	float fDelta1 = pt1.x - pt1.y;

	float fDenominator = fDelta2 - fDelta1;

	// SM 3.0 requires a check for divide by zero, since that operation will generate
	// an 'Inf' number instead of 0, as previous models (conveniently) did: (Ati's comment)
	if (fDenominator == 0.0f)
	{
		fParallaxAmount = 0.0f;
	}
	else
	{
		fParallaxAmount = (pt1.x * fDelta2 - pt2.x * fDelta1) / fDenominator;
	}

	float2 vParallaxOffset = vParallaxOffsetTS * (1 - fParallaxAmount);
		newTexCoord = uv - vParallaxOffset;

	//Do some ordinary Parallax Mapping / Normal Mapping using the new coordinates:

	float3 PixelNormal = expand(tex2D(normalHeightMap, newTexCoord).xyz);
		PixelNormal = normalize(PixelNormal);
	float3 diffuse = tex2D(diffuseMap, newTexCoord).xyz;

		float NdotL = dot(normalize(lightVec1), PixelNormal);
	float NdotH = dot(normalize(halfAngle1), PixelNormal);
	float4 Lit1 = lit(NdotL, NdotH, spec_exponent);
		NdotL = dot(normalize(lightVec2), PixelNormal);
	NdotH = dot(normalize(halfAngle2), PixelNormal);
	float4 Lit2 = lit(NdotL, NdotH, spec_exponent);
		NdotL = dot(normalize(lightVec3), PixelNormal);
	NdotH = dot(normalize(halfAngle3), PixelNormal);
	float4 Lit3 = lit(NdotL, NdotH, spec_exponent);
		float3 AmbientCol = diffuse * lightAmbient;
		float3 DiffuseCol = (Lit1.y * lightDiffuse1 * Attenuation.x) + (Lit2.y * lightDiffuse2 * Attenuation.y) + (Lit3.y * lightDiffuse3 * Attenuation.z);
		float3 SpecularCol = lightSpecular1 * Lit1.z * spec_factor * Attenuation.x + lightSpecular2 * Lit2.z * spec_factor * Attenuation.y + lightSpecular3 * Lit3.z * spec_factor * Attenuation.z;

		oColor = float4(AmbientCol + diffuse * DiffuseCol + diffuse*SpecularCol, 1);
}