#include "src/CoreModule.h"
#include <iostream>
int main()
{
    xmls::Window window("Hello World", 800, 600, glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    xmls::Camera camera;
    camera.SetPosition(glm::vec3(0.0f, 1.0f, 3.0f));
    camera.LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    xmls::Shader shader("shaders/mesh.vs", "shaders/mesh.fs");
    camera.SetUniforms(&shader);
    shader.Set("viewPos", camera.GetPosition());

    camera.SetPosition(glm::vec3(0.0f, 2.0f, 3.0f));

    xmls::Scene *scene = xmls::Parse("data/a.xml");
    scene->SetShader(&shader);
    scene->Update();
    shader.Activate();
    window.SetUpdate([&]()
                     { scene->Draw(); });

    window.Run();

    return 0;
}