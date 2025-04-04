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

        // En : Sets shader for the scene.
        // Tr : Sahne için shader ayarlar.
        void SetShader(Shader *shader);
        // En : Returns the shader used in the scene.
        // Tr : Sahnede kullanılan shader'ı döner.
        Shader *GetShader() const;
        // En : Sets the camera for the scene.
        // Tr : Sahne için kamera ayarlar.
        void SetCamera(Camera *camera);
        // En : Returns the camera used in the scene.
        // Tr : Sahnede kullanılan kamera'yı döndürür.
        Camera *GetCamera() const;
        // En : Adds a mesh to the scene.
        // Tr : Sahneye bir mesh ekler.
        void AddMesh(Mesh *mesh);
        // En : Adds an object to the scene.
        // Tr : Sahneye bir nesne ekler.
        void AddObject(Object *object);
        // En : Sets the vertices and indices for the scene.
        // Tr : Sahne için vertex ve index ayarlar.
        void Update();
        // En : Draws the scene.
        // Tr : Sahneyi çizer.
        void Draw();
    };
}

#endif