#include "BoundingBox.hpp"

using namespace std;
using namespace glm;

vec3 getSmaller(const vec3& corner, const vec3& point){
    vec3 res;

    res.x = (point.x < corner.x)? point.x : corner.x;
    res.y = (point.y < corner.y)? point.y : corner.y;
    res.z = (point.z < corner.z)? point.z : corner.z;

    return res;
}
void BoundingBox::generateFromPoints(vector<vec3>& p_vertexPositions){
    smallCorner = vec3(0);
    largeCorner = vec3(0);

    for(vec3& point : p_vertexPositions){
        smallCorner = getSmaller(smallCorner, point);
        
        largeCorner = -getSmaller(-largeCorner, -point);
    }
}