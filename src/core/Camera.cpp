#ifndef CAMERA_CPP
#define CAMERA_CPP
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
namespace parseShape
{
#pragma region Constructors and Destructors
    Camera::Camera() : Object()
    {
        fov = 45.0f;
        pNear = 0.1f;
        pFar = 100.0f;
        aspect = 1.0f;

        left = -1.0f;
        right = 1.0f;
        bottom = -1.0f;
        top = 1.0f;
        oNear = 0.1f;
        oFar = 100.0f;

        up = glm::vec3(0, 1, 0);
    }
    Camera::~Camera()
    {
    }
#pragma endregion
#pragma region Getters and Setters
    // En : Returns the view matrix.
    // Tr : Görünüm matrisini döndürür.
    glm::mat4 Camera::GetViewMatrix() const
    {

        return glm::lookAt(position, position + GetFront(), GetUp());
    }
    // En : Returns the perspective matrix.
    // Tr : Perspektif matrisini döndürür.
    glm::mat4 Camera::GetPerspectiveMatrix() const
    {
        return glm::perspective(glm::radians(fov), aspect, pNear, pFar);
    }
    // En : Returns the orthographic matrix.
    // Tr : Ortografik matrisini döndürür.
    glm::mat4 Camera::GetOrthographicMatrix() const
    {
        return glm::ortho(left, right, bottom, top, oNear, oFar);
    }
    // En : Returns the projection matrix.
    // Tr : Projeksiyon matrisini döndürür.
    glm::mat4 Camera::GetProjectionMatrix() const
    {
        switch (type)
        {
        case CameraType::Perspective:
            return GetPerspectiveMatrix();
        case CameraType::Orthographic:
            return GetOrthographicMatrix();
        }
    }
    // En : Returns the model matrix.
    // Tr : Model matrisini döndürür.
    glm::mat4 Camera::GetModelMatrix() const
    {
        return glm::mat4(1.0f);
    }
    // En : Sets the perspective properties.
    // `fov` : Field of view.
    // `aspect` : Aspect ratio.
    // `near` : Near plane, the closest point to the camera.
    // `far` : Far plane, the farthest point to the camera.
    // Tr : Perspektif özelliklerini ayarlar.
    // `fov` : Görüş alanı.
    // `aspect` : en boy oranı.
    // `near` : Yakın düzlem, kameraya en yakın nokta.
    // `far` : Uzak düzlem, kameraya en uzak nokta.
    void Camera::SetPerspective(float fov, float aspect, float near, float far)
    {
        this->fov = fov;
        this->aspect = aspect;
        pNear = near;
        pFar = far;
    }
    // En : Sets the orthographic properties.
    // `left` : Left plane.
    // `right` : Right plane.
    // `bottom` : Bottom plane.
    // `top` : Top plane.
    // `near` : Near plane, the closest point to the camera.
    // `far` : Far plane, the farthest point to the camera.
    // Tr : Ortografik özellikleri ayarlar.
    // `left` : Sol düzlem.
    // `right` : Sağ düzlem.
    // `bottom` : Alt düzlem.
    // `top` : Üst düzlem.
    // `near` : Yakın düzlem, kameraya en yakın nokta.
    // `far` : Uzak düzlem, kameraya en uzak nokta.
    void Camera::SetOrthographic(float left, float right, float bottom, float top, float near, float far)
    {
        this->left = left;
        this->right = right;
        this->bottom = bottom;
        this->top = top;
        oNear = near;
        oFar = far;
    }
    // En : Sets the uniforms of the camera.
    // `shader` : Shader object.
    // `model` : Model matrix uniform name.
    // `view` : View matrix uniform name.
    // `projection` : Projection matrix uniform name.
    // `type` : Camera type.
    // Tr : Kameranın uniformlarını ayarlar.
    // `shader` : Shader nesnesi.
    // `model` : Model matrisi uniform ismi.
    // `view` : Görünüm matrisi uniform ismi.
    // `projection` : Projeksiyon matrisi uniform ismi.
    // `type` : Kamera tipi.
    void Camera::SetUniforms(Shader *shader, const char *model, const char *view, const char *projection, CameraType type)
    {
        shader->Set(model, GetModelMatrix());
        shader->Set(view, GetViewMatrix());
        switch (type)
        {
        case CameraType::Perspective:
            shader->Set(projection, GetPerspectiveMatrix());
            break;
        case CameraType::Orthographic:
            shader->Set(projection, GetOrthographicMatrix());
            break;
        }
    }
    // En : Sets the field of view.
    // `fov` : Field of view.
    // Tr : Görüş alanını ayarlar.
    // `fov` : Görüş alanı.
    void Camera::SetFOV(float fov)
    {
        this->fov = fov;
    }
    // En : Sets the near plane.
    // `near` : Near plane, the closest point to the camera.
    // Tr : Yakın düzlemi ayarlar.
    // `near` : Yakın düzlem, kameraya en yakın nokta.
    void Camera::SetNearPlane(float near)
    {
        pNear = near;
    }
    // En : Sets the far plane.
    // `far` : Far plane, the farthest point to the camera.
    // Tr : Uzak düzlemi ayarlar.
    // `far` : Uzak düzlem, kameraya en uzak nokta.
    void Camera::SetFarPlane(float far)
    {
        pFar = far;
    }
    // En : Sets the near and far planes.
    // `near` : Near plane, the closest point to the camera.
    // `far` : Far plane, the farthest point to the camera.
    // Tr : Yakın ve uzak düzlemleri ayarlar.
    // `near` : Yakın düzlem, kameraya en yakın nokta.
    // `far` : Uzak düzlem, kameraya en uzak nokta.
    void Camera::SetPlanes(float near, float far)
    {
        pNear = near;
        pFar = far;
    }
    // En : Sets the aspect ratio.
    // `aspect` : Aspect ratio.
    // Tr : Ekran oranını ayarlar.
    // `aspect` : Ekran oranı.
    void Camera::SetAspect(float aspect)
    {
        this->aspect = aspect;
    }
    // En : Sets the left plane.
    // `left` : Left plane.
    // Tr : Sol düzlemi ayarlar.
    // `left` : Sol düzlem.
    void Camera::SetLeft(float left)
    {
        this->left = left;
    }
    // En : Sets the right plane.
    // `right` : Right plane.
    // Tr : Sağ düzlemi ayarlar.
    // `right` : Sağ düzlem.
    void Camera::SetRight(float right)
    {
        this->right = right;
    }
    // En : Sets the bottom plane.
    // `bottom` : Bottom plane.
    // Tr : Alt düzlemi ayarlar.
    // `bottom` : Alt düzlem.
    void Camera::SetBottom(float bottom)
    {
        this->bottom = bottom;
    }
    // En : Sets the top plane.
    // `top` : Top plane.
    // Tr : Üst düzlemi ayarlar.
    // `top` : Üst düzlem.
    void Camera::SetTop(float top)
    {
        this->top = top;
    }
    // En : Sets the near plane.
    // `near` : Near plane, the closest point to the camera.
    // Tr : Yakın düzlemi ayarlar.
    // `near` : Yakın düzlem, kameraya en yakın nokta.
    void Camera::SetNear(float near)
    {
        oNear = near;
    }
    // En : Sets the far plane.
    // `far` : Far plane, the farthest point to the camera.
    // Tr : Uzak düzlemi ayarlar.
    // `far` : Uzak düzlem, kameraya en uzak nokta.
    void Camera::SetFar(float far)
    {
        oFar = far;
    }
    // En : Returns the camera type.
    // `type` : Camera type.
    // Tr : Kamera tipini döndürür.
    // `type` : Kamera tipi.
    // Tr : Kamera tipini döndürür.
    void Camera::SetType(CameraType type)
    {
        this->type = type;
    }
    // En : Return camera type.
    // Tr : Kamera tipini döndürür.
    CameraType Camera::GetType() const
    {
        return type;
    }
#pragma endregion
}
#endif