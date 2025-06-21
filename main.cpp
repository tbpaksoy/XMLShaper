#include "src/CoreModule.h"
#include <iostream>
#include <filesystem>
int main()
{
    parseShape::Window window("Hello World", 800, 600, glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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
    parseShape::Scene *scene = nullptr;
    parseShape::Shader *shader = nullptr;
    parseShape::Camera *camera = nullptr;
    window.SetStyle([]()
                    { parseShape::Nuky(); });
    bool playAnimation[5] = {false, false, false, false, false};
    window.SetUpdate([&](float delta)
                     {

        ImGui::Begin("Scene");


        if(playAnimation[0])
        {
            float alpha = std::abs(std::cos(glfwGetTime() * 2.0f)) * 0.5f + 0.5f;
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        }
        if(ImGui::Combo("File", &current, &files[0], files.size())) file = files[current];
        if (playAnimation[0])
        {
            ImGui::PopStyleVar();
        }
        playAnimation[0] = file ? false : !ImGui::IsItemHovered() && ImGui::IsItemFocused();

        
        if(playAnimation[1])
        {
            float alpha = std::abs(std::cos(glfwGetTime() * 2.0f)) * 0.5f + 0.5f;
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        }
        if (file && ImGui::Button("reload"))
        {
            if (scene)
                delete scene;
            if (shader)
                delete shader;
            scene = parseShape::Parse(file);
            if(scene)
            {
                shader = scene->GetShader();
                scene->Update();
                shader->Activate();
                scene->UpdateCamera();
                camera = scene->GetCamera();
            }
        }

        if (playAnimation[1])
        {
            ImGui::PopStyleVar();
        }
        playAnimation[1] = file && !ImGui::IsItemHovered() && !ImGui::IsItemFocused();

        if(shader)
        {
            if(ImGui::SliderFloat("Brightness", &brightness, 0.0f, 3.0f))
                shader->Set("brightness", brightness);
            if(ImGui::SliderFloat("Ambient", &ambientStrength, 0.0f, 1.0f))
                shader->Set("ambientStrength", ambientStrength);
        }

        if(camera && parseShape::GUI<parseShape::Object*>(camera))
            camera->SetUniforms(shader);
        ImGui::End();
        if (scene)
            scene->Draw(); });

    window.Run();

    return 0;
}