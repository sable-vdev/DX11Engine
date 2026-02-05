#pragma once
#include "EngineTypes.hpp"
#include "ConstantBufferInfo.hpp"

class Camera
{
public:
	
	Camera(I32 width, I32 height);
	~Camera();

	void Update(float dt);
	void OnResize(I32 width, I32 height);

	static inline Mat4x4 GetViewMatrix() { return s_viewMatrix; }
	static inline Mat4x4 GetProjectionMatrix() { return s_projectionMatrix; }
	static inline Mat4x4 GetOrthographicMatrix() { return s_orthographicMatrix; }

	static inline CBDCamera GetConstantBufferInfo() { return s_cameraConstantInfo; }

	inline float3 GetCameraFront() const
	{
		float3 dest{};
		DX::XMStoreFloat3(&dest, m_cameraFront);

		return dest;
	};
	inline float3 GetPosition() const { return m_position; }
	inline float3 GetRotation() const { return m_rotation; }
	inline float GetFar() const { return m_farZ; }
	inline float GetNear() const { return m_nearZ; }
	inline float GetAspect() const { return m_aspect; }
	inline float GetFOV() const { return m_FOV; }
private:
	static Vec4 s_up;
	static float3 s_lookAt;
	static Mat4x4 s_viewMatrix;
	static Mat4x4 s_projectionMatrix;
	static Mat4x4 s_orthographicMatrix;
	static CBDCamera s_cameraConstantInfo;

	float3 m_position = float3(0.0f, 0.0f, -20.0f);
	float3 m_rotation;
	Vec4 m_cameraRight = Vec4{ 1.0f, 0.0f, 0.0f, 0.0f };
	Vec4 m_cameraFront = Vec4{ 0.0f, 0.0f, -1.0f };
	float2 m_prevMousePos = float2(0.0f, 0.0f);
	float m_FOV = DX::XM_PIDIV4;
	float m_aspect = 16.0f / 9.0f;
	float m_nearZ = 0.1f;
	float m_farZ = 1000.0f;
	float m_cameraSpeed = 15.0f;
	bool m_mouseFirst = true;
};

