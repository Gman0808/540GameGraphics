
#include "IncludeShader.hlsli"


cbuffer ExternalData : register(b0)
{

	float3 pointLightPos;
	float3 pointLightColor;

	float3 cameraPosition;

	float specIn;

	directionalLight lData1;
	directionalLight lData2;
	directionalLight lData3;
}

// Texture-related resources
Texture2D diffuseTexture	: register(t0);
//Texture2D specularMap		: register(t1);
Texture2D normalMap			: register(t1);
SamplerState samplerOptions : register(s0);


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------

float4 main(VertexToPixelNormalMap input) : SV_TARGET
{
	// Sample the texture to get a color
	 float3 surfaceColor = diffuseTexture.Sample(samplerOptions, input.uv).rgb;

	 // Grab the normal map sample and UNPACK THE NORMAL
	 float3 unpackedNormal = normalMap.Sample(samplerOptions, input.uv).rgb * 2 - 1;

	 // Create the three vectors for normal mapping
	 float3 N = normalize(input.normal);
	 float3 T = normalize(input.tangent);
	 T = normalize(T - N * dot(T, N)); // Gram-Schmidt orthogonalization
	 float3 B = cross(T, N);
	 float3x3 TBN = float3x3(T, B, N);

	 // Adjust and overwrite the existing normal
	 input.normal = normalize(mul(unpackedNormal, TBN));


	 // Calculate the vector from the pixel's world position to the camera
	 float3 V = normalize(cameraPosition - input.worldPos);

	 float3 pointLightDirection = normalize(input.worldPos - pointLightPos);
	 float pl_diffuse = Diffuse(input.normal, pointLightDirection);
	 float pl_spec = SpecularPhong(input.normal, pointLightDirection, V, 64.0f); // *specMapValue;
	 pl_spec *= any(pl_diffuse);

	 float3 finalPLColor =
		 pl_diffuse * specIn * pointLightColor +
		 pl_spec * specIn * pointLightColor;



	 float3 light1 = ((Diffuse(input.normal, lData1.direction) * 1.0f * lData1.diffuseColor) + ((SpecularPhong(input.normal, lData1.direction, V, 64.0f) * lData1.diffuseColor) + lData1.ambientColor));
 float3 light2 = ((Diffuse(input.normal, lData2.direction) * 0.1f * lData2.diffuseColor) + ((SpecularPhong(input.normal, lData2.direction, V, 64.0f) * lData2.diffuseColor)));
	 float3 light3 = ((Diffuse(input.normal, lData3.direction) * 1.0f * lData3.diffuseColor) + ((SpecularPhong(input.normal, lData3.direction, V, 64.0f) * lData3.diffuseColor)));



	 //return Diffuse(input.normal, lData1.direction);
 //	return input.normal;
	//return float4((light1)*input.color * surfaceColor, 1);
	 return float4((light1 + light2 + light3 + finalPLColor) * input.color * surfaceColor, 1);
}


/*
float4 main(VertexToPixel input) : SV_TARGET
{
	// Sample the texture to get a color
	 float3 surfaceColor = diffuseTexture.Sample(samplerOptions, input.uv).rgb;
//float specMapValue = specularMap.Sample(samplerOptions, input.uv).r;

	// Calculate the vector from the pixel's world position to the camera
	float3 V = normalize(cameraPosition - input.worldPos);

	float3 pointLightDirection = normalize(input.worldPos - pointLightPos);
	float pl_diffuse = Diffuse(input.normal, pointLightDirection);
	float pl_spec = SpecularPhong(input.normal, pointLightDirection, V, 64.0f); // *specMapValue;

	float3 finalPLColor =
		pl_diffuse * specIn * pointLightColor +
		pl_spec * specIn * pointLightColor;



	float3 light1 = ((Diffuse(input.normal, lData1.direction) * 1.0f * lData1.diffuseColor) + ((SpecularPhong(input.normal, lData1.direction, V, 64.0f) * lData1.diffuseColor) + lData1.ambientColor ));
float3 light2 = ((Diffuse(input.normal, lData2.direction) * 0.1f * lData2.diffuseColor) + ((SpecularPhong(input.normal, lData2.direction, V, 64.0f)  * lData2.diffuseColor) ));
	float3 light3 = ((Diffuse(input.normal, lData3.direction) * 1.0f * lData3.diffuseColor) + ((SpecularPhong(input.normal, lData3.direction, V, 64.0f) * lData3.diffuseColor)));



	//return Diffuse(input.normal, lData1.direction);
//	return input.normal;

	return float4((light1 + light2 + light3 + finalPLColor) * input.color * surfaceColor, 1);
}
*/

