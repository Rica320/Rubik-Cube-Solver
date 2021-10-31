#ifndef MAT_H
#define MAT_H

#include <vector>
#include <tuple>
#include <cmath>

struct vec3d
{
    float x = 0, y = 0, z = 0, w = 1;

    inline vec3d& operator-=(const vec3d& t2) {
        this->x -= t2.x;
        this->y -= t2.y;
        this->z -= t2.z;
        return *this;
    }
    inline vec3d& operator+=(const vec3d& t2) {
        this->x += t2.x;
        this->y += t2.y;
        this->z += t2.z;
        return *this;
    }
};

struct vec2d
{
    float x = 0, y = 0;
};

inline vec3d operator *(const float l, const vec3d& v) {
    return { v.x * l,v.y * l ,v.z * l };
}

inline vec3d operator *(const vec3d& l, const vec3d& v) {
    return { v.x * l.x,v.y * l.y ,v.z * l.z };
}

inline vec3d operator *(const vec3d& v, const float l) {
    return { v.x * l,v.y * l ,v.z * l };
}

inline vec3d operator -(const vec3d& t1, const vec3d& t2) {
    vec3d e;
    e = { t1.x - t2.x,t1.y - t2.y ,t1.z - t2.z };
    return e;
}
inline vec3d operator +(const vec3d& t1, const vec3d& t2) {
    vec3d e;
    e = { t1.x + t2.x,t1.y + t2.y ,t1.z + t2.z };
    return e;
}

inline bool operator ==(const vec3d& v, const vec3d& l) {

    return (std::abs(v.x - l.x) < 0.00001f) && (std::abs(v.y - l.y) < 0.00001f) && (std::abs(v.z - l.z) < 0.00001f);
    // see if there is a problem with doing this --- There is
}
inline bool operator !=(const vec3d& v, const vec3d& l) {
    return !(v == l); // see if there is a problem with doing this --- There is
}

struct vec4d // only for colors
{
    float R, G, B, A;
};

struct mat4x4 // make a class
{
    float m[4][4] = { 0 };
};

struct triangle // make class
{
    vec3d p[3];
    vec4d col = { 255.0f,255.0f,255.0f,0.9f };


    // TODO: KILL THE MAGIC NUMBERS
    vec2d t[3] = { {10.0f * 64.0f /1024.0f, 3* 64.0f  / 1024.0f},
                   {10.0f * 64.0f / 1024.0f, 4 * 64.0f / 1024.0f},
                   {11.0f * 64.0f / 1024.0f,  4 * 64.0f / 1024.0f} };

};

struct mesh // make class : public triangle
{
    std::vector<triangle> tris;
};


struct Plane {
    vec3d n; // normal
    float d; // distance from origin
   
    // Plane() {};
    Plane(triangle tri);
    Plane(vec3d point, vec3d normal);
    vec3d intersectLine(vec3d a, vec3d b);
    std::vector<triangle> ClipTri(triangle& tri); // change this to use generic obj and its planes, make this faster
    vec3d projectVectorToPlane(const vec3d& cameraFront);

};

inline bool operator<(const vec3d& lhs, const vec3d& rhs) {
    return std::tie(lhs.x, lhs.y, lhs.z) <
           std::tie(rhs.x, rhs.y, rhs.z);
}

inline bool operator<(const triangle& lhs, const triangle& rhs) {
    return std::tie(lhs.p[0], lhs.p[1], lhs.p[2]) <
           std::tie(rhs.p[0], rhs.p[1], rhs.p[2]);
}
float vecDis(const vec3d& l, const vec3d& r);
float Norm(const vec3d& v);
inline bool operator<(const Plane& l, const Plane& r) {
    if (l.d != r.d) {
        return l.d < r.d;
    }
    return l.n < r.n;

}
// this piece of code was useless ... try again later


vec3d getNormal(const triangle& tri);

void MultiplyMatrixVector(vec3d& i, vec3d& o, const mat4x4& m);

triangle operator*(triangle& this1, const mat4x4& m);

void normalize(vec3d& v);

float dot(const vec3d& v1, const vec3d& v2);

vec3d cross(const vec3d& v1, const vec3d& v2);

mat4x4 Identity();

mat4x4 MatrixInverse(mat4x4& m);

mat4x4 MultiplyMatrix(mat4x4& m1, mat4x4& m2);

mat4x4 Translate(float x, float y, float z);

mat4x4 lookAt(const vec3d& eye, const vec3d& center, const vec3d& up);

mat4x4 Matrix_MakeRotationY(float fAngleRad);

mat4x4 Matrix_MakeRotationX(float fAngleRad);

mat4x4 Matrix_MakeRotationZ(float fAngleRad);

mat4x4 Matrix_Projection(float Screenheight, float Screenwidth);

#endif // MAT_H