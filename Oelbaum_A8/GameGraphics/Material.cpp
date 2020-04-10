#include "Material.h"

SimplePixelShader*  Material::getPixel()
{
	return pixel;
}

SimpleVertexShader* Material::getVertex()
{
	return vertex;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::getDiffuseTexture()
{
	return diffuseTexture;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::getSpecularMap()
{
	return specularMap;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState> Material::getSamplerOptions()
{
	return samplerOptions;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::getNormalMap()
{
	return normalMap;
}
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::getSurfaceInput()
{
	return surfaceInput;
}

XMFLOAT4 Material::getTint()
{
	return tint;
}

Material::Material()
{
}


Material::Material(XMFLOAT4 t, SimpleVertexShader* v, SimplePixelShader* p, float spec, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dText, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nMap, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sInput, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampOpt)
{

	tint = t;
	vertex = v;
	pixel = p;
	specInensity = spec;
	diffuseTexture = dText;
	samplerOptions = sampOpt;
	normalMap = nMap;
	isNormal = true;
	surfaceInput = sInput;
}
Material::Material(XMFLOAT4 t, SimpleVertexShader* v, SimplePixelShader* p, float spec, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dText, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nMap, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> specMap, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sInput, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampOpt)
{
	tint = t;
	vertex = v;
	pixel = p;
	specInensity = spec;
	diffuseTexture = dText;
	specularMap = specMap;
	samplerOptions = sampOpt;
	normalMap = nMap;
	isNormal = true;
	surfaceInput = sInput;
}
Material::Material(XMFLOAT4 t, SimpleVertexShader* v, SimplePixelShader* p, float spec, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dText, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sInput, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampOpt)
{

	tint = t;
	vertex = v;
	pixel = p;
	specInensity = spec;
	diffuseTexture = dText;
	samplerOptions = sampOpt;
	normalMap = nullptr;
	isNormal = false;
	surfaceInput = sInput;
}

Material::~Material()
{
}
