#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

#include "Logger.hpp"
#include "EngineTypes.hpp"

class Timer
{
public:
	inline Timer() :
		m_frameCount(0), m_framesPerSecond(0), m_deltaTime(0.0f), m_timeElapsed(0.0f)
	{
		QueryPerformanceFrequency(&m_frequency);
		QueryPerformanceCounter(&m_lastTime);
	}

	inline void Tick()
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);

		m_deltaTime = static_cast<float>(currentTime.QuadPart - m_lastTime.QuadPart) / m_frequency.QuadPart;
		m_lastTime = currentTime;

		if (m_deltaTime < 0.0f) m_deltaTime = 0.0f;

		m_frameCount++;
		m_timeElapsed += m_deltaTime;

		if (m_timeElapsed >= 1.0f)
		{
			m_framesPerSecond = m_frameCount;
			m_frameCount = 0;
			m_timeElapsed = 0.0f;
			LOG(std::to_string(m_framesPerSecond) + ":" + std::to_string(m_deltaTime));
		}

	}

	inline float GetDeltaTime() const { return m_deltaTime; }
	inline U32 GetFramesPerSecond() const { return m_framesPerSecond; }

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_lastTime;

	U32 m_frameCount;
	U32 m_framesPerSecond;
	float m_deltaTime;
	float m_timeElapsed;
};