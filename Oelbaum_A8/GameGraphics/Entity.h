#pragma once
#include "Vertex.h"
#include "DXCore.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include <math.h>
#include "Transform.h"
#include "Vertex.h"
#include "Material.h"
#include "Camera.h"
#include <wrl/client.h>
#include "SimpleShader.h"

using namespace DirectX;

class Entity
{
public:
	Mesh* mesh;
	Transform object;

	Entity();
	Entity(Mesh* mPoint, Transform obj);
	~Entity();

	Material* mat;
	Mesh* GetMesh();
	Transform* GetTransform();
	void MoveAbsolute(float x, float y, float z);
	void MoveRelative(float x, float y, float z);
	void Rotate(float pit, float ya, float ro);
	void Scale(float x, float y, float z);
	//void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, SimpleVertexShader* vs, SimplePixelShader* ps, Camera* camera); - //freaked out for no reason
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, SimpleVertexShader* vs, SimplePixelShader* ps, XMFLOAT4X4 view, XMFLOAT4X4 proj);

	XMFLOAT4 getTint();
};

