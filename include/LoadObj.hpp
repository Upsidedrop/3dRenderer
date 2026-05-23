#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <iostream>
#include <string>

#include "Mesh.hpp"

extern std::vector<Mesh*> models;

namespace LoadObj{
    struct GroupedPoint{
        glm::vec3* position;
        glm::vec2* uv;
    };
    // Pushes new mesh "outMesh" to heap, delete after usage
    void ParseFile(const char* file, Mesh*& outMesh, const char* texture);
    void gatherVertexPositions(const char* p_file, std::vector<glm::vec3>& p_vertexPositions);
    void gatherUVs(const char* p_file, std::vector<glm::vec2>& p_vertexUVs);
    void organizeVertices(const char* p_file, std::vector<GLuint>& indices, std::vector<GroupedPoint>& p_points, std::vector<glm::vec3>& p_vertexPositions, std::vector<glm::vec2>& p_vertexUVs);
    void orderData(std::vector<GLfloat>& p_data, std::vector<GroupedPoint> p_points);
    int countSpaces(std::string& text);
    void seperateValues(std::string& line, std::string* out, int size, int start = 1, char delimiter = ' ');
    void copyStr(std::string* dst, std::string& source, int begin, int end, int index);
    void createFace(std::vector<GLuint>& indices, int offset, int numPoints);

    template <typename T>
    void collectValues(std::string& line, T* out, std::string* textBuffer, int size, T (*conversion)(const std::string&), int start = 1, char delimiter = ' '){
        seperateValues(line, textBuffer, size, start, delimiter);
        for(int i = 0; i < size; ++i){
            out[i] = conversion(textBuffer[i]);
        }       
    }
    
    void loadModels(std::vector<std::pair<const char*, const char*>> filenames);
}
