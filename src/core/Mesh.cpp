#ifndef MESH_CPP
#define MESH_CPP

#include <math.h>
#include <GL/glew.h>
#include <iostream>
#include <numeric>

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
    Mesh::Mesh(int vertexCount, Shader *shader)
    {
        this->vertexCount = vertexCount;
        this->vertexSize = shader->GetAttributeSize();
        vertices.resize(vertexCount * vertexSize);
        for (auto &it : shader->GetAttributeTypes())
            attributeTypes.insert({it.first, it.second});
        for (auto &it : shader->GetAttributeLocations())
            attributeLocations.insert({it.first, it.second});
        for (auto &it : shader->GetAttributeOffsets())
            attributeOffsets.insert({it.first, it.second});
    }
    Mesh::~Mesh()
    {
    }

    void Mesh::Adapt(Shader *shader)
    {
        if (!shader || shader->GetAttributeSize() != vertexSize)
            return;
        for (auto &it : shader->GetAttributeTypes())
            attributeTypes.insert({it.first, it.second});
        for (auto &it : shader->GetAttributeLocations())
            attributeLocations.insert({it.first, it.second});
        for (auto &it : shader->GetAttributeOffsets())
            attributeOffsets.insert({it.first, it.second});
    }
    // En: Adds a vertex to the mesh.
    // `vertex` is a vector of floats.
    // Tr: Mesh'e bir vertex ekler.
    // `vertex`, float'ların vektörüdür.
    void Mesh::AddVertex(std::vector<float> vertex)
    {
        int sizeDiffrence = vertex.size() % vertexSize;
        vertices.insert(vertices.end(), vertex.begin(), vertex.end() - sizeDiffrence);
        vertexCount = vertices.size() / vertexSize;
    }
    // En: Adds a vertex to the mesh.
    // `vertex` is an array of floats.
    // `size` is the size of the array.
    // Tr: Mesh'e bir vertex ekler.
    // `vertex`, float'ların dizisidir.
    // `size`, dizinin boyutudur.
    void Mesh::AddVertex(float *vertex, int size)
    {
        int sizeDiffrence = size % vertexSize;
        vertices.insert(vertices.end(), vertex, vertex + size - sizeDiffrence);
        vertexCount = vertices.size() / vertexSize;
    }
    // En: Adds indices to the mesh.
    // `indices` is a vector of unsigned integers.
    // Tr: Mesh'e indisler ekler.
    // `indices`, unsigned int'lerin vektörüdür.
    void Mesh::AddIndices(std::vector<unsigned int> indices)
    {
        if (indices.size() % 3 == 0)
            this->indices.insert(this->indices.end(), indices.begin(), indices.end());
    }
    // En: Adds indices to the mesh.
    // `indices` is an array of unsigned integers.
    // `size` is the size of the array.
    // Tr: Mesh'e indisler ekler.
    // `indices`, unsigned int'lerin dizisidir.
    // `size`, dizinin boyutudur.
    void Mesh::AddIndices(unsigned int *indices, int size)
    {
        if (size % 3 == 0)
            this->indices.insert(this->indices.end(), indices, indices + size);
    }
    // En: Adds indices to the mesh.
    // `a`, `b`, and `c` are unsigned integers.
    // Tr: Mesh'e indisler ekler.
    // `a`, `b` ve `c` unsigned int'tir.
    void Mesh::AddIndices(unsigned int a, unsigned int b, unsigned int c)
    {
        indices.push_back(a);
        indices.push_back(b);
        indices.push_back(c);
    }
    // En: Changes the vertex at the specified index and offset with the given value.
    // `value` is a glm::vec3 for 3D vertices.
    // `index` is the index of the vertex.
    // `offset` is the offset of the vertex.
    // Tr: Belirtilen indexteki ve offsetteki vertexi verilen değerle değiştirir.
    // `value`, 3D vertexler için glm::vec3'tür.
    // `index`, vertexin indeksi.
    // `offset`, vertexin offseti.
    void Mesh::ChangeVertex(glm::vec3 value, int index, int offset)
    {
        vertices[index * vertexSize + offset] = value.x;
        vertices[index * vertexSize + offset + 1] = value.y;
        vertices[index * vertexSize + offset + 2] = value.z;
    }
    // En: Changes the vertex at the specified index and offset with the given value.
    // `value` is a glm::vec2 for 2D vertices.
    // `index` is the index of the vertex.
    // `offset` is the offset of the vertex.
    // Tr: Belirtilen indexteki ve offsetteki vertexi verilen değerle değiştirir.
    // `value`, 2D vertexler için glm::vec2'dir.
    // `index`, vertexin indeksi.
    // `offset`, vertexin offseti.
    void Mesh::ChangeVertex(glm::vec2 value, int index, int offset)
    {
        vertices[index * vertexSize + offset] = value.x;
        vertices[index * vertexSize + offset + 1] = value.y;
    }
    // En: Changes the vertex at the specified index and offset with the given value.
    // `value` is a float for 1D vertices.
    // `index` is the index of the vertex.
    // `offset` is the offset of the vertex.
    // Tr: Belirtilen indexteki ve offsetteki vertexi verilen değerle değiştirir.
    // `value`, 1D vertexler için float'tur.
    // `index`, vertexin indeksi.
    // `offset`, vertexin offseti.
    void Mesh::ChangeVertex(float value, int index, int offset)
    {
        vertices[index * vertexSize + offset] = value;
    }
    void Mesh::ChangeVertex(unsigned int index, float value, const char *name)
    {
        if (name && attributeTypes.find(std::string(name)) == attributeTypes.end() && attributeTypes[std::string(name)] != FLOAT)
            return;
        int location = attributeLocations.find(std::string(name)) == attributeLocations.end() ? -1 : attributeLocations[std::string(name)];
        if (location < 0 || location >= vertexSize)
            return;
        ChangeVertex(value, index, location + attributeOffsets[std::string(name)]);
    }
    void Mesh::ChangeVertex(unsigned int index, glm::vec2 value, const char *name)
    {
        if (name && attributeTypes.find(std::string(name)) == attributeTypes.end() && attributeTypes[std::string(name)] != VEC2)
            return;
        int location = attributeLocations.find(std::string(name)) == attributeLocations.end() ? -1 : attributeLocations[std::string(name)];
        if (location < 0 || location >= vertexSize)
            return;
        ChangeVertex(value, index, location + attributeOffsets[std::string(name)]);
    }
    void Mesh::ChangeVertex(unsigned int index, glm::vec3 value, const char *name)
    {
        if (name && attributeTypes.find(std::string(name)) == attributeTypes.end() && attributeTypes[std::string(name)] != VEC3)
            return;
        auto it = attributeLocations.find(name);
        int location = (it == attributeLocations.end()) ? -1 : it->second;
        if (location < 0 || location >= vertexSize)
            return;
        ChangeVertex(value, index, attributeOffsets[std::string(name)]);
    }
    // En: Sets the indices of the triangles of the mesh.
    // `indices` is a vector of unsigned integers.
    // Tr: Meshin üçgenlerinin indislerini ayarlar.
    // `indices`, unsigned int'lerin vektörüdür.
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

    // En: Returns the vertex at the specified index and offset.
    // `index` is the index of the vertex.
    // `offset` is the offset of the vertex.
    // Tr: Belirtilen indexteki ve offsetteki vertexi döndürür.
    // `index`, vertexin indeksi.
    // `offset`, vertexin offseti.

    // En: Returns the number of vertices in the mesh.
    // Tr: Meshin içindeki vertex sayısını döndürür.
    int Mesh::GetVertexCount() const
    {
        return vertexCount;
    }
    // En: Returns the size of the vertices in the mesh.
    // Tr: Meshin içindeki vertexlerin boyutunu döndürür.
    int Mesh::GetVertexSize() const
    {
        return vertexSize;
    }
    // En: Returns the vertices of the mesh.
    // Tr: Meshin vertexlerini döndürür.
    std::vector<float> Mesh::GetVertices() const
    {
        return vertices;
    }
    // En: Returns the vertices of the mesh.
    // `size` is the size of the vertices.
    // Tr: Meshin vertexlerini döndürür.
    // `size`, vertexlerin boyutudur.
    float *Mesh::GetVertices(int &size)
    {
        size = vertices.size();
        float *data = new float[size];
        std::copy(vertices.begin(), vertices.end(), data);

        glm::vec3 center = std::accumulate(vertices.begin(), vertices.end(), glm::vec3(0.0f)) / (float)vertexCount;

        glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
                              glm::scale(glm::mat4(1.0f), scale);

        for (int i = 0; i < vertexCount; i++)
        {
            glm::vec3 vertex(vertices[i * vertexSize], vertices[i * vertexSize + 1], vertices[i * vertexSize + 2]);
            vertex -= center;
            vertex = transform * glm::vec4(vertex, 1.0f);
            vertex += center + position;
            data[i * vertexSize] = vertex.x;
            data[i * vertexSize + 1] = vertex.y;
            data[i * vertexSize + 2] = vertex.z;
        }
        return data;
    }
    // En: Returns the indices of the mesh.
    // `size` is the size of the indices.
    // Tr: Meshin indislerini döndürür.
    // `size`, indislerin boyutudur.
    unsigned int *Mesh::GetIndices(int &size)
    {
        size = indices.size();
        return &indices.front();
    }

    template <>
    float Mesh::GetVertex<float>(unsigned int index, unsigned int offset) const
    {
        return vertices[index * vertexSize + offset];
    }
    template <>
    glm::vec2 Mesh::GetVertex<glm::vec2>(unsigned int index, unsigned int offset) const
    {
        return glm::vec2(vertices[index * vertexSize + offset], vertices[index * vertexSize + offset + 1]);
    }
    template <>
    glm::vec3 Mesh::GetVertex<glm::vec3>(unsigned int index, unsigned int offset) const
    {
        return glm::vec3(vertices[index * vertexSize + offset], vertices[index * vertexSize + offset + 1], vertices[index * vertexSize + offset + 2]);
    }
    template <>
    glm::vec4 Mesh::GetVertex<glm::vec4>(unsigned int index, unsigned int offset) const
    {
        return glm::vec4(vertices[index * vertexSize + offset], vertices[index * vertexSize + offset + 1], vertices[index * vertexSize + offset + 2], vertices[index * vertexSize + offset + 3]);
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
    Mesh *CreateBox(float width, float height, float depth, Shader *shader)
    {
        Mesh *mesh = CreateBox(width, height, depth, shader->GetAttributeSize());
        mesh->Adapt(shader);
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
    Mesh *CreatePlane(float width, float height, Shader *shader)
    {
        Mesh *mesh = CreatePlane(width, height, shader->GetAttributeSize());
        mesh->Adapt(shader);
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
    Mesh *CreateCylinder(float radius, float height, int sectorCount, Shader *shader)
    {
        Mesh *mesh = CreateCylinder(radius, height, sectorCount, shader->GetAttributeSize());
        mesh->Adapt(shader);
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
    Mesh *CreateCone(float radius, float height, int sectorCount, Shader *shader)
    {
        Mesh *mesh = CreateCone(radius, height, sectorCount, shader->GetAttributeSize());
        mesh->Adapt(shader);
        return mesh;
    }
}
#endif