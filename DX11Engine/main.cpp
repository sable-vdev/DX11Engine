#include "src/Application.hpp"
#include <Windows.h>
/*
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
    }
    catch (...)
    {
        MessageBoxA(nullptr, "Unknown fatal error", "Fatal Error", MB_OK | MB_ICONERROR);
    }

    return 0;
}
*/

#include "src/Application.hpp"
#include <Windows.h>

int GuardedMain()
{
    /*
    // Set working directory to exe location
    char exePath[MAX_PATH];
    GetModuleFileNameA(nullptr, exePath, MAX_PATH);
    std::string dir = std::string(exePath).substr(0, std::string(exePath).find_last_of("\\/"));
    SetCurrentDirectoryA(dir.c_str());
    */
    Application* app = new Application();
    app->Run();
    delete app;
    return 0;
}

int main()
{
    __try
    {
        GuardedMain();
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        DWORD code = GetExceptionCode();
        char msg[64];
        sprintf_s(msg, "Crash! Exception code: 0x%08X", code);
        MessageBoxA(nullptr, msg, "Fatal Error", MB_OK | MB_ICONERROR);
    }

    return 0;
}