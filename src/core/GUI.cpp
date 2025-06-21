#ifndef CORE_GUI_CPP
#define CORE_GUI_CPP

#include "GUI.h"

#include <imgui/imgui.h>

namespace parseShape
{
    template <typename T>
    bool GUI(T t)
    {
        return false;
    }

    template <>
    bool GUI<Object *>(Object *object)
    {
        bool result = false;
        if (object)
        {
            glm::vec3 position = object->GetPosition(), scale = object->GetScale();
            glm::quat rotation = object->GetRotation();

            bool pc = ImGui::DragFloat3("Position", &position.x, 0.01f, -100.0f, 100.0f);
            bool sc = ImGui::DragFloat3("Scale", &scale.x, 0.01f, 0.0f, 100.0f);
            bool rc = ImGui::DragFloat4("Rotation", &rotation.x, 0.01f, -1.0f, 1.0f);

            if (pc)
                object->SetPosition(position);
            if (sc)
                object->SetScale(scale);
            if (rc)
                object->SetRotation(rotation);

            result |= pc || sc || rc;
        }
        else
            ImGui::Text("Object is null");

        return result;
    }
    template <>
    bool GUI<Shader *>(Shader *shader)
    {
        if (shader)
        {
            bool result = false;
            for (auto uniform : shader->GetUniforms())
            {

                switch (uniform.second)
                {
                case FLOAT:
                {
                    float value = shader->Get<float>(uniform.first.c_str());
                    if (ImGui::DragFloat(uniform.first.c_str(), &value, 0.01f, -100.0f, 100.0f))
                    {
                        shader->Set(uniform.first.c_str(), value);
                        result |= true;
                    }
                    break;
                }
                case VEC2:
                {
                    glm::vec2 value = shader->Get<glm::vec2>(uniform.first.c_str());
                    if (ImGui::DragFloat2(uniform.first.c_str(), &value.x, 0.01f, -100.0f, 100.0f))
                    {
                        shader->Set(uniform.first.c_str(), value);
                        result |= true;
                    }
                    break;
                }
                case VEC3:
                {
                    glm::vec3 value = shader->Get<glm::vec3>(uniform.first.c_str());
                    if (ImGui::DragFloat3(uniform.first.c_str(), &value.x, 0.01f, -100.0f, 100.0f))
                    {
                        shader->Set(uniform.first.c_str(), value);
                        result |= true;
                    }
                    break;
                }
                case VEC4:
                {
                    glm::vec4 value = shader->Get<glm::vec4>(uniform.first.c_str());
                    if (ImGui::DragFloat4(uniform.first.c_str(), &value.x, 0.01f, -100.0f, 100.0f))
                    {
                        shader->Set(uniform.first.c_str(), value);
                        result |= true;
                    }
                    break;
                }
                }
            }
            return result;
        }
        else
        {
            ImGui::Text("Shader is null");
            return false;
        }
    }
    template <>
    bool GUI<Camera *>(Camera *camera)
    {
        bool result = GUI<Object *>(camera);
        if (camera)
        {
            float fov = camera->GetFOV(), near = camera->GetNearPlane(), far = camera->GetFarPlane(), aspect = camera->GetAspect();
            if (ImGui::DragFloat("FOV", &fov, 0.0f, 100.0f))
            {
                camera->SetFOV(fov);
                result |= true;
            }
            if (ImGui::DragFloat("Near", &near, 0.0f, 100.0f))
            {
                camera->SetNearPlane(near);
                result |= true;
            }
            if (ImGui::DragFloat("Far", &far, 0.0f, 100.0f))
            {
                camera->SetFarPlane(far);
                result |= true;
            }
            if (ImGui::DragFloat("Aspect", &aspect, 0.0f, 100.0f))
            {
                camera->SetAspect(aspect);
                result |= true;
            }
        }
        return result;
    }
    template <>
    bool GUI<Mesh *>(Mesh *mesh)
    {
        bool result = GUI<Object *>(mesh);
        if (mesh)
        {
            unsigned int vertexCount = mesh->GetVertexCount(), vertexSize = mesh->GetVertexSize();
            for (unsigned int i = 0; i < vertexCount; i++)
            {
                float v0 = mesh->GetVertex<float>(i, 0);
                ImGui::SetNextItemWidth(40.0f);
                bool change0 = ImGui::DragFloat(("##" + std::to_string(i * vertexSize)).c_str(), &v0, 0.01f, -100.0f, 100.0f);
                result |= change0;
                if (change0)
                    mesh->ChangeVertex(v0, i, 0);
                for (unsigned int j = 1; j < vertexSize; j++)
                {
                    float v = mesh->GetVertex<float>(i, j);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(40.0f);
                    bool change = ImGui::DragFloat(("##" + std::to_string(i * vertexSize + j)).c_str(), &v, 0.01f, -100.0f, 100.0f);
                    result |= change;
                    if (change)
                        mesh->ChangeVertex(v, i, j);
                }
            }
        }
        return result;
    }

    bool ElementEdit(tinyxml2::XMLElement **element, char *text, size_t length, bool addApplyButton)
    {
        bool result = false;
        result |= ImGui::InputTextMultiline("XML Element", text, 1024 * 16);

        try
        {
            if (addApplyButton && ImGui::Button("Apply"))
            {
                tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
                doc->Parse(text, length);
                *element = doc->RootElement();
                result = true;
            }
        }
        catch (const std::exception &e)
        {
            ImGui::Text("Error: %s", e.what());
            return false;
        }

        return result;
    }
}
#endif