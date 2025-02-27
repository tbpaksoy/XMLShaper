#ifndef MESH_CPP
#define MESH_CPP

#include <math.h>
#include <GL/glew.h>

#include "Mesh.h"

namespace xmls
{
    Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices)
    {
        this->vertices = vertices;
        this->indices = indices;
    }
    Mesh::~Mesh()
    {
    }

    void Mesh::AddVertex(glm::vec3 vertex, int offset)
    {
        if (offset == -1)
        {
            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);
        }
        else
        {
            vertices.insert(vertices.begin() + offset, vertex.x);
            vertices.insert(vertices.begin() + offset + 1, vertex.y);
            vertices.insert(vertices.begin() + offset + 2, vertex.z);
        }
    }
    void Mesh::AddVertex(float x, float y, float z, int offset)
    {
        if (offset == -1)
        {
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        else
        {
            vertices.insert(vertices.begin() + offset, x);
            vertices.insert(vertices.begin() + offset + 1, y);
            vertices.insert(vertices.begin() + offset + 2, z);
        }
    }
    void Mesh::AddVertex(glm::vec2 vertex, int offset)
    {
        if (offset == -1)
        {
            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
        }
        else
        {
            vertices.insert(vertices.begin() + offset, vertex.x);
            vertices.insert(vertices.begin() + offset + 1, vertex.y);
        }
    }
    void Mesh::AddVertex(float x, float y, int offset)
    {
        if (offset == -1)
        {
            vertices.push_back(x);
            vertices.push_back(y);
        }
        else
        {
            vertices.insert(vertices.begin() + offset, x);
            vertices.insert(vertices.begin() + offset + 1, y);
        }
    }
    void Mesh::AddVertex(float vertex, int offset)
    {
        if (offset == -1)
        {
            vertices.push_back(vertex);
        }
        else
        {
            vertices.insert(vertices.begin() + offset, vertex);
        }
    }
    void Mesh::AddVertices(std::vector<float> vertices, int offset)
    {
        if (offset == -1)
        {
            this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());
        }
        else
        {
            this->vertices.insert(this->vertices.begin() + offset, vertices.begin(), vertices.end());
        }
    }
    void Mesh::AddIndex(unsigned int index, int offset)
    {
        if (offset == -1)
        {
            indices.push_back(index);
        }
        else
        {
            indices.insert(indices.begin() + offset, index);
        }
    }
    void Mesh::AddIndices(std::vector<unsigned int> indices, int offset)
    {
        if (offset == -1)
        {
            this->indices.insert(this->indices.end(), indices.begin(), indices.end());
        }
        else
        {
            this->indices.insert(this->indices.begin() + offset, indices.begin(), indices.end());
        }
    }
    void Mesh::SetVertex(int index, glm::vec3 vertex)
    {
        if (vertices.size() < index + 3)
            return;
        vertices[index] = vertex.x;
        vertices[index + 1] = vertex.y;
        vertices[index + 2] = vertex.z;
    }
    void Mesh::SetVertex(int index, float x, float y, float z)
    {
        if (vertices.size() < index + 3)
            return;
        vertices[index] = x;
        vertices[index + 1] = y;
        vertices[index + 2] = z;
    }
    void Mesh::SetVertex(int index, glm::vec2 vertex)
    {
        if (vertices.size() < index + 2)
            return;
        vertices[index] = vertex.x;
        vertices[index + 1] = vertex.y;
    }
    void Mesh::SetVertex(int index, float x, float y)
    {
        if (vertices.size() < index + 2)
            return;
        vertices[index] = x;
        vertices[index + 1] = y;
    }
    void Mesh::SetVertex(int index, float vertex)
    {
        if (vertices.size() < index + 1)
            return;
        vertices[index] = vertex;
    }
    void Mesh::SetVertices(std::vector<float> vertices)
    {
        this->vertices = vertices;
    }
    void Mesh::SetIndex(int index, unsigned int value)
    {
        if (indices.size() < index + 1)
            return;
        indices[index] = value;
    }
    void Mesh::SetIndices(int index, std::vector<unsigned int> indices)
    {
        this->indices = indices;
    }

    float *Mesh::GetVertices(int &size)
    {
        size = vertices.size();
        return &vertices.front();
    }
    unsigned int *Mesh::GetIndices(int &size)
    {
        size = indices.size();
        return &indices.front();
    }

    Mesh *CreateBox(float width, float height, float depth)
    {
        Mesh *mesh = new Mesh();

        mesh->AddVertex(-width / 2, -height / 2, -depth / 2);
        mesh->AddVertex(width / 2, -height / 2, -depth / 2);
        mesh->AddVertex(width / 2, height / 2, -depth / 2);
        mesh->AddVertex(-width / 2, height / 2, -depth / 2);
        mesh->AddVertex(-width / 2, -height / 2, depth / 2);
        mesh->AddVertex(width / 2, -height / 2, depth / 2);
        mesh->AddVertex(width / 2, height / 2, depth / 2);
        mesh->AddVertex(-width / 2, height / 2, depth / 2);

        mesh->AddIndices({0, 1, 2, 2, 3, 0});
        mesh->AddIndices({1, 5, 6, 6, 2, 1});
        mesh->AddIndices({5, 4, 7, 7, 6, 5});
        mesh->AddIndices({4, 0, 3, 3, 7, 4});
        mesh->AddIndices({3, 2, 6, 6, 7, 3});
        mesh->AddIndices({4, 5, 1, 1, 0, 4});

        return mesh;
    }
    Mesh *CreatePlane(float width, float height)
    {
        Mesh *mesh = new Mesh();

        mesh->AddVertex(-width / 2, 0, -height / 2);
        mesh->AddVertex(width / 2, 0, -height / 2);
        mesh->AddVertex(width / 2, 0, height / 2);
        mesh->AddVertex(-width / 2, 0, height / 2);

        mesh->AddIndices({0, 1, 2, 2, 3, 0});

        return mesh;
    }
    Mesh *CreateCylinder(float radius, float height, int sectorCount)
    {
        Mesh *mesh = new Mesh();

        float sectorStep = 2 * (22.0f / 7.0f) / sectorCount;
        float sectorAngle;
        for (int i = 0; i <= sectorCount; i++)
        {
            sectorAngle = i * sectorStep;
            mesh->AddVertex(radius * cos(sectorAngle), -height / 2, radius * sin(sectorAngle));
            mesh->AddVertex(radius * cos(sectorAngle), height / 2, radius * sin(sectorAngle));
        }

        for (unsigned int i = 0; i < sectorCount; i++)
        {
            mesh->AddIndices({i * 2, i * 2 + 1, i * 2 + 3, i * 2 + 3, i * 2 + 2, i * 2});
        }

        return mesh;
    }
    Mesh *CreateCone(float radius, float height, int sectorCount)
    {
        Mesh *mesh = new Mesh();

        float sectorStep = 2 * (22.0f / 7.0f) / sectorCount;
        float sectorAngle;
        for (int i = 0; i <= sectorCount; i++)
        {
            sectorAngle = i * sectorStep;
            mesh->AddVertex(radius * cos(sectorAngle), -height / 2, radius * sin(sectorAngle));
        }
        mesh->AddVertex(0, height / 2, 0);

        for (unsigned int i = 0; i < sectorCount; i++)
        {
            mesh->AddIndices({i, i + 1, (unsigned int)sectorCount});
        }

        return mesh;
    }
}
#endif