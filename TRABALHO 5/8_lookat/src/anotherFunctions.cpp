#include "anotherFunctions.h"

Vector3 calculateNormal(Vector3 v1, Vector3 v2, Vector3 v3){
    Vector3 normal;
    Vector3 u = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
    Vector3 v = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};

    normal.x = u.y * v.z - u.z * v.y;
    normal.y = u.z * v.x - u.x * v.z;
    normal.z = u.x * v.y - u.y * v.x;

    float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    if (length != 0) {
        normal.x /= length;
        normal.y /= length;
        normal.z /= length;
    }

    normal.x = -normal.x;
    normal.y = -normal.y;
    normal.z = -normal.z;

    return normal;
}

Vector3 rotateX(Vector3 v, float angle){
    float rad = angle * PI / 180;
    float cosA = cos(rad);
    float sinA = sin(rad);
    return Vector3(v.x, v.y * cosA - v.z * sinA, v.y * sinA + v.z * cosA);
}

Vector3 rotateY(Vector3 v, float angle){
    float rad = angle * PI / 180;
    float cosA = cos(rad);
    float sinA = sin(rad);
    return Vector3(v.x * cosA + v.z * sinA, v.y, -v.x * sinA + v.z * cosA);
}

Vector3 rotateZ(Vector3 v, float angle){
    float rad = angle * PI / 180;
    float cosA = cos(rad);
    float sinA = sin(rad);
    return Vector3(v.x * cosA - v.y * sinA, v.x * sinA + v.y * cosA, v.z);
}

void normalizePoint(float &x, float &y, float &z){
    float length = sqrt(x * x + y * y + z * z);
    if (length != 0) {
        x /= length;
        y /= length;
        z /= length;
    }
}

float calculateDistance(Vector3 point, Vector3 cameraPos){
    return sqrt((cameraPos.x - point.x) * (cameraPos.x - point.x) + (cameraPos.y - point.y) * (cameraPos.y - point.y) + (cameraPos.z - point.z) * (cameraPos.z - point.z));
}