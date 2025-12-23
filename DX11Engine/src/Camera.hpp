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

	inline float4 GetPosition() const { return m_position; }
	inline float4 GetRotation() const { return m_rotation; }
private:
	static float3 s_up;
	static float3 s_lookAt;
	static Mat4x4 s_viewMatrix;
	static Mat4x4 s_projectionMatrix;
	static Mat4x4 s_orthographicMatrix;
	static CBDCamera s_cameraConstantInfo;

	float4 m_position = float4(0.0f, 0.0f, -5.0f, 0.0f);
	float4 m_rotation;
	float m_FOV = DirectX::XM_PIDIV4;
	float m_aspect = 16.0f / 9.0f;
	float m_nearZ = 0.1f;
	float m_farZ = 1000.0f;
};

