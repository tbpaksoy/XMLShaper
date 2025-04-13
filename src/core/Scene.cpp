#ifndef SCENE_CPP
#define SCENE_CPP

#include "Scene.h"
#include <iostream>

namespace parseShape
{
    Scene::Scene()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
    }
    Scene::~Scene()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

    void Scene::SetShader(Shader *shader)
    {
        this->shader = shader;
    }
    Shader *Scene::GetShader() const
    {
        return shader;
    }
    void Scene::SetCamera(Camera *camera)
    {
        this->camera = camera;
    }
    void Scene::UpdateCamera()
    {
        if (camera && shader)
        {
            camera->SetUniforms(shader);
        }
    }
    Camera *Scene::GetCamera() const
    {
        return camera;
    }
    void Scene::AddObject(Object *object)
    {
        objects.push_back(object);
        Mesh *mesh = dynamic_cast<Mesh *>(object);
        if (mesh)
        {
            meshes.push_back(mesh);
        }
    }
    void Scene::AddMesh(Mesh *mesh)
    {
        meshes.push_back(mesh);
    }

    void Scene::Update()
    {
        vertices.clear();
        indices.clear();
        for (auto mesh : meshes)
        {
            int size;
            float *v = mesh->GetVertices(size);
            vertices.insert(vertices.end(), v, v + size);
            unsigned int *i = mesh->GetIndices(size);
            unsigned int offset = indices.size() ? *std::max_element(indices.begin(), indices.end()) + 1 : 0;
            for (int j = 0; j < size; j++)
            {
                indices.push_back(i[j] + offset);
            }
        }

        if (vertices.size() != _vertexSize)
        {
            if (!_vertices)
                delete[] _vertices;
            _vertices = new float[vertices.size()];
            _vertexSize = vertices.size();
        }
        if (indices.size() != _indexSize)
        {
            if (!_indices)
                delete[] _indices;
            _indices = new unsigned int[indices.size()];
            _indexSize = indices.size();
        }
        std::copy(vertices.begin(), vertices.end(), _vertices);
        std::copy(indices.begin(), indices.end(), _indices);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), _vertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), _indices, GL_STATIC_DRAW);

        if (shader != nullptr && camera != nullptr)
        {
            shader->Use();
            camera->SetUniforms(shader);
        }
    }
    void Scene::Draw()
    {
        glBindVertexArray(vao);
        if (shader)
            shader->Use();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
}
#endif