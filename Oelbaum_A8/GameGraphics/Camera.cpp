#include "Camera.h"
#include <iostream>
using namespace std;

Camera::Camera()
{
}

Camera::Camera(XMFLOAT3 inPos, float asRatio, float mSpeed, float lSpeed, float fClip, float nClip, float fV)
{
	position = inPos;
	movSpeed = mSpeed;
	lookSpeed = lSpeed;
	fView = fV;
	nearClip = nClip;
	farClip = fClip;
	transform = Transform(position, XMFLOAT3(0.3f, 0.0f, 0), XMFLOAT3(1.0f, 1.0f, 1.0f));
	XMStoreFloat4x4(&proMat, XMMatrixIdentity());
	XMStoreFloat4x4(&viewMat, XMMatrixIdentity());

     prevMouse = {};
	UpdateProjectionMatrix(asRatio);
	UpdateViewMatrix();


}

Camera::~Camera()
{
	//delete this;
}

XMFLOAT4X4 Camera::getView()
{
	return viewMat;
}

XMFLOAT4X4  Camera::getProj()
{
	return proMat;
}

Transform Camera::getWorld()
{
	return transform;
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{

//	XMStoreFloat4x4(&proMat, XMMatrixPerspectiveFovLH(fView, aspectRatio, nearClip, farClip));
	
	XMStoreFloat4x4(&proMat, XMMatrixPerspectiveFovLH(fView, aspectRatio, nearClip,farClip));


}


void Camera::UpdateViewMatrix()
{


	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(transform.GetPitchYawRoll().x, transform.GetPitchYawRoll().y, transform.GetPitchYawRoll().z);
	XMVECTOR basicForward = XMVectorSet(0, 0, 1, 0);
	dir = XMVector3Rotate(basicForward, rotation);
	up = XMVectorSet(0, 1, 0, 0);
	// Define the "standard" forward vector w/o rotation (0,0,1)

	XMFLOAT3 trans = transform.GetPosition();
	XMStoreFloat4x4(&viewMat, XMMatrixLookToLH(XMLoadFloat3(&trans), dir, up));
}

void Camera::Update(float dt, HWND windowHandle)
{
	

	if (GetAsyncKeyState('O') & 0x8000) {
		transform.MoveRelative(0,dt * movSpeed, 0);
		cout << "O" << endl;
	}
	if (GetAsyncKeyState('I') & 0x8000) {
		transform.MoveRelative(0,dt * -movSpeed, 0);
		cout << "I" << endl;
	}


	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {

	}

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {

	}
	

	
}

float Camera::getSpeed()
{
	return movSpeed;
}
