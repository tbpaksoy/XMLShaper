#ifndef WINDOW_H
#define WINDOW_H

#include "Shader.h"
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <vector>
namespace xmls
{
    class Window
    {
    private:
        // En : Window properties
        // Tr : Pencere özellikleri

        GLFWwindow *window;
        unsigned int width, height;
        const char *title;
        std::function<void()> update = []() {};
        std::function<void()> start = []() {};
        float deltaTime = 0.0f;
        glm::vec4 backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

        // En : Render properties
        // Tr : Render özellikleri

        std::function<void()> style = []()
        {
            ImGui::StyleColorsClassic();
        };
        std::vector<Shader *> defaultShaders;

    public:
        // En : Constructor and Destructor
        // Tr : Yapıcı ve Yıkıcı

        Window(const char *title, unsigned int width, unsigned int height);
        Window(const char *title, unsigned int width, unsigned int height, glm::vec4 backgroundColor);
        ~Window();

        // En : Window functions
        // Tr : Pencere fonksiyonları

        const char *GetTitle() const;
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
        GLFWwindow *GetWindow() const;
        float GetDeltaTime() const;
        void Run();

        // En : Render functions
        // Tr : Render fonksiyonları

        void SetUpdate(std::function<void()> update);
        void SetStart(std::function<void()> start);
        void SetStyle(std::function<void()> style);
        void SetDefaultShaders(std::vector<Shader *> shaders);
    };
}
#endif