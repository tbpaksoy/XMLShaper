#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Object.h"
#include "Shader.h"
namespace parseShape
{
    enum class CameraType
    {
        Perspective,
        Orthographic
    };

    class Camera : public Object
    {
    private:
        // En : Camera properties (perspective).
        // Tr : Kamera özellikleri (perspektif).

        float fov, pNear, pFar, aspect;

        // En : Camera properties (orthographic).
        // Tr : Kamera özellikleri (ortografik).

        float left, right, bottom, top, oNear, oFar;

        CameraType type;

    public:
        Camera();
        ~Camera();

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetPerspectiveMatrix() const;
        glm::mat4 GetOrthographicMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
        glm::mat4 GetModelMatrix() const;
        void SetPerspective(float fov, float aspect, float near, float far);
        void SetOrthographic(float left, float right, float bottom, float top, float near, float far);
        void SetUniforms(Shader *shader, const char *model = "model", const char *view = "view", const char *projection = "projection", CameraType type = CameraType::Perspective);
        void SetFOV(float fov);
        void SetNearPlane(float near);
        void SetFarPlane(float far);
        void SetPlanes(float near, float far);
        void SetAspect(float aspect);
        void SetLeft(float left);
        void SetRight(float right);
        void SetBottom(float bottom);
        void SetTop(float top);
        void SetNear(float near);
        void SetFar(float far);
        void SetType(CameraType type);
        CameraType GetType() const;
    };
}
#endif