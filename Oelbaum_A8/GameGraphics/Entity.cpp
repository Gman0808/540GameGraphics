#include "Entity.h"
#include "Vertex.h"
#include "DXCore.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include <math.h>
#include "Transform.h"
#include "Vertex.h"

Entity::Entity()
{

}

Entity::Entity(Mesh* mPoint, Transform obj)
{
	mesh = mPoint;
	object = obj;
}

Entity::~Entity()
{
//	DELETE mat;
}

Mesh* Entity::GetMesh()
{
	return mesh;
}

Transform* Entity::GetTransform()
{
	return &object;
}

void Entity::MoveAbsolute(float x, float y, float z)
{
	object.MoveAbsolute(x, y, z);
}

void Entity::MoveRelative(float x, float y, float z)
{
	object.MoveRelative(x, y, z);
}

void Entity::Rotate(float pit, float ya, float ro)
{
	object.Rotate(pit, ya, ro);
}

void Entity::Scale(float x, float y, float z)
{
	object.Scale(x, y, z);
}


void Entity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, XMFLOAT4X4 view, XMFLOAT4X4 proj, XMFLOAT3 playPos, bool isPlayer )
{
	mat->getVertex()->SetFloat4("colorTint", getTint()); //getTint
	mat->getVertex()->SetMatrix4x4("world", object.GetWorldMatrix());
	mat->getVertex()->SetMatrix4x4("view", view);
	mat->getVertex()->SetMatrix4x4("projection", proj);
	
	if (!isPlayer) {
		mat->getPixel()->SetFloat3("playerPos", playPos);
	}
	//mat->getPixel()->SetFloat("isPlayer", isPlayer);

	// Actually copy the data to the GPU
	mat->getVertex()->CopyAllBufferData();

	mat->getPixel()->SetFloat("specIn", mat->specInensity);
	// Set texture resources for the next draw

	mat->getPixel()->SetShaderResourceView("diffuseTexture", mat->getDiffuseTexture().Get());
	mat->getPixel()->SetShaderResourceView("normalMap", mat->getNormalMap().Get());
	mat->getPixel()->SetShaderResourceView("surfTexture", mat->getSurfaceInput().Get());
	mat->getPixel()->SetSamplerState("samplerOptions", mat->getSamplerOptions().Get());


	mat->getPixel()->CopyAllBufferData();

	// Draw the mesh
	mesh->drawShape(context);
}

XMFLOAT4 Entity::getTint()
{
	return mat->getTint();
}


