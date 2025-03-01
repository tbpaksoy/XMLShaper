#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace xmls
{
    class Object
    {
    protected:
        glm::vec3 position = glm::vec3(), scale = glm::vec3(1.0f), up = glm::vec3(0, 1, 0);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    public:
        Object(glm::vec3 position = glm::vec3(), glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1), glm::vec3 up = glm::vec3(0, 1, 0));
        ~Object();

        // En : Tranform functions
        // Tr : Dönüşüm fonksiyonları

        void Translate(glm::vec3 translation);
        void Rotate(glm::quat rotation);
        void Scale(glm::vec3 scale);
        void LookAt(glm::vec3 target);
        void SetPosition(glm::vec3 position);
        void SetRotation(glm::quat rotation);
        void SetScale(glm::vec3 scale);
        virtual void DoTransformations();

        // En : Getters
        // Tr : Alıcılar

        glm::vec3 GetPosition() const;
        glm::quat GetRotation() const;
        glm::vec3 GetScale() const;
        glm::vec3 GetUp() const;
        glm::vec3 GetFront() const;
        glm::vec3 GetRight() const;
    };
}
#endif