#include "src/Window.h"

int main()
{
    xmls::Window window("Hello World", 800, 600, glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
    window.Run();

    return 0;
}