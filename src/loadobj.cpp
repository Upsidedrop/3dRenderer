#include "LoadObj.hpp"

#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>

using namespace std;
using namespace glm;

namespace LoadObj{
    void ParseFile(const char* file, Mesh*& outMesh, const char* texture){
        vector<vec3> vertexPositions;
        gatherVertexPositions(file, vertexPositions);

        vector<vec2> vertexUVs;
        gatherUVs(file, vertexUVs);

        vector<GroupedPoint> points;
        vector<GLuint> indices;
        organizeVertices(file, indices, points, vertexPositions, vertexUVs);

        vector<GLfloat> data;
        orderData(data, points);

        outMesh = new Mesh(data, indices, texture);
        
    }
    void seperateValues(string& line, string* out, int size, int start, char delimiter){
        int i = 0;
        int numSpaces = 0;

        for(; numSpaces < start; ++i){
            if(line[i] == delimiter){
                ++numSpaces;
            }
        }
        
        int begin = i;
        for(; numSpaces < size + start; ++i){
            if(i >= line.size()){
                copyStr(out, line, begin, i, numSpaces - start);
                return;
            }
            if(line[i] == delimiter){
                copyStr(out, line, begin, i, numSpaces - start);
                begin = i + 1;
                ++numSpaces;
            }
        }
    }
    void copyStr(string* dst, string& source, int begin, int end, int index){
        dst[index].clear();
        for(int i = 0; i < end - begin; ++i){
            dst[index].push_back(source[begin + i]);
        }
    }
    void gatherVertexPositions(const char* p_file, vector<vec3>& p_vertexPositions){
        fstream stream(p_file);
        string line;

        float components[3];
        string textBuffer[3];

        while(getline(stream, line)){
            if(line[0] != 'v' || line[1] != ' '){
                continue;
            }

            collectValues<float>(line, components, textBuffer, 3, [](const string& a){return stof(a);});

            p_vertexPositions.push_back({components[0], components[1], components[2]});
        }
    }
    void gatherUVs(const char* p_file, vector<vec2>& p_vertexUVs){
	    fstream stream(p_file);
        string line;

        float components[2];
        string textBuffer[2];

        while(getline(stream, line)){
            if(line[0] != 'v' || line[1] != 't'){
                continue;
            }

            collectValues<float>(line, components, textBuffer, 2, [](const string& a){return stof(a);});

            p_vertexUVs.push_back({components[0], components[1]});
        }
    }
    void organizeVertices(const char* p_file, vector<GLuint>& indices, vector<GroupedPoint>& p_points, vector<vec3>& p_vertexPositions, vector<vec2>& p_vertexUVs){
        fstream stream(p_file);
        string line;

        int highest = 0;
        string* textBuffer = nullptr;
        GroupedPoint* faceVertices;
        
        int components[2];
        string componentBuffer[2];

        while(getline(stream, line)){
            if(line[0] != 'f' || line[1] != ' '){
                continue;
            }
            int numValues = countSpaces(line);
            if(numValues > highest){
                if(textBuffer != nullptr){
                    delete[] textBuffer;
                    delete[] faceVertices;
                }
                textBuffer = new string[numValues];
                faceVertices = new GroupedPoint[numValues];
                highest = numValues;
            }
            seperateValues(line, textBuffer, numValues);
            for(int i = 0; i < numValues; ++i){
                collectValues<int>(textBuffer[i], components, componentBuffer, 2, [](const string& a){return stoi(a);}, 0, '/');
                faceVertices[i].position = &p_vertexPositions[components[0] - 1];
                faceVertices[i].uv = &p_vertexUVs[components[1] - 1];
            }
            createFace(indices, p_points.size(), numValues);
            
            p_points.insert(p_points.end(), faceVertices, faceVertices + numValues);
            
        }
        if(textBuffer != nullptr){
            delete[] textBuffer;
            delete[] faceVertices;
        }
    }
    void createFace(vector<GLuint>& indices, int offset, int numPoints){
        indices.reserve(indices.size() + (numPoints - 2) * 3);
        for(int i = 2; i < numPoints; ++i){
            indices.push_back(offset);
            indices.push_back(offset + i);
            indices.push_back(offset + i - 1);
        }
        
    }
    void orderData(vector<GLfloat>& p_data, vector<GroupedPoint> p_points){
        p_data.reserve(p_points.size() * 6);
        for(GroupedPoint& point : p_points){
            p_data.push_back(point.position -> x);
            p_data.push_back(point.position -> y);
            p_data.push_back(point.position -> z);
            p_data.push_back(point.uv -> x);
            p_data.push_back(1 - point.uv -> y);
            p_data.push_back(0);
        }
    }
    int countSpaces(string& text){
        int res = 0;
        for(char letter : text){
            if(letter == ' '){
                ++res;
            }
        }
        return res;
    }
    void loadModels(vector<pair<const char*, const char*>> filenames){
        models.reserve(filenames.size());
        for(int i = 0; i < filenames.size(); ++i){
            Mesh* foo;
            ParseFile(filenames[i].first, foo, filenames[i].second);
            models.push_back(foo);
        }
    }
}
