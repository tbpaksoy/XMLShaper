#ifndef SHADER_CPP
#define SHADER_CPP

#include <GL/glew.h>
#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace parseShape
{
    static std::map<std::string, GLint> attributesSizes =
        {
            {"bool", 1},
            {"float", 1},
            {"int", 1},
            {"bool", 1},
            {"vec2", 2},
            {"vec3", 3},
            {"vec4", 4},
            {"mat2", 4},
            {"mat3", 9},
            {"mat4", 16}};

    void Shader::CheckProgramLinkErrors()
    {
        GLint success;
        GLchar infoLog[1024];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, 1024, NULL, infoLog);
            std::cerr << "Error: Shader program failed to link\n"
                      << infoLog << std::endl;
        }
    }
    void Shader::CheckShaderCompileErrors(GLuint shader)
    {
        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "Error: Shader failed to compile\n"
                      << infoLog << std::endl;
        }
    }

    // En: Constructor and destructor
    // Tr: Yapıcı ve yıkıcı fonksiyonlar

    Shader::Shader(const char *vertexPath, const char *fragmentPath)
    {
        std::string vertexCode, fragmentCode;
        std::ifstream vertexFile, fragmentFile;

        // Vertex shader

        while (vertexCode.find("  ") != std::string::npos)
            vertexCode.replace(vertexCode.find("  "), 2, " ");

        vertexFile.open(vertexPath);
        for (std::string line; std::getline(vertexFile, line);)
            vertexCode += line + '\n';

        std::stringstream ss(vertexCode);
        for (std::string line; std::getline(ss, line);)
        {
            if (line.find("uniform") != std::string::npos)
            {
                AttributeType type;
                std::string typeName;
                for (auto &it : attributeTable)
                {
                    if (line.find(it.first) != std::string::npos)
                    {
                        type = it.second;
                        typeName = it.first;
                        break;
                    }
                }

                std::string name = line.substr(line.find(type) + typeName.length() + 1, line.find(";") - line.find(type) - typeName.length() - 1);
                uniforms.insert({name, type});
            }
            else if (line.find("in ") != std::string::npos &&
                     (line.find(")in") != std::string::npos || line.find(" in") != std::string::npos) &&
                     line.find("layout") != std::string::npos)
            {
                std::string type;
                AttributeType attributeType;
                int offset = attributeSize;
                for (auto &it : attributeTable)
                {
                    if (line.find(it.first) != std::string::npos)
                    {
                        attributeType = it.second;
                        type = it.first;
                        switch (it.second)
                        {
                        case FLOAT:
                            attributeSize += 1;
                            break;
                        case INT:
                            attributeSize += 1;
                            break;
                        case VEC2:
                            attributeSize += 2;
                            break;
                        case VEC3:
                            attributeSize += 3;
                            break;
                        case VEC4:
                            attributeSize += 4;
                            break;
                        case MAT2:
                            attributeSize += 4;
                            break;
                        case MAT3:
                            attributeSize += 9;
                            break;
                        case MAT4:
                            attributeSize += 16;
                            break;
                        case SAMPLER2D:
                            attributeSize += 1;
                            break;
                        }
                        break;
                    }
                }
                int first = line.find(type) + type.length() + 1,
                    last = line.find(";");
                std::string name = line.substr(first, last - first);
                size_t start = line.find("layout") + 7;
                std::string t;
                bool began = false;
                for (int i = start; i < line.size(); i++)
                {
                    if (!began && std::isdigit(line[i]))
                    {
                        t += line[i];
                        began = true;
                    }
                    else if (began && std::isdigit(line[i]))
                        t += line[i];
                    else if (began && !std::isdigit(line[i]))
                        break;
                }
                if (began)
                {
                    int value = std::stoi(t.c_str());
                    attributes.push_back(value);
                    attributeLocations[name] = value;
                    attributeTypes[name] = attributeType;
                    attributeNames[value] = name;
                    attributeOffsets[name] = offset;
                }
            }
        }

        ss.clear();

        vertexFile.close();

        // Fragment shader

        fragmentFile.open(fragmentPath);
        for (std::string line; std::getline(fragmentFile, line);)
            fragmentCode += line + '\n';

        while (fragmentCode.find("  ") != std::string::npos)
            fragmentCode.replace(fragmentCode.find("  "), 2, " ");

        ss.str(fragmentCode);
        for (std::string line; std::getline(ss, line);)
        {
            if (line.find("uniform") != std::string::npos)
            {
                AttributeType type;
                std::string typeName;
                for (auto &it : attributeTable)
                {
                    if (line.find(it.first) != std::string::npos)
                    {
                        type = it.second;
                        typeName = it.first;
                        break;
                    }
                }

                int start = line.find(typeName) + typeName.length() + 1, end = line.find(";");
                std::string name = line.substr(start, end - start);
                uniforms.insert({name, type});
            }
        }

        fragmentFile.close();

        const char *vertexSource = vertexCode.c_str();
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        CheckShaderCompileErrors(vertexShader);

        const char *fragmentSource = fragmentCode.c_str();
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        CheckShaderCompileErrors(fragmentShader);

        program = glCreateProgram();

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        CheckProgramLinkErrors();

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glUseProgram(program);
    }

    Shader::~Shader()
    {
        glDeleteProgram(program);
        for (auto it : attributeLocations)
        {
            glDisableVertexAttribArray(it.second);
        }
    }

    // En: Activates the shader
    // Tr: Shader'ı aktif hale getirir
    void Shader::Activate()
    {
        glUseProgram(program);
        unsigned int offset = 0;
        for (int location : attributes)
        {
            int size;
            switch (attributeTypes[attributeNames[location]])
            {
            case FLOAT:
                size = 1;
                break;
            case INT:
                size = 1;
                break;
            case VEC2:
                size = 2;
                break;
            case VEC3:
                size = 3;
                break;
            case VEC4:
                size = 4;
                break;
            case MAT2:
                size = 4;
                break;
            case MAT3:
                size = 9;
                break;
            case MAT4:
                size = 16;
                break;
            case SAMPLER2D:
                size = 1;
                break;
            }
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, attributeSize * sizeof(float), (void *)(offset * sizeof(float)));
            attributeOffsets[attributeNames[location]] = offset;
            offset += size;
        }
    }

    // En: Use the shader
    // Tr: Shader'ı kullan
    void Shader::Use()
    {
        glUseProgram(program);
    }

    // En: Set uniform variables
    // `name` is the name of the uniform variable
    // `value` is the value of the uniform variable
    // Tr: Uniform değişkenlerini ayarla
    // `name` uniform değişkenin adıdır
    // `value` uniform değişkenin değeridir
    void Shader::Set(const char *name, float value)
    {
        glUniform1f(glGetUniformLocation(program, name), value);
    }
    // En: Set uniform variables
    // `name` is the name of the uniform variable
    // `value` is the value of the uniform variable
    // Tr: Uniform değişkenlerini ayarla
    // `name` uniform değişkenin adıdır
    // `value` uniform değişkenin değeridir
    void Shader::Set(const char *name, int value)
    {
        glUniform1i(glGetUniformLocation(program, name), value);
    }
    // En: Set uniform variables
    // `name` is the name of the uniform variable
    // `value` is the value of the uniform variable
    // Tr: Uniform değişkenlerini ayarla
    // `name` uniform değişkenin adıdır
    // `value` uniform değişkenin değeridir
    void Shader::Set(const char *name, bool value)
    {
        glUniform1i(glGetUniformLocation(program, name), value);
    }
    // En: Set uniform variables
    // `name` is the name of the uniform variable
    // `value` is the value of the uniform variable
    // Tr: Uniform değişkenlerini ayarla
    // `name` uniform değişkenin adıdır
    // `value` uniform değişkenin değeridir
    void Shader::Set(const char *name, glm::vec2 value)
    {
        glUniform2fv(glGetUniformLocation(program, name), 1, &value[0]);
    }
    // En: Set uniform variables
    // `name` is the name of the uniform variable
    // `value` is the value of the uniform variable
    // Tr: Uniform değişkenlerini ayarla
    // `name` uniform değişkenin adıdır
    // `value` uniform değişkenin değeridir
    void Shader::Set(const char *name, glm::vec3 value)
    {
        glUniform3fv(glGetUniformLocation(program, name), 1, &value[0]);
    }
    // En: Set uniform variables
    // `name` is the name of the uniform variable
    // `value` is the value of the uniform variable
    // Tr: Uniform değişkenlerini ayarla
    // `name` uniform değişkenin adıdır
    // `value` uniform değişkenin değeridir
    void Shader::Set(const char *name, glm::vec4 value)
    {
        glUniform4fv(glGetUniformLocation(program, name), 1, &value[0]);
    }
    // En: Set uniform variables
    // `name` is the name of the uniform variable
    // `value` is the value of the uniform variable
    // Tr: Uniform değişkenlerini ayarla
    // `name` uniform değişkenin adıdır
    // `value` uniform değişkenin değeridir
    void Shader::Set(const char *name, glm::mat2 value)
    {
        glUniformMatrix2fv(glGetUniformLocation(program, name), 1, GL_FALSE, &value[0][0]);
    }
    // En: Set uniform variables
    // `name` is the name of the uniform variable
    // `value` is the value of the uniform variable
    // Tr: Uniform değişkenlerini ayarla
    // `name` uniform değişkenin adıdır
    // `value` uniform değişkenin değeridir
    void Shader::Set(const char *name, glm::mat3 value)
    {
        glUniformMatrix3fv(glGetUniformLocation(program, name), 1, GL_FALSE, &value[0][0]);
    }
    // En: Set uniform variables
    // `name` is the name of the uniform variable
    // `value` is the value of the uniform variable
    // Tr: Uniform değişkenlerini ayarla
    // `name` uniform değişkenin adıdır
    // `value` uniform değişkenin değeridir
    void Shader::Set(const char *name, glm::mat4 value)
    {
        glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, &value[0][0]);
    }

    template <typename T>
    T Shader::Get(const char *name)
    {
        return T();
    }
    template <>
    float Shader::Get<float>(const char *name)
    {
        float value;
        glGetUniformfv(program, glGetUniformLocation(program, name), &value);
        return value;
    }
    template <>
    int Shader::Get<int>(const char *name)
    {
        int value;
        glGetUniformiv(program, glGetUniformLocation(program, name), &value);
        return value;
    }
    template <>
    bool Shader::Get<bool>(const char *name)
    {
        int value;
        glGetUniformiv(program, glGetUniformLocation(program, name), &value);
        return value != 0;
    }
    template <>
    glm::vec2 Shader::Get<glm::vec2>(const char *name)
    {
        glm::vec2 value;
        glGetUniformfv(program, glGetUniformLocation(program, name), &value[0]);
        return value;
    }
    template <>
    glm::vec3 Shader::Get<glm::vec3>(const char *name)
    {
        glm::vec3 value;
        glGetUniformfv(program, glGetUniformLocation(program, name), &value[0]);
        return value;
    }
    template <>
    glm::vec4 Shader::Get<glm::vec4>(const char *name)
    {
        glm::vec4 value;
        glGetUniformfv(program, glGetUniformLocation(program, name), &value[0]);
        return value;
    }
    template <>
    glm::mat2 Shader::Get<glm::mat2>(const char *name)
    {
        glm::mat2 value;
        glGetUniformfv(program, glGetUniformLocation(program, name), &value[0][0]);
        return value;
    }
    template <>
    glm::mat3 Shader::Get<glm::mat3>(const char *name)
    {
        glm::mat3 value;
        glGetUniformfv(program, glGetUniformLocation(program, name), &value[0][0]);
        return value;
    }
    template <>
    glm::mat4 Shader::Get<glm::mat4>(const char *name)
    {
        glm::mat4 value;
        glGetUniformfv(program, glGetUniformLocation(program, name), &value[0][0]);
        return value;
    }

    unsigned int Shader::GetAttributeSize() const
    {
        return attributeSize;
    }
    Uniforms Shader::GetUniforms() const
    {
        return uniforms;
    }
    AttributeLocation Shader::GetAttributeLocations() const
    {
        return attributeLocations;
    }
    AttributeTypeName Shader::GetAttributeTypes() const
    {
        return attributeTypes;
    }
    AttributeOffset Shader::GetAttributeOffsets() const
    {
        return attributeOffsets;
    }
}
#endif