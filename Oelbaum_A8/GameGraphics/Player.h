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

class Player
{
	Entity* pEntity;
	Camera* camera;


	float moveSpeed;
	float rotSpeed;


	float vFoward;
	float vRotateX;
	float vRotateY;


	public:
	Player();
	Player(Entity* play, Camera* cam);
	~Player();

	float simulateAcceleration(float velocity,float acceleration, float decceleration, float vLimit, float deltaTime, int pos, int neg);
	float simulateAcceleration(float velocity, float acceleration, float decceleration, float decLimit, float vLimit, float deltaTime, int pos);
	XMFLOAT4X4 getWorldPosition();
	XMFLOAT3  getPosition();
	void Update(float deltaTime, HWND windowHandle, float width, float height);
	

};

