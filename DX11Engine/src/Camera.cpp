#include "Camera.hpp"
#include "EngineTypes.hpp"
#include "Input.hpp"

#define RADIANS 0.0174532925f

float3 Camera::s_up = float3(0.0f, 1.0f, 0.0f);
float3 Camera::s_lookAt = float3(0.0f, 0.0f, 1.0f);

Mat4x4 Camera::s_viewMatrix = DirectX::XMMatrixIdentity();
Mat4x4 Camera::s_projectionMatrix = DirectX::XMMatrixIdentity();
Mat4x4 Camera::s_orthographicMatrix = DirectX::XMMatrixIdentity();

CBDCamera Camera::s_cameraConstantInfo = CBDCamera{};

Camera::Camera(I32 width, I32 height) : m_position(0.0f, 0.0f, -5.0f, 1.0f), m_rotation(0.0f, 0.0f, 0.0f, 1.0f)
{
	m_aspect = static_cast<float>(width) / static_cast<float>(height);

	s_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_aspect, m_nearZ, m_farZ);
	s_orthographicMatrix = DirectX::XMMatrixOrthographicLH(static_cast<float>(width), static_cast<float>(height), m_nearZ, m_farZ);

	s_cameraConstantInfo.cameraPosition = m_position;
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
	if (Input::GetKey(KeyCode::W))
	{
		m_position.z += 15.0f * dt;
	}
	if (Input::GetKey(KeyCode::S))
	{
		m_position.z -= 15.0f * dt;
	}
	if (Input::GetKey(KeyCode::A))
	{
		m_position.x -= 15.0f * dt;
	}
	if (Input::GetKey(KeyCode::D))
	{
		m_position.x += 15.0f * dt;
	}
	if (Input::GetKey(KeyCode::Space))
	{
		m_position.y += 15.0f * dt;
	}
	if (Input::GetKey(KeyCode::Shift))
	{
		m_position.y -= 15.0f * dt;
	}

	float2 mousePos = Input::GetMousePos();

	if (Input::GetMouse(KeyCode::Right))
	{
		if (m_mouseFirst)
		{
			m_prevMousePos = mousePos;
			m_mouseFirst = false;
		}
	}

	Vec4 upV = DirectX::XMLoadFloat3(&s_up);
	Vec4 positionV = DirectX::XMLoadFloat4(&m_position);
	Vec4 lookAtV = DirectX::XMLoadFloat3(&s_lookAt);

	float sensitivity = 0.0f;
	float pitch, yaw, roll;
	
	pitch = m_rotation.x * RADIANS;
	yaw = m_rotation.y * RADIANS;
	roll = m_rotation.z * RADIANS;
	
	Mat4x4 rotationMat = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	upV = DirectX::XMVector3TransformNormal(upV, rotationMat);
	lookAtV = DirectX::XMVector3TransformNormal(lookAtV, rotationMat);

	lookAtV = DirectX::XMVectorAdd(positionV, lookAtV);

	//look at function for left-handed coordinate system
	s_viewMatrix = DirectX::XMMatrixLookAtLH(positionV, lookAtV, upV);

	s_cameraConstantInfo.cameraPosition = m_position;
}

void Camera::OnResize(I32 width, I32 height)
{
	m_aspect = static_cast<float>(width) / static_cast<float>(height);

	s_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_aspect, m_nearZ, m_farZ);
	s_orthographicMatrix = DirectX::XMMatrixOrthographicLH(static_cast<float>(width), static_cast<float>(height), m_nearZ, m_farZ);
}