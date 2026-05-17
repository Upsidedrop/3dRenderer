#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Camera::Camera(){
    position = vec3(0,0,0);
    lookDir = vec3(0, 0, -1);
    upDir = vec3(0, 1, 0);
}
mat4 Camera::getCameraMatrix(){
    return lookAt(position, lookDir + position, upDir);
}
void Camera::setPosition(glm::vec3 p_position){
    position = p_position;
}
glm::vec3 Camera::getPosition(){
    return position;
}
void Camera::turnHorizontal(float angle){
    lookDir = rotate(mat4(1), angle, upDir) * vec4(0, 0, -1, 1);
}
void Camera::turnVertical(float angle){
    vec4 lookDir4 = vec4(lookDir.x, lookDir.y, lookDir.z, 0);
    lookDir = rotate(mat4(1), angle, (vec3)(rotate(mat4(1), radians((float)90), upDir) * lookDir4)) * lookDir4;
}
vec3 Camera::getLookDir(){
    return lookDir;
}
vec3 Camera::getUpDir(){
    return upDir;
}