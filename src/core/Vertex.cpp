#ifndef VERTEX_CPP
#define VERTEX_CPP
#include "Vertex.h"

namespace parseShape
{

    void Vertex::SetData(const VertexData &data, unsigned int index)
    {
        if (std::holds_alternative<float>(data))
        {
            this->data[index] = std::get<float>(data);
        }
        else if (std::holds_alternative<glm::vec2>(data))
        {
            glm::vec2 vec = std::get<glm::vec2>(data);
            this->data[index] = vec.x;
            this->data[index + 1] = vec.y;
        }
        else if (std::holds_alternative<glm::vec3>(data))
        {
            glm::vec3 vec = std::get<glm::vec3>(data);
            this->data[index] = vec.x;
            this->data[index + 1] = vec.y;
            this->data[index + 2] = vec.z;
        }
        else if (std::holds_alternative<glm::vec4>(data))
        {
            glm::vec4 vec = std::get<glm::vec4>(data);
            this->data[index] = vec.x;
            this->data[index + 1] = vec.y;
            this->data[index + 2] = vec.z;
            this->data[index + 3] = vec.w;
        }
    }
    void Vertex::Apply()
    {
        if (!dataPtr)
            return;
        for (int i = 0, j = 0; i < data.size(); i++)
        {
            if (std::holds_alternative<float>(data[i]))
            {
                dataPtr[j++] = std::get<float>(data[i]);
            }
            else if (std::holds_alternative<glm::vec2>(data[i]))
            {
                glm::vec2 vec = std::get<glm::vec2>(data[i]);
                dataPtr[j++] = vec.x;
                dataPtr[j++] = vec.y;
            }
            else if (std::holds_alternative<glm::vec3>(data[i]))
            {
                glm::vec3 vec = std::get<glm::vec3>(data[i]);
                dataPtr[j++] = vec.x;
                dataPtr[j++] = vec.y;
                dataPtr[j++] = vec.z;
            }
            else if (std::holds_alternative<glm::vec4>(data[i]))
            {
                glm::vec4 vec = std::get<glm::vec4>(data[i]);
                dataPtr[j++] = vec.x;
                dataPtr[j++] = vec.y;
                dataPtr[j++] = vec.z;
                dataPtr[j++] = vec.w;
            }
        }
    }
}
#endif