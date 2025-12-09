#include "Camera.hpp"
#include "EngineTypes.hpp"
#include "Input.hpp"

#define RADIANS 0.0174532925f

float3 Camera::s_up = float3(0.0f, 1.0f, 0.0f);
float3 Camera::s_lookAt = float3(0.0f, 0.0f, 1.0f);

Camera::Camera() : m_position(0.0f, 0.0f, -5.0f), m_rotation(0.0f, 0.0f, 0.0f), m_viewMatrix()
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (Input::GetKey('W'))
	{
		m_position.z += 0.001f;
	}
	else if (Input::GetKey('S'))
	{
		m_position.z -= 0.001f;
	}
	else if (Input::GetKey('A'))
	{
		m_position.x -= 0.001f;
	}
	else if (Input::GetKey('D'))
	{
		m_position.x += 0.001f;
	}

	Vec4 upV = DirectX::XMLoadFloat3(&s_up);
	Vec4 positionV = DirectX::XMLoadFloat3(&m_position);
	Vec4 lookAtV = DirectX::XMLoadFloat3(&s_lookAt);

	float pitch, yaw, roll;
	
	pitch = m_rotation.x * RADIANS;
	yaw = m_rotation.y * RADIANS;
	roll = m_rotation.z * RADIANS;
	
	Mat4x4 rotationMat = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	upV = DirectX::XMVector3TransformCoord(upV, rotationMat);
	lookAtV = DirectX::XMVector3TransformCoord(lookAtV, rotationMat);

	lookAtV = DirectX::XMVectorAdd(positionV, lookAtV);

	//look at function for left-handed coordinate system
	m_viewMatrix = DirectX::XMMatrixLookAtLH(positionV, lookAtV, upV);
}
