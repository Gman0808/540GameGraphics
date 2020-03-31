#include "Player.h"
#include <iostream>

using namespace std;

Player::Player()
{
}

Player::Player(Entity* play, Camera* cam)
{
	pEntity = play;
	camera = cam;
	moveSpeed = 3.5f;
	rotSpeed = 3;
	vFoward = 0;

	vRotateX = 0;
	vRotateY = 0;
}

Player::~Player()
{
}

float Player::simulateAcceleration(float velocity, float acceleration, float decceleration , float vLimit, float deltaTime, int pos, int neg)
{
	bool fowardKey = false;
	if (GetAsyncKeyState(pos) & 0x8000) {
		velocity += (acceleration * deltaTime);
		fowardKey = true;
	}
	if (GetAsyncKeyState(neg) & 0x8000) {
		velocity -= (acceleration * deltaTime);
		fowardKey = true;
	}
	if (velocity > vLimit)
		velocity = vLimit;
	else if (velocity < -vLimit)
		velocity = -vLimit;

	if (!fowardKey && velocity > 0)
		velocity -= (decceleration * deltaTime);
	else if (!fowardKey && velocity < 0)
		velocity += (decceleration * deltaTime);

	return velocity;
}
float Player::simulateAcceleration(float velocity, float acceleration, float decceleration, float decLimit, float vLimit, float deltaTime,int pos)
{
	bool fowardKey = false;
	if (GetAsyncKeyState(pos) & 0x8000) {
		velocity += (acceleration * deltaTime);
		fowardKey = true;
	}
	
	if (velocity > vLimit)
		velocity = vLimit;


	if (!fowardKey && velocity > decLimit)
		velocity -= (decceleration * deltaTime);
	else if (!fowardKey && vFoward < decLimit)
		velocity += (decceleration * deltaTime);

	return velocity;
}

void Player::Update(float deltaTime, HWND windowHandle, float width, float height)
{
	

	//space - move forwards
	//adsw - rotate

	vFoward = simulateAcceleration(vFoward,0.1f, 0.05f, 0.05f, 5,  deltaTime, VK_SPACE);
	vRotateX = simulateAcceleration(vRotateX, 0.3f, 1.0f, 1.0f, deltaTime, 'D', 'A');
	vRotateY = simulateAcceleration(vRotateY, 0.3f, 1.0f, 1.0f, deltaTime, 'W', 'S');
	pEntity->object.MoveRelative(0, 0, vFoward);

	pEntity->object.Rotate(vRotateY * rotSpeed * deltaTime,vRotateX * rotSpeed * deltaTime, 0);
	
	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(windowHandle, &mousePos);
	float x = (width / 2 - mousePos.x) * -0.001f;
	float y = (height / 2 - mousePos.y) * -0.001f;


	camera->transform.SetRotation(pEntity->object.pitch + 0.3f + y, pEntity->object.yaw + x, pEntity->object.roll);
	

	camera->transform.SetPosition(pEntity->object.position.x, pEntity->object.position.y, pEntity->object.position.z);
	camera->transform.MoveRelative(0, 0.9f, -4.3f);
	pEntity->object.RotateCap(1.0f, -1.0f);
	
	camera->transform.RotateCap(pEntity->object.pitch + 0.8f, pEntity->object.pitch - 0.8f, pEntity->object.yaw + 0.8f, pEntity->object.yaw - 0.8f);

	camera->UpdateViewMatrix();
}
