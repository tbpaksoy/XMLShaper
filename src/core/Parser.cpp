#ifndef PARSER_CPP
#define PARSER_CPP

#include "Parser.h"

#include <map>
#include <functional>
#include <fstream>
#include <algorithm>
#include <string>
#include <iostream>

namespace xmls
{
    static std::map<std::string, std::function<Mesh *(tinyxml2::XMLElement *)>> meshFuncs =
        {{"box", Box}, {"plane", Plane}, {"cylinder", Cylinder}, {"cone", Cone}};

    static std::map<std::string, std::function<void(tinyxml2::XMLElement *, Mesh *)>> meshModifierFuncs =
        {{"color", Color}, {"normal", Normal}};

    static std::map<std::string, std::function<Object *(tinyxml2::XMLElement *)>> objectFuncs;

    static std::map<std::string, std::function<void(tinyxml2::XMLElement *, Object *)>> objectModifierFuncs =
        {{"translate", Translate}, {"rotate", Rotate}, {"scale", Scale}};

    Scene *Parse(tinyxml2::XMLDocument *document)
    {
        return Parse(document->RootElement());
    }
    Scene *Parse(tinyxml2::XMLElement *element)
    {
        Scene *scene = new Scene();

        for (tinyxml2::XMLElement *e = element->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
        {
            std::string typeName(e->Name());
            std::transform(typeName.begin(), typeName.end(), typeName.begin(), [](unsigned char c)
                           { return tolower(c); });
            if (meshFuncs.find(typeName) != meshFuncs.end())
            {
                Mesh *mesh = meshFuncs[typeName](e);
                scene->AddMesh(mesh);
                for (tinyxml2::XMLElement *o = e->FirstChildElement(); o; o = o->NextSiblingElement())
                {
                    std::string objectName(o->Name());
                    std::transform(objectName.begin(), objectName.end(), objectName.begin(), [](unsigned char c)
                                   { return tolower(c); });

                    if (meshModifierFuncs.find(objectName) != meshModifierFuncs.end())
                        meshModifierFuncs[objectName](o, mesh);

                    if (objectModifierFuncs.find(objectName) != objectModifierFuncs.end())
                        objectModifierFuncs[objectName](o, mesh);
                }
            }
        }

        return scene;
    }
    Scene *Parse(std::string path)
    {
        std::ifstream stream(path, std::ios::binary);
        if (!stream.is_open())
            return nullptr;

        std::string content = "", line;
        while (std::getline(stream, line))
            content += line;

        tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
        doc->Parse(content.c_str());
        return Parse(doc);
    }

    Mesh *Box(tinyxml2::XMLElement *element)
    {
        float width = 1.0f, depth = 1.0f, height = 1.0f;
        if (element->Attribute("w"))
            width = element->FloatAttribute("w");
        else if (element->Attribute("x"))
            width = element->FloatAttribute("x");
        else if (element->Attribute("width"))
            width = element->FloatAttribute("width");

        if (element->Attribute("d"))
            depth = element->FloatAttribute("d");
        else if (element->Attribute("z"))
            depth = element->FloatAttribute("z");
        else if (element->Attribute("depth"))
            depth = element->FloatAttribute("depth");

        if (element->Attribute("h"))
            height = element->FloatAttribute("h");
        else if (element->Attribute("y"))
            height = element->FloatAttribute("y");
        else if (element->Attribute("height"))
            height = element->FloatAttribute("height");

        return CreateBox(width, depth, height, 9);
    }
    Mesh *Plane(tinyxml2::XMLElement *element)
    {
        return new Mesh();
    }
    Mesh *Cylinder(tinyxml2::XMLElement *element)
    {
        return new Mesh();
    }
    Mesh *Cone(tinyxml2::XMLElement *element)
    {
        return new Mesh();
    }

    void Color(tinyxml2::XMLElement *element, Mesh *mesh)
    {
        if (mesh == nullptr && std::strcmp(element->Name(), "color") != 0)
            return;

        float r = 1.0f, g = 1.0f, b = 1.0f;
        if (element->Attribute("r"))
            r = element->FloatAttribute("r");
        else if (element->Attribute("red"))
            r = element->FloatAttribute("red");

        if (element->Attribute("g"))
            g = element->FloatAttribute("g");
        else if (element->Attribute("green"))
            g = element->FloatAttribute("green");

        if (element->Attribute("b"))
            b = element->FloatAttribute("b");
        else if (element->Attribute("blue"))
            b = element->FloatAttribute("blue");

        for (int i = 0; i < mesh->GetVertexCount(); i++)
        {
            mesh->ChangeVertex(glm::vec3(r, g, b), i, 6);
        }
    }
    void Normal(tinyxml2::XMLElement *element, Mesh *mesh)
    {
        if (mesh == nullptr && std::strcmp(element->Name(), "normal") != 0)
            return;

        float x = 0.0f, y = 0.0f, z = 0.0f;
        if (element->Attribute("x"))
            x = element->FloatAttribute("x");

        if (element->Attribute("y"))
            y = element->FloatAttribute("y");

        if (element->Attribute("z"))
            z = element->FloatAttribute("z");

        for (int i = 0; i < mesh->GetVertexCount(); i++)
        {
            mesh->ChangeVertex(glm::vec3(x, y, z), i, 3);
        }
    }

    void Translate(tinyxml2::XMLElement *element, Object *object)
    {
        if (object == nullptr && std::strcmp(element->Name(), "translate") != 0)
            return;

        float x = 0.0f, y = 0.0f, z = 0.0f;
        if (element->Attribute("x"))
            x = element->FloatAttribute("x");

        if (element->Attribute("y"))
            y = element->FloatAttribute("y");

        if (element->Attribute("z"))
            z = element->FloatAttribute("z");

        object->Translate(glm::vec3(x, y, z));
    }
    void Rotate(tinyxml2::XMLElement *element, Object *object)
    {
        if (object == nullptr && std::strcmp(element->Name(), "rotate") != 0)
            return;

        float x = 0.0f, y = 0.0f, z = 0.0f;
        if (element->Attribute("x"))
            x = element->FloatAttribute("x");

        if (element->Attribute("y"))
            y = element->FloatAttribute("y");

        if (element->Attribute("z"))
            z = element->FloatAttribute("z");

        object->Rotate(glm::quat(glm::vec3(x, y, z)));
    }
    void Scale(tinyxml2::XMLElement *element, Object *object)
    {
        if (object == nullptr && std::strcmp(element->Name(), "scale") != 0)
            return;

        float x = 1.0f, y = 1.0f, z = 1.0f;
        if (element->Attribute("x"))
            x = element->FloatAttribute("x");

        if (element->Attribute("y"))
            y = element->FloatAttribute("y");

        if (element->Attribute("z"))
            z = element->FloatAttribute("z");

        object->Scale(glm::vec3(x, y, z));
    }
}
#endif