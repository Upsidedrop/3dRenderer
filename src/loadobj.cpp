#include "LoadObj.hpp"

#include <list>
#include <vector>
#include <fstream>

using namespace std;
using namespace glm;

namespace LoadObj{
    void ParseFile(const char* file, Mesh* outMesh){
	vector<vec3> vertexPositions;
	gatherVertexPositions(file, vertexPositions);

	vector<vec2> vertexUVs;
	gatherUVs(file, vertexUVs);

	vector<GLuint> indices;
	pairVertices(indices, vertexPositions, vertexUVs);

    }
    void gatherVertexPositions(const char* p_file, vector<vec3>& p_vertexPositions){
	
    }
    void gatherUVs(const char* p_file, vector<vec2>& p_vertexUVs){
	
    }
    void pairVertices(vector<GLuint>& p_indices, vector<vec3>& p_vertexPositions, vector<vec2>& p_vertexUVs){
	
    }
}
