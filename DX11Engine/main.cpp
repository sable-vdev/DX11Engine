#include "src/Application.hpp"
#include <Windows.h>

int main()
{
    try
    {
        Application* app = new Application();
        app->Run();
        delete app;
    }
    catch (const std::exception& e)
    {
        MessageBoxA(nullptr, e.what(), "Fatal Error", MB_OK | MB_ICONERROR);
        LOG_ERROR(e.what());
    }
    catch (...)
    {
        MessageBoxA(nullptr, "Unknown fatal error", "Fatal Error", MB_OK | MB_ICONERROR);
    }

    return 0;
}
