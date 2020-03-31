#include "Transform.h"
#include <iostream>
using namespace std;
	Transform::Transform()
	{
		pitch = 1;
		roll = 1;
		yaw = 1;

		position = XMFLOAT3(1, 1, 1);
		scale = XMFLOAT3(1, 1, 1);
		rotation = XMFLOAT3(1, 1, 1);
		XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	}

	Transform::Transform(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scal)
	{
	

		position = pos;
		scale = scal;
		rotation = rot;

		pitch = rotation.x;
		yaw = rotation.y;
		roll = rotation.z;
		

		XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	}

	Transform::~Transform()
	{

	}

	
	void Transform::SetPosition(float x, float y, float z)
	{
		position = XMFLOAT3(x, y, z);
	}
	void Transform::SetRotation(float x, float y, float z)
	{
		pitch = x;
		yaw = y;
		roll = z;
		rotation = XMFLOAT3(pitch, yaw, roll);

	}
	void Transform::SetScale(float x, float y, float z)
	{
		scale = XMFLOAT3(x, y, z);
	}

	XMFLOAT3 Transform::GetPosition()
	{
		return position;
	}

	XMFLOAT3 Transform::GetPitchYawRoll()
	{
		return rotation;
	}

	XMFLOAT3 Transform::GetScale()
	{
		return scale;
	}

	XMFLOAT4X4 Transform::GetWorldMatrix()
	{
		XMMATRIX trans = XMMatrixTranslation(position.x, position.y, position.z);
		XMMATRIX size = XMMatrixScaling(scale.x, scale.y, scale.z);
		XMMATRIX rot = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

		XMMATRIX world = size * rot * trans;
	

		XMStoreFloat4x4(&worldMatrix, world);

	
		return worldMatrix; 
	}

	void Transform::MoveAbsolute(float x, float y, float z)
	{
		position = XMFLOAT3(position.x + x, position.y + y, position.z + z);
	}

	void Transform::Rotate(float pit, float ya, float ro)
	{
		pitch += pit;
		yaw += ya;
		roll += ro;
		rotation = XMFLOAT3(pitch, yaw, roll);
	}

	void Transform::Scale(float x, float y, float z)
	{
		scale = XMFLOAT3(scale.x * x, scale.y * y, scale.z * z);
		//scale = XMFLOAT3(scale.x + x, scale.y + y, scale.z + z);
	}

	void Transform::Divide(float x, float y, float z)
	{
		scale = XMFLOAT3(scale.x/ x, scale.y/y, scale.z/ z);
	}

	void Transform::MoveRelative(float x, float y, float z)
	{
		//float tempZ = position.z;
		XMVECTOR prevPos = XMVectorSet(position.x, position.y, position.z, 0);
		XMVECTOR basicForward = XMVectorSet(x, y, z, 0);
		XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
		XMVECTOR rotPosition = XMVector3Rotate(basicForward, rotation) + prevPos;
		posPoint = &position;


		XMLoadFloat3(posPoint);
		XMStoreFloat3(posPoint, rotPosition);
		//SetPosition(position.x, position.y, tempZ);
		//XMLoadFloat3
		//XMStoreFloat3
	}
	

