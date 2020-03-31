#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "Mesh.h"
#include "Transform.h"
#include "Entity.h"
#include "Camera.h"
#include "Material.h"
#include "SimpleShader.h"
#include "Lights.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();


	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateBasicGeometry();

	//constantBufferVS
	//Microsoft::WRL::ComPtr<ID3D11Buffer> constantBufferVS;

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Mode, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

		// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	Mesh obj1;
	Mesh obj2;
	Mesh obj3;

	Material mat1;
	Material mat2;
	Material mat3;

	Mesh* gMesh1;
	Camera* cam;

	DirectionalLight lData;
	DirectionalLight lData2;
	DirectionalLight lData3;
	//Microsoft::WRL::ComPtr<Camera> cam;

	// Shaders and shader-related constructs
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;


	Mesh playMesh;
	Entity player;


	Entity gObj1;
	Transform tObj1;
	Transform tObj2;
	Entity gObjList [6];


	// Texture related resources
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> specularMap;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions;

	// Texture related resources
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> specularMap2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap2;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions2;
};

