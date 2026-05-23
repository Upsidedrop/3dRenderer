#include "OpenGLRenderer.hpp"
#include "Shaders.hpp"
#include "Utils.hpp"
#include "Time.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL_surface.h>

using namespace glm;

const int INDICES_PER_POINT = 6;

OpenGLRenderer::OpenGLRenderer()
:vertexArrayObject(0), vertexBufferObject(0), graphicsPipeline(0), indexBufferObject(0), offset(-3), rotation(0), scale(2), vertexDataQueue(nullptr), drawCallSize(0)
{
    // VertexSpecification();
    CreateGraphicsPipeline();
}
void OpenGLRenderer::VertexSpecification(){
    // std::vector<GLfloat> vertexData{
    //     -0.5, -0.5, 0,
    //     1.0, 0.0, 0.0,

    //      0.5, -0.5, 0,
    //      0.0, 1.0, 0.0,

    //     -0.5,  0.5, 0,
    //      0.0, 0.0, 1.0,

    //      0.5, 0.5, 0,
    //      0.0, 1.0, 0.0
    // };

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, Utils::getSize<GLfloat>(vertexDataQueue -> size()), vertexDataQueue -> data(), GL_STATIC_DRAW);

    delete vertexDataQueue;

    // const std::vector<GLuint> indexBufferData{
    //     1, 2, 0, 1, 3, 2
    // };

    // glGenBuffers(1, &indexBufferObject);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, Utils::getSize<GLuint>(indexBufferData.size()), indexBufferData.data(), GL_STATIC_DRAW);

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

        std::cout << errorMessages << "\n";

        delete[] errorMessages;
        glDeleteShader(p_shaderObject);
    }
}
void OpenGLRenderer::PreDraw(int p_screenWidth, int p_screenHeight){
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.75, 0.67, 0.55, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(graphicsPipeline);
    
    glBindVertexArray(vertexArrayObject);

    rotation += 10 * Time::deltaTime;

    // mat4 transformMatrix = translate(mat4(1), vec3(0, 0, offset));
    // transformMatrix = rotate<float>(transformMatrix, radians(rotation), vec3(0, 1, 0));
    // transformMatrix = glm::scale(transformMatrix, vec3(scale, scale, scale));

    // GLint translateLocation = glGetUniformLocation(graphicsPipeline, "u_TranslateMatrix");
    // glUniformMatrix4fv(translateLocation, 1, GL_FALSE, &transformMatrix[0][0]);

    mat4 cameraMatrix = cam.getCameraMatrix();

    GLint cameraLocation = glGetUniformLocation(graphicsPipeline, "u_CameraMatrix");
    glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, &cameraMatrix[0][0]);

    mat4 perspectiveMatrix = perspective<float>(radians(45.0), (float)p_screenWidth / (float)p_screenHeight, 0.1, 10);
    GLint perspectiveLocation = glGetUniformLocation(graphicsPipeline, "u_PerspectiveMatrix");
    glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, &perspectiveMatrix[0][0]);
}
void OpenGLRenderer::setTransformUniform(Transform& p_transform){
    mat4 transformMatrix = translate(mat4(1), p_transform.position);
    transformMatrix = rotate<float>(transformMatrix, radians(p_transform.rotation), vec3(0, 1, 0));
    transformMatrix = glm::scale(transformMatrix, p_transform.scale);

    GLint translateLocation = glGetUniformLocation(graphicsPipeline, "u_TranslateMatrix");
    glUniformMatrix4fv(translateLocation, 1, GL_FALSE, &transformMatrix[0][0]);
}
void OpenGLRenderer::setAlbedoUniform(glm::vec4& color){
    GLint albedoLocation = glGetUniformLocation(graphicsPipeline, "u_Albedo");
    glUniform4f(albedoLocation, color.r, color.g, color.b, color.a);
}
void OpenGLRenderer::setTextureUniform(GLuint p_tex){
    glBindTexture(GL_TEXTURE_2D, p_tex);
}
void OpenGLRenderer::Draw(){
    
    glDrawElements(GL_TRIANGLES, drawCallSize, GL_UNSIGNED_INT, 0);
}
Camera* OpenGLRenderer::getCam(){
    return &cam;
}

int OpenGLRenderer::pushQueue(std::vector<GLfloat>& p_points, int queueSize){
    if(vertexDataQueue == nullptr){
        vertexDataQueue = new std::vector<GLfloat>();
    }

    drawCallSize += queueSize;

    int offset = vertexDataQueue -> size() / INDICES_PER_POINT;

    vertexDataQueue->insert(vertexDataQueue -> end(), p_points.begin(), p_points.end());

    return offset;
}
GLuint OpenGLRenderer::loadTexture(const char* file){
    SDL_Surface* surface = SDL_LoadBMP(file);
    
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(surface);

    return textureID;
}