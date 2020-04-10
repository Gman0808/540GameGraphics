#include "Vertex.h"
#include "DXCore.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include <math.h>
#include "Transform.h"
#include "Vertex.h"
#include "SimpleShader.h"

#pragma once
class Material
{
public:
	XMFLOAT4 tint;
	float specInensity;
	SimpleVertexShader* vertex;
	SimplePixelShader* pixel;
	bool isNormal;

	// Texture related resources
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> specularMap;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> surfaceInput;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions;


	 SimplePixelShader* getPixel();
	SimpleVertexShader* getVertex();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getDiffuseTexture();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getSpecularMap();
	Microsoft::WRL::ComPtr<ID3D11SamplerState> getSamplerOptions();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getNormalMap();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getSurfaceInput();
	XMFLOAT4 getTint();

	Material();
	Material(XMFLOAT4 t, SimpleVertexShader* v, SimplePixelShader* p, float spec, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dText, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sInput, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampOpt);
	Material(XMFLOAT4 t, SimpleVertexShader* v, SimplePixelShader* p, float spec, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dText, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nMap, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sInput, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampOpt);
	Material(XMFLOAT4 t, SimpleVertexShader* v, SimplePixelShader* p, float spec, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dText, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nMap, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> specMap, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sInput, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampOpt);
	~Material();

};

