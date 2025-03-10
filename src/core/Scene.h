#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <GL/glew.h>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

namespace parseShape
{
    class Scene
    {
    private:
        GLuint vao, vbo, ebo;
        std::vector<Mesh *> meshes;
        Shader *shader;
        Camera *camera;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        int vertexSize, indexSize;

        float *_vertices = nullptr;
        unsigned int *_indices = nullptr;
        int _vertexSize = 0, _indexSize = 0;

    public:
        Scene();
        ~Scene();

        void SetShader(Shader *shader);
        Shader *GetShader() const;
        void SetCamera(Camera *camera);
        Camera *GetCamera() const;
        void AddMesh(Mesh *mesh);
        void AddObject(Object *object);
        void Update();
        void Draw();
    };
}

#endif