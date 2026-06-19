#pragma once
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


class Logger
{
public:
	static void Init()
	{
		spdlog::set_pattern("%^[%d-%m-%Y %H:%M:%S]%! (%#)[%l][%n] %v%$");

		s_Logger = spdlog::stdout_color_mt("Engine");
		s_Logger->set_level(spdlog::level::trace);
	}

	static std::string WideToNarrow(const std::wstring& wide)
	{
		if (wide.empty()) return {};
		int size = WideCharToMultiByte(CP_UTF8, 0, wide.data(), (int)wide.size(), nullptr, 0, nullptr, nullptr);
		std::string result(size, 0);
		WideCharToMultiByte(CP_UTF8, 0, wide.data(), (int)wide.size(), result.data(), size, nullptr, nullptr);
		return result;
	}

	static std::shared_ptr<spdlog::logger> GetLogger() { return s_Logger; };
private:
	static std::shared_ptr<spdlog::logger> s_Logger;
};

#ifdef NDEBUG
#define LOG_INFO(...) ((void)0)
#define LOG_DEBUG(...) ((void)0)
#define LOG_WARNING(...) Logger::GetLogger()->warning(__VA_ARGS__)
#define LOG_ERROR(...) Logger::GetLogger()->error(__VA_ARGS__)

#else

#define LOG_INFO(...) Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_DEBUG(...) Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_WARNING(...) Logger::GetLogger()->warning(__VA_ARGS__)
#define LOG_ERROR(...) Logger::GetLogger()->error(__VA_ARGS__)
#endif