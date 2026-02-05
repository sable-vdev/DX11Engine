#include "src/Application.hpp"

//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
int main()
{
    Application* app = new Application();

    app->Run();

    delete app;

    return 0;
}