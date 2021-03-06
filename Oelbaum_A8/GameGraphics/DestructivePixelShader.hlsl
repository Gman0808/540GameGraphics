#include "IncludeShader.hlsli"



// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage

cbuffer ExternalData : register(b0)
{
	
	float3 pointLightPos;
	float3 pointLightColor;

	float3 cameraPosition;
	float3 Center;
	float specIn;
	bool isPlayer;
	float3 playerPos;

	float gameTime;
	int screenSizeX;
	int screenSizeY;

	directionalLight lData1;
	directionalLight lData2;
	directionalLight lData3;


}

// Texture-related resources
Texture2D diffuseTexture	: register(t0);
Texture2D normalMap			: register(t1);
Texture2D surfTexture		: register(t2); // _mainTex
SamplerState samplerOptions : register(s0);



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

	//Specular
	float3 pointLightDirection = normalize(input.worldPos - pointLightPos);
	float pl_diffuse = Diffuse(input.normal, pointLightDirection);
	float pl_spec = SpecularPhong(input.normal, pointLightDirection, V, 64.0f); // *specMapValue;
	pl_spec *= any(pl_diffuse);

	float3 finalPLColor =
		pl_diffuse * specIn * pointLightColor +
		pl_spec * specIn * pointLightColor;


	//Lights
	float3 light1 = ((Diffuse(input.normal, lData1.direction) * 1.0f * lData1.diffuseColor) + ((SpecularPhong(input.normal, lData1.direction, V, 64.0f) * lData1.diffuseColor) + lData1.ambientColor));
	float3 light2 = ((Diffuse(input.normal, lData2.direction) * 0.1f * lData2.diffuseColor) + ((SpecularPhong(input.normal, lData2.direction, V, 64.0f) * lData2.diffuseColor)));
	float3 light3 = ((Diffuse(input.normal, lData3.direction) * 1.0f * lData3.diffuseColor) + ((SpecularPhong(input.normal, lData3.direction, V, 64.0f) * lData3.diffuseColor)));


	//clipper
	float Radius = 6.0f;//_Radius
	float d = distance(playerPos, input.worldPos);
	float3 surfaceInput = surfTexture.Sample(samplerOptions, input.uv).rgb;
	float dN = 1 - saturate(d / Radius);
	dN = step(0.25, dN) * step(dN, 1.5);
	surfaceInput = surfaceInput * (1 - dN) + half3(1, 1, 1) * dN;

	clip(surfaceInput > 0.99 ? -1 : 1);

	//Inside pattern
	
	//Manhattan distance from center
	float x = abs(input.position.x - (screenSizeX / 2)) + abs(input.position.y - (screenSizeY / 2));
	
	//Pasted with light modifications from https://thebookofshaders.com/13/
	float amplitude = 2;
	float frequency = .5;
	float y = sin(x * frequency);
	float t = (gameTime * -6);
	//Some BS
	y += sin(x * frequency * 1.1 + t);
	y += sin(x * frequency * 1.22 + t * 1.121);
	y += sin(x * frequency * 1.221 + t);
	y += sin(x * frequency * 1.1022 + t * .9269);
	y *= amplitude * 0.06;
	//end paste
	
	//get quantity towards or away from camera, then manipulate into 1 or zero
	float preToggle = dot(V, N);
	float toggle = ((preToggle / abs(preToggle)) + 1) / 2;
	
	//toggle front or back face
	float4 finalColor = float4((light1 + light2 + light3 + finalPLColor) * input.color.xyz * surfaceColor, 1) * toggle + float4(y, 0, 0, 1.0f) * (1 - toggle);


	return finalColor;
}



