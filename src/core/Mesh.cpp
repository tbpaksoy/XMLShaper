#ifndef MESH_CPP
#define MESH_CPP

#include <math.h>
#include <GL/glew.h>
#include <iostream>

#include "Mesh.h"

namespace parseShape
{
    Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices)
    {
        this->vertices = vertices;
        this->indices = indices;
    }
    Mesh::Mesh(int vertexCount, int vertexSize)
    {
        this->vertexCount = vertexCount;
        this->vertexSize = vertexSize;
        vertices.resize(vertexCount * vertexSize);
    }
    Mesh::~Mesh()
    {
    }

    void Mesh::ChangeVertex(glm::vec3 value, int index, int offset)
    {
        vertices[index * vertexSize + offset] = value.x;
        vertices[index * vertexSize + offset + 1] = value.y;
        vertices[index * vertexSize + offset + 2] = value.z;
    }
    void Mesh::ChangeVertex(glm::vec2 value, int index, int offset)
    {
        vertices[index * vertexSize + offset] = value.x;
        vertices[index * vertexSize + offset + 1] = value.y;
    }
    void Mesh::ChangeVertex(float value, int index, int offset)
    {
        vertices[index * vertexSize + offset] = value;
    }
    void Mesh::SetIndices(std::vector<unsigned int> indices)
    {
        this->indices = indices;
    }
    void Mesh::DoTransformations(unsigned int begin, unsigned int offset)
    {
        if (offset < 3)
            return;
        for (int i = begin; i < vertices.size(); i += offset)
        {
            glm::vec3 vertex(vertices[i], vertices[i + 1], vertices[i + 2]);
            vertex = glm::translate(glm::mat4(1.0f), position) * glm::vec4(vertex, 1.0f);
            vertex = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(vertex, 1.0f);
            vertex = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(vertex, 1.0f);
            vertex = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(vertex, 1.0f);
            vertex = glm::scale(glm::mat4(1.0f), scale) * glm::vec4(vertex, 1.0f);
            vertices[i] = vertex.x;
            vertices[i + 1] = vertex.y;
            vertices[i + 2] = vertex.z;
        }
    }
    void Mesh::DoTransformations()
    {
        DoTransformations(0, vertexCount);
    }

    int Mesh::GetVertexCount() const
    {
        return vertexCount;
    }
    int Mesh::GetVertexSize() const
    {
        return vertexSize;
    }
    float *Mesh::GetVertices(int &size)
    {
        size = vertices.size();
        float *data = new float[size];
        std::copy(vertices.begin(), vertices.end(), data);
        for (int i = 0; i < vertexCount; i++)
        {
            glm::vec3 vertex(data[i * vertexSize], data[i * vertexSize + 1], data[i * vertexSize + 2]);
            vertex = glm::translate(glm::mat4(1.0f), position) * glm::vec4(vertex, 1.0f);
            vertex = rotation * vertex;
            vertex = glm::scale(glm::mat4(1.0f), scale) * glm::vec4(vertex, 1.0f);
            data[i * vertexSize] = vertex.x;
            data[i * vertexSize + 1] = vertex.y;
            data[i * vertexSize + 2] = vertex.z;
        };
        return data;
    }
    unsigned int *Mesh::GetIndices(int &size)
    {
        size = indices.size();
        return &indices.front();
    }

    Mesh *CreateBox(float width, float height, float depth, int vertexSize)
    {
        Mesh *mesh = new Mesh(8, vertexSize);

        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        float halfDepth = depth / 2.0f;

        mesh->ChangeVertex(glm::vec3(-halfWidth, -halfHeight, -halfDepth), 0, 0);
        mesh->ChangeVertex(glm::vec3(halfWidth, -halfHeight, -halfDepth), 1, 0);
        mesh->ChangeVertex(glm::vec3(halfWidth, halfHeight, -halfDepth), 2, 0);
        mesh->ChangeVertex(glm::vec3(-halfWidth, halfHeight, -halfDepth), 3, 0);
        mesh->ChangeVertex(glm::vec3(-halfWidth, -halfHeight, halfDepth), 4, 0);
        mesh->ChangeVertex(glm::vec3(halfWidth, -halfHeight, halfDepth), 5, 0);
        mesh->ChangeVertex(glm::vec3(halfWidth, halfHeight, halfDepth), 6, 0);
        mesh->ChangeVertex(glm::vec3(-halfWidth, halfHeight, halfDepth), 7, 0);

        mesh->SetIndices({2, 1, 0, 0, 3, 2,
                          6, 5, 1, 1, 2, 6,
                          7, 4, 5, 5, 6, 7,
                          3, 0, 4, 4, 7, 3,
                          6, 2, 3, 3, 7, 6,
                          1, 5, 4, 4, 0, 1});

        return mesh;
    }
    Mesh *CreatePlane(float width, float height, int vertexSize)
    {
        Mesh *mesh = new Mesh(4, vertexSize);

        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;

        mesh->ChangeVertex(glm::vec3(-halfWidth, 0.0f, -halfHeight), 0, 0);
        mesh->ChangeVertex(glm::vec3(halfWidth, 0.0f, -halfHeight), 1, 0);
        mesh->ChangeVertex(glm::vec3(halfWidth, 0.0f, halfHeight), 2, 0);
        mesh->ChangeVertex(glm::vec3(-halfWidth, 0.0f, halfHeight), 3, 0);

        mesh->SetIndices({2, 1, 0, 0, 3, 2});

        return mesh;
    }
    Mesh *CreateCylinder(float radius, float height, int sectorCount, int vertexSize)
    {
        Mesh *mesh = new Mesh(sectorCount * 2, vertexSize);

        float sectorStep = 2 * (22.0f / 7.0f) / sectorCount;

        for (int i = 0; i < sectorCount; i++)
        {
            float sectorAngle = i * sectorStep;
            float x = radius * cos(sectorAngle);
            float z = radius * sin(sectorAngle);

            mesh->ChangeVertex(glm::vec3(x, -height / 2.0f, z), i, 0);
            mesh->ChangeVertex(glm::vec3(x, height / 2.0f, z), i + sectorCount, 0);
        }

        std::vector<unsigned int> indices;
        for (int i = 0; i < sectorCount; i++)
        {
            indices.push_back(i);
            indices.push_back((i + 1) % sectorCount);
            indices.push_back(i + sectorCount);

            indices.push_back(i + sectorCount);
            indices.push_back((i + 1) % sectorCount);
            indices.push_back((i + 1) % sectorCount + sectorCount);
        }
        mesh->SetIndices(indices);

        return mesh;
    }
    Mesh *CreateCone(float radius, float height, int sectorCount, int vertexSize)
    {
        Mesh *mesh = new Mesh(sectorCount + 1, vertexSize);

        float sectorStep = 2 * (22.0f / 7.0f) / sectorCount;

        for (int i = 0; i < sectorCount; i++)
        {
            float sectorAngle = i * sectorStep;
            float x = radius * cos(sectorAngle);
            float z = radius * sin(sectorAngle);

            mesh->ChangeVertex(glm::vec3(x, -height / 2.0f, z), i, 0);
        }
        mesh->ChangeVertex(glm::vec3(0.0f, height / 2.0f, 0.0f), sectorCount, 0);

        std::vector<unsigned int> indices;
        for (int i = 0; i < sectorCount; i++)
        {
            indices.push_back(i);
            indices.push_back(sectorCount);
            indices.push_back((i + 1) % sectorCount);
        }
        mesh->SetIndices(indices);

        return mesh;
    }
}
#endif