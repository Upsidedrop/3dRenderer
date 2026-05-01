#pragma once

#include <vector>
#include <glad/glad.h>
#include <string>

#include "Camera.hpp"

class OpenGLRenderer{
    public:
    OpenGLRenderer();
    void PreDraw(int p_screenWidth, int p_screenHeight);
    void Draw();
    float offset;
    float rotation;
    float scale;
    Camera* getCam();
    private:
    void VertexSpecification();
    void CreateGraphicsPipeline();
    GLuint CreateShaderProgram(const std::string& p_vertexShader, const std::string& p_fragmentShader);
    GLuint CompileShader(GLuint p_type, const std::string& p_source);
    void logShaderErrors(GLuint p_shaderObject, GLuint p_type);

    GLuint vertexArrayObject;
    GLuint vertexBufferObject;
    GLuint indexBufferObject;
    GLuint graphicsPipeline;
    Camera cam;
};