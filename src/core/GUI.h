#ifndef CORE_GUI_H
#define CORE_GUI_H

#include "Object.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

#include <tinyxml2.h>
#include <string>

namespace parseShape
{
    template <typename T>
    bool GUI(T t);

    template <>
    bool GUI<Object *>(Object *object);
    template <>
    bool GUI<Shader *>(Shader *shader);
    template <>
    bool GUI<Camera *>(Camera *camera);
    template <>
    bool GUI<Mesh *>(Mesh *mesh);

    bool ElementEdit(tinyxml2::XMLElement **element, char *text, size_t length = 1024 * 16, bool addApplyButton = true);
}
#endif