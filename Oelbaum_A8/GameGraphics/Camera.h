#pragma once
#include<Windows.h>
#include "Transform.h";
#include "Entity.h"
class Camera
{
	
	XMFLOAT4X4 viewMat;
	XMFLOAT4X4 proMat;
	XMFLOAT3 position;
	XMVECTOR pos;
	XMVECTOR up;
	XMVECTOR dir;
	

	POINT prevMouse;
	float fView;
	float nearClip;
	float farClip;
	float movSpeed;
	float lookSpeed;
public:
	Transform transform;
	Camera();
	Camera(XMFLOAT3 inPos, float asRatio, float mSpeed, float lSpeed, float fClip, float nClip, float fV);
	~Camera();
	XMFLOAT4X4  getView();
	XMFLOAT4X4 getProj();
	Transform getWorld();
	 void UpdateProjectionMatrix(float aspectRatio);
	 void UpdateViewMatrix();
	 void Update(float dt, HWND windowHandle);
	 float getSpeed();


};

