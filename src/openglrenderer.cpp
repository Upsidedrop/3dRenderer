#include "OpenGLRenderer.hpp"
#include "Shaders.hpp"
#include "Utils.hpp"
#include "Time.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

OpenGLRenderer::OpenGLRenderer()
:vertexArrayObject(0), vertexBufferObject(0), graphicsPipeline(0), indexBufferObject(0), offset(-3), rotation(0), scale(2)
{
    VertexSpecification();
    CreateGraphicsPipeline();
}
void OpenGLRenderer::VertexSpecification(){
    const std::vector<GLfloat> vertexData{
        -0.5, -0.5, 0,
        1.0, 0.0, 0.0,

         0.5, -0.5, 0,
         0.0, 1.0, 0.0,

        -0.5,  0.5, 0,
         0.0, 0.0, 1.0,

         0.5, 0.5, 0,
         0.0, 1.0, 0.0
    };

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, Utils::getSize<GLfloat>(vertexData.size()), vertexData.data(), GL_STATIC_DRAW);

    const std::vector<GLuint> indexBufferData{
        1, 2, 0, 1, 3, 2
    };

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Utils::getSize<GLuint>(indexBufferData.size()), indexBufferData.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Utils::getSize<GLfloat>(6), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Utils::getSize<GLfloat>(6), (GLvoid*)Utils::getSize<GLfloat>(3));

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}
void OpenGLRenderer::CreateGraphicsPipeline(){
    graphicsPipeline = CreateShaderProgram(Shaders::vertexShader, Shaders::fragmentShader);
}
GLuint OpenGLRenderer::CreateShaderProgram(const std::string& p_vertexShader, const std::string& p_fragmentShader){
    GLuint pipeline = glCreateProgram();

    GLuint compiledVertexShader = CompileShader(GL_VERTEX_SHADER, p_vertexShader);
    GLuint compiledFragmentShader = CompileShader(GL_FRAGMENT_SHADER, p_fragmentShader);

    glAttachShader(pipeline, compiledVertexShader);
    glAttachShader(pipeline, compiledFragmentShader);

    glLinkProgram(pipeline);

    glValidateProgram(pipeline);

    glDetachShader(pipeline, compiledVertexShader);
    glDetachShader(pipeline, compiledFragmentShader);

    glDeleteShader(compiledVertexShader);
    glDeleteShader(compiledFragmentShader);

    return pipeline;
}
GLuint OpenGLRenderer::CompileShader(GLuint p_type, const std::string& p_source){
    GLuint shaderObject;
    shaderObject = glCreateShader(p_type);

    const char* c_source = p_source.c_str();
    glShaderSource(shaderObject, 1, &c_source, nullptr);
    glCompileShader(shaderObject);

    logShaderErrors(shaderObject, p_type);

    return shaderObject;
}
void OpenGLRenderer::logShaderErrors(GLuint p_shaderObject, GLuint p_type){
    int result;

    glGetShaderiv(p_shaderObject, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE){
        int length;
        glGetShaderiv(p_shaderObject, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length];
        glGetShaderInfoLog(p_shaderObject, length, &length, errorMessages);

        std::cout << "ERROR: SHADER TYPE " << p_type << " FAILED TO COMPILE.\n";
        delete[] errorMessages;
        glDeleteShader(p_shaderObject);
    }
}
void OpenGLRenderer::PreDraw(int p_screenWidth, int p_screenHeight){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.75, 0.67, 0.55, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(graphicsPipeline);

    rotation += 10 * Time::deltaTime;

    mat4 transformMatrix = translate(mat4(1), vec3(0, 0, offset));
    transformMatrix = rotate<float>(transformMatrix, radians(rotation), vec3(0, 1, 0));
    transformMatrix = glm::scale(transformMatrix, vec3(scale, scale, scale));

    GLint translateLocation = glGetUniformLocation(graphicsPipeline, "u_TranslateMatrix");
    glUniformMatrix4fv(translateLocation, 1, GL_FALSE, &transformMatrix[0][0]);

    mat4 cameraMatrix = cam.getCameraMatrix();

    GLint cameraLocation = glGetUniformLocation(graphicsPipeline, "u_CameraMatrix");
    glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, &cameraMatrix[0][0]);

    mat4 perspectiveMatrix = perspective<float>(radians(45.0), (float)p_screenWidth / (float)p_screenHeight, 0.1, 10);
    GLint perspectiveLocation = glGetUniformLocation(graphicsPipeline, "u_PerspectiveMatrix");
    glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, &perspectiveMatrix[0][0]);
}
void OpenGLRenderer::Draw(){
    glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
Camera* OpenGLRenderer::getCam(){
    return &cam;
}