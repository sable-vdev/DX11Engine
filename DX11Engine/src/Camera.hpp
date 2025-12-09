#pragma once
#include "EngineTypes.hpp"

class Camera
{
public:
	Camera();
	~Camera();

	void Update();

	inline Mat4x4 GetViewMatrix() const { return m_viewMatrix; }
	inline float3 GetPosition() const { return m_position; }
	inline float3 GetRotation() const { return m_rotation; }
private:
	static float3 s_up;
	static float3 s_lookAt;
	float3 m_position = float3(0.0f, 0.0f, -5.0f);
	float3 m_rotation;
	Mat4x4 m_viewMatrix;
};

