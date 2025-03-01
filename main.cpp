#include "src/CoreModule.h"
#include <iostream>
#include <filesystem>
int main()
{
    xmls::Window window("Hello World", 800, 600, glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    xmls::Shader shader("shaders/mesh.vs", "shaders/mesh.fs");

    xmls::Scene *scene = nullptr;

    window.SetUpdate([&]()
                     {
                        ImGui::Begin("Scene");
                        for(auto &p: std::filesystem::directory_iterator("data"))
                        {
                            if(ImGui::Button(p.path().filename().string().c_str()))
                            {
                                if(scene) delete scene;
                                scene = xmls::Parse(p.path().string().c_str());
                                scene->SetShader(&shader);
                                scene->Update();
                                shader.Activate();
                            }
                        }

                        ImGui::End(); 
                        if(scene) scene->Draw(); });

    window.Run();

    return 0;
}