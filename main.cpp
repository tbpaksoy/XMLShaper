#include "src/CoreModule.h"
#include <iostream>
int main()
{
    xmls::Window window("Hello World", 800, 600, glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));

    xmls::Camera camera;
    camera.SetPosition(glm::vec3(0.0f, 1.0f, 3.0f));
    camera.LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    xmls::Shader shader("shaders/mesh.vs", "shaders/mesh.fs");
    camera.SetUniforms(&shader);

    camera.SetPosition(glm::vec3(0.0f, 2.0f, 3.0f));
    xmls::Mesh *mesh = xmls::CreateBox(1.0f, 1.0f, 1.0f);
    for (int i = 0, offset = 0; i < 8; i++, offset += 9)
    {
        mesh->AddVertex(glm::vec3(), offset + 3);
        mesh->AddVertex(glm::vec3(0.2f, 0.1f, 0.7f), offset + 6);
    }
    xmls::Scene scene;
    scene.AddMesh(mesh);
    scene.SetShader(&shader);
    scene.Update();
    shader.Activate();
    window.SetUpdate([&]()
                     { 
                        scene.Draw(); 
                        camera.Translate(glm::vec3(0.0f, 0.0f, -0.1f) * window.GetDeltaTime());
                        camera.SetUniforms(&shader); });

    window.Run();

    return 0;
}