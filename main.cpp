#include "src/CoreModule.h"
#include <iostream>
#include <filesystem>
int main()
{
    xmls::Window window("Hello World", 800, 600, glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    xmls::Shader shader("shaders/mesh.vs", "shaders/mesh.fs");

    int current;
    const char *file = nullptr;
    std::vector<const char *> files;
    for (auto &p : std::filesystem::directory_iterator("data"))
    {
        const char *name = new char[p.path().string().size() + 1];
        std::strcpy(const_cast<char *>(name), p.path().string().c_str());
        files.push_back(name);
    }
    float brightness = 0.0f, ambientStrength = 0.0f;
    xmls::Scene *scene = nullptr;
    window.SetUpdate([&]()
                     {
        ImGui::Begin("Scene");
        if(ImGui::Combo("File", &current, &files[0], files.size())) file = files[current];
            if (file && ImGui::Button("reload"))
            {
                if (scene)
                    delete scene;
                scene = xmls::Parse(file);
                scene->SetShader(&shader);
                scene->Update();
                shader.Activate();
            }
            if(ImGui::SliderFloat("Brightness", &brightness, 0.0f, 3.0f))
                shader.Set("brightness", brightness);
            if(ImGui::SliderFloat("Ambient", &ambientStrength, 0.0f, 1.0f))
                shader.Set("ambientStrength", ambientStrength);

            ImGui::End();
            if (scene)
                scene->Draw(); });

    window.Run();

    return 0;
}