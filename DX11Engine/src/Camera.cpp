#include <algorithm>

#include "Camera.hpp"
#include "EngineTypes.hpp"
#include "Input.hpp"


#define RADIANS 0.0174532925f

Vec4 Camera::s_up = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
float3 Camera::s_lookAt = float3(0.0f, 0.0f, 1.0f);

Mat4x4 Camera::s_viewMatrix = DX::XMMatrixIdentity();
Mat4x4 Camera::s_projectionMatrix = DX::XMMatrixIdentity();
Mat4x4 Camera::s_orthographicMatrix = DX::XMMatrixIdentity();

CBDCamera Camera::s_cameraConstantInfo = CBDCamera{};

Camera::Camera(I32 width, I32 height) : m_position(0.0f, 0.0f, -20.0f), m_rotation(0.0f, 0.0f, 0.0f)
{
	m_aspect = static_cast<float>(width) / static_cast<float>(height);

	s_projectionMatrix = DX::XMMatrixPerspectiveFovLH(m_FOV, m_aspect, m_nearZ, m_farZ);
	s_orthographicMatrix = DX::XMMatrixOrthographicOffCenterLH(0, static_cast<float>(width), static_cast<float>(height), 0, m_nearZ, m_farZ);

	s_cameraConstantInfo.cameraPosition = float4(m_position.x, m_position.y, m_position.z, 1.0f);
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
	float2 mousePos = Input::GetMousePos();

	if (Input::GetMouse(KeyCode::Right))
	{
		if (m_mouseFirst)
		{
			m_prevMousePos = mousePos;
			m_mouseFirst = false;
		}

		float sensitivity = 0.1f;
		float xOffset = sensitivity * (mousePos.x - m_prevMousePos.x);
		float yOffset = sensitivity * (mousePos.y - m_prevMousePos.y);

		m_prevMousePos = mousePos;

		m_rotation.x += xOffset;
		m_rotation.y -= yOffset;

		m_rotation.y = std::clamp(m_rotation.y, -89.0f, 89.0f);
	}
	else m_mouseFirst = true;


	Vec4 positionV = DX::XMLoadFloat3(&m_position);
	Vec4 lookAtV = DX::XMLoadFloat3(&s_lookAt);

	float yaw = m_rotation.x * RADIANS;
	float pitch = m_rotation.y * RADIANS;

	Vec4 forward = DX::XMVector3Normalize(DX::XMVectorSet(cosf(pitch) * sinf(yaw), sinf(pitch), cosf(pitch) * cosf(yaw), 0));

	Vec4 right = DX::XMVector3Normalize(DX::XMVector3Cross(s_up, forward));

	m_cameraFront = DX::XMVector3Normalize(DX::XMVector3Cross(s_up, right));

	Vec4 pos = DX::XMLoadFloat3(&m_position);

	if (Input::GetKey(KeyCode::W))
		pos = DX::XMVectorAdd(pos, DX::XMVectorScale(forward, m_cameraSpeed * dt));
	if (Input::GetKey(KeyCode::S))
		pos = DX::XMVectorSubtract(pos, DX::XMVectorScale(forward, m_cameraSpeed * dt));
	if (Input::GetKey(KeyCode::A))
		pos = DX::XMVectorSubtract(pos, DX::XMVectorScale(right, m_cameraSpeed * dt));
	if (Input::GetKey(KeyCode::D))
		pos = DX::XMVectorAdd(pos, DX::XMVectorScale(right, m_cameraSpeed * dt));
	if (Input::GetKey(KeyCode::Space))
		pos = DX::XMVectorAdd(pos, DX::XMVectorScale(s_up, m_cameraSpeed * dt));
	if (Input::GetKey(KeyCode::Shift))
		pos = DX::XMVectorSubtract(pos, DX::XMVectorScale(s_up, m_cameraSpeed * dt));

	DX::XMStoreFloat3(&m_position, pos);

	Vec4 target = DX::XMVectorAdd(pos, forward);

	//look at function for left-handed coordinate system
	s_viewMatrix = DX::XMMatrixLookAtLH(pos, target, s_up);

	s_cameraConstantInfo.cameraPosition = float4(m_position.x, m_position.y, m_position.z, 1.0f);
}

void Camera::OnResize(I32 width, I32 height)
{
	m_aspect = static_cast<float>(width) / static_cast<float>(height);

	s_projectionMatrix = DX::XMMatrixPerspectiveFovLH(m_FOV, m_aspect, m_nearZ, m_farZ);
	s_orthographicMatrix = DX::XMMatrixOrthographicLH(static_cast<float>(width), static_cast<float>(height), m_nearZ, m_farZ);
}