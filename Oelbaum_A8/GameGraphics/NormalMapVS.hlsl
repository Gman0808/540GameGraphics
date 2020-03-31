#include "IncludeShader.hlsli"


cbuffer ExternalData : register(b0)
{
	float4 colorTint;
	matrix world;
	matrix view;
	matrix projection;

}

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------

VertexToPixelNormalMap main(VertexShaderInput input)
{
	// Set up output struct
	VertexToPixelNormalMap output;

	// Modifying the position using the provided transformation (world) matrix
	matrix wvp = mul(projection, mul(view, world));
	output.position = mul(wvp, float4(input.position, 1.0f));

	// Calculate the final world position of the vertex
	output.worldPos = mul(world, float4(input.position, 1.0f)).xyz;


	output.normal = mul((float3x3)world, input.normal);
	output.normal = normalize(output.normal);

	// Modify the tangent much like the normal
	output.tangent = mul((float3x3)world, input.tangent);
	output.tangent = normalize(output.tangent);

	output.color = colorTint;
	output.uv = input.uv;

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}

/*
VertexToPixel main(VertexShaderInput input)
{
	// Set up output struct
	VertexToPixel output;

	// Modifying the position using the provided transformation (world) matrix
	matrix wvp = mul(projection, mul(view, world));
	output.position = mul(wvp, float4(input.position, 1.0f));

	// Calculate the final world position of the vertex
	output.worldPos = mul(world, float4(input.position, 1.0f)).xyz;


	output.normal = mul((float3x3)world, input.normal);
	output.normal = normalize(output.normal);

	output.color = colorTint;
	output.uv = input.uv;

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}
*/