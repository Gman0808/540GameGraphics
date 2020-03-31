#ifndef __GGP_SHADER_INCLUDES__
#define __GGP_SHADER_INCLUDES__


struct directionalLight {
	float3 ambientColor;
	float3 diffuseColor;
	float3 direction;

};

// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;     // XYZ position
	float2 uv			: TEXCOORD;
	float3 normal       : NORMAL;
	float3 tangent		: TANGENT;
};

// Struct representing the data we're sending down the pipeline
// - Should match our pixel shader's input (hence the name: Vertex to Pixel)
// - At a minimum, we need a piece of data defined tagged as SV_POSITION
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR;        // RGBA color
	float2 uv			: TEXCOORD;
	float3 normal       : NORMAL;
	float3 worldPos		: POSITION;

};

struct VertexToPixelNormalMap
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR;        // RGBA color
	float2 uv			: TEXCOORD;
	float3 normal       : NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;

};


// Calculates the diffuse (Lambert) light amount, given a
// surface normal and a direction FROM the light
// Note: This function will "reverse" the light direction
//       so we're comparing the direction back TO the light
float Diffuse(float3 normal, float3 dir)
{

	return saturate(dot(normalize(normal), -normalize(dir)));
}

float SpecularPhong(float3 normal, float3 lightDir, float3 V, float specExp)
{
	// Calculate light reflection vector
	float3 R = reflect(normalize(lightDir), normal);

	return pow(saturate(dot(R, V)), specExp);
}


#endif