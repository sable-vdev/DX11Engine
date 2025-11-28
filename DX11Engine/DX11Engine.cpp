#include "src/Application.hpp"

int main()
{
    Application* app = new Application();

    app->Run();

    delete app;
}