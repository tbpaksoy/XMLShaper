#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>

#include "Attribute.h"

namespace parseShape
{
    typedef std::map<std::string, AttributeType> Uniforms;
    class Shader
    {
    private:
        GLuint program;
        std::map<int, std::string> attributeNames;
        std::vector<int> attributes;
        AttributeLocation attributeLocations;
        AttributeTypeName attributeTypes;
        AttributeOffset attributeOffsets;
        Uniforms uniforms;
        void CheckProgramLinkErrors();
        void CheckShaderCompileErrors(GLuint shader);
        unsigned int attributeSize = 0;

    public:
        Shader(const char *vertexPath, const char *fragmentPath);
        ~Shader();
        void Activate();
        void Use();

        void Set(const char *name, float value);
        void Set(const char *name, int value);
        void Set(const char *name, bool value);
        void Set(const char *name, glm::vec2 value);
        void Set(const char *name, glm::vec3 value);
        void Set(const char *name, glm::vec4 value);
        void Set(const char *name, glm::mat2 value);
        void Set(const char *name, glm::mat3 value);
        void Set(const char *name, glm::mat4 value);

        template <typename T>
        T Get(const char *name);
        template <>
        float Get<float>(const char *name);
        template <>
        int Get<int>(const char *name);
        template <>
        bool Get<bool>(const char *name);
        template <>
        glm::vec2 Get<glm::vec2>(const char *name);
        template <>
        glm::vec3 Get<glm::vec3>(const char *name);
        template <>
        glm::vec4 Get<glm::vec4>(const char *name);
        template <>
        glm::mat2 Get<glm::mat2>(const char *name);
        template <>
        glm::mat3 Get<glm::mat3>(const char *name);
        template <>
        glm::mat4 Get<glm::mat4>(const char *name);

        unsigned int
        GetAttributeSize() const;
        Uniforms GetUniforms() const;
        AttributeLocation GetAttributeLocations() const;
        AttributeTypeName GetAttributeTypes() const;
        AttributeOffset GetAttributeOffsets() const;
        std::string GetAttributeName(int location) const;
    };
}
#endif