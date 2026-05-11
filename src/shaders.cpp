#include "Shaders.hpp"

namespace Shaders{
    std::string vertexShader = 
    "#version 410 compatibility\n"

    "layout(location=0) in vec3 position;\n"
    "layout(location=1) in vec3 vertexColors;\n"

    "uniform mat4 u_TranslateMatrix;\n"
    "uniform mat4 u_PerspectiveMatrix;\n"
    "uniform mat4 u_CameraMatrix;\n"

    "out vec3 v_vertexColors;\n"

    "void main()\n"
    "{\n"
    "   v_vertexColors = vertexColors;\n"

    "   vec4 newPosition = u_PerspectiveMatrix * u_CameraMatrix * u_TranslateMatrix * vec4(position, 1);\n"

    "   gl_Position = newPosition;\n"
    "}\n";

    std::string fragmentShader =
    "#version 410 compatibility\n"

    "in vec3 v_vertexColors;\n"

    "out vec4 color;\n"

    "uniform sampler2D u_textureSampler;\n"

    "void main()\n"
    "{\n"
    "   color = texture(u_textureSampler, vec2(v_vertexColors.x, v_vertexColors.y)).rgba;\n"
    "}\n";
}