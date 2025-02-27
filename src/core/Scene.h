#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <GL/glew.h>

#include "Mesh.h"
#include "Shader.h"

namespace xmls
{
    class Scene
    {
    private:
        GLuint vao, vbo, ebo;
        std::vector<Mesh *> meshes;
        Shader *shader;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        int vertexSize, indexSize;

        float *_vertices = nullptr;
        unsigned int *_indices = nullptr;

    public:
        Scene();
        ~Scene();

        void SetShader(Shader *shader);
        void AddMesh(Mesh *mesh);
        void Update();
        void Draw();
    };
}

#endif