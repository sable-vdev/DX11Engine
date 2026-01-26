#pragma once
#include <iostream>

#define LOG(...) std::cout << __VA_ARGS__ << "\n";
#define LOG_W(...) std::wprintf(__VA_ARGS__); \
					std::cout << "\n";