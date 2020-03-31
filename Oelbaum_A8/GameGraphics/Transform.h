#pragma once
#include "Vertex.h"
#include "DXCore.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include <math.h>


using namespace DirectX;


	class Transform
	{
	public:
	    XMFLOAT4X4 worldMatrix;
		XMFLOAT3 position;
		XMFLOAT3 scale;
		XMFLOAT3 rotation;

		XMFLOAT3* posPoint;

		float pitch; //x rotation
		float yaw; // y rotation
		float roll; // z rotation

		Transform();
		Transform(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scal);
		~Transform();

		//setters
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetScale(float x, float y, float z);

		//getters
		XMFLOAT3 GetPosition();
		XMFLOAT3 GetPitchYawRoll();
		XMFLOAT3 GetScale();
		XMFLOAT4X4 GetWorldMatrix();

		//transformers
		void MoveAbsolute(float x, float y, float z);
		void Rotate(float pit, float ya, float ro);
		void RotateCap(float pit, float pit2, float ya, float ya2);
		void RotateCap(float pit, float pit2);
		void Scale(float x, float y, float z);
		void Divide(float x, float y, float z);
		void MoveRelative(float x, float y, float z);

	};

