#include "Mat.h"
#include <vector>
#include <iostream>


vec3d getNormal(const triangle& tri) {
    vec3d line1, line2;
    line1.x = tri.p[1].x - tri.p[0].x;
    line1.y = tri.p[1].y - tri.p[0].y;
    line1.z = tri.p[1].z - tri.p[0].z;
           
    line2.x = tri.p[2].x - tri.p[0].x;
    line2.y = tri.p[2].y - tri.p[0].y;
    line2.z = tri.p[2].z - tri.p[0].z;

    return cross(line1, line2);
}

Plane::Plane(triangle tri) {
    n = getNormal(tri);
    normalize(n);
    d = -dot(tri.p[0], n);
}
vec3d Plane::intersectLine(vec3d a, vec3d b) {
    vec3d ba = b - a; 
    float nDotA = dot(n, a);
    float nDotB = dot(n, b);


    return a + (((-d - nDotA) / (nDotB - nDotA)) * ba);
    /*float ad = dot(lineStart, n);
    float bd = dot(lineEnd, n);
    float t = (-d - ad) / (bd - ad);
    vec3d lineStartToEnd = lineEnd - lineStart;
    vec3d lineToIntersect = lineStartToEnd * t;
    return lineStart + lineToIntersect;*/
}



Plane::Plane(vec3d point, vec3d normal) {
    normalize(normal);
    n = normal;
    d = -dot(point, n);
}
std::vector<triangle> Plane::ClipTri(triangle& tri) // change this to use generic obj and its planes, make this faster
{
    // inspired in olc code

    std::vector<triangle> resTris;
    auto dist = [&](vec3d& p)
    {
        return (n.x * p.x + n.y * p.y + n.z * p.z + d);
    };

    vec3d* inside_points[3];  int nInsidePointCount = 0;
    vec3d* outside_points[3]; int nOutsidePointCount = 0;

    float d0 = dist(tri.p[0]);
    float d1 = dist(tri.p[1]);
    float d2 = dist(tri.p[2]);

    if (d0 >= 0) { inside_points[nInsidePointCount++] = &tri.p[0]; }
    else { outside_points[nOutsidePointCount++] = &tri.p[0]; }
    if (d1 >= 0) { inside_points[nInsidePointCount++] = &tri.p[1]; }
    else { outside_points[nOutsidePointCount++] = &tri.p[1]; }
    if (d2 >= 0) { inside_points[nInsidePointCount++] = &tri.p[2]; }
    else { outside_points[nOutsidePointCount++] = &tri.p[2]; }

    if (nInsidePointCount == 3) {
        resTris.push_back(tri);
    }
    if (nInsidePointCount == 1 ) {
        triangle newTri{ *inside_points[0],
            intersectLine(*inside_points[0], *outside_points[0]),
            intersectLine(*inside_points[0], *outside_points[1]) };
        newTri.col = tri.col;
        resTris.push_back(newTri);
    }

    if (nInsidePointCount == 2) {
        triangle newTri1{ *inside_points[0],
            * inside_points[1],
            intersectLine(*inside_points[0], *outside_points[0]) };
        newTri1.col = tri.col;

        triangle newTri2{ *inside_points[1],
            newTri1.p[2],
            intersectLine(*inside_points[1], *outside_points[0]) };
        newTri2.col = tri.col;

        resTris.push_back(newTri1);
        resTris.push_back(newTri2);

    }

    return resTris;
}

void MultiplyMatrixVector(vec3d& i, vec3d& o, const mat4x4& m)
{
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f)
    {
        o.x /= w; o.y /= w; o.z /= w;
    }
}

float vecDis(const vec3d& l, const vec3d& r) {
    return sqrt(pow(l.x - r.x, 2) + pow(l.y - r.y, 2) + pow(l.z - r.z, 2));
}


triangle operator*(triangle& this1, const mat4x4& m) { // optimize
    vec3d v1;
    triangle o{ v1,v1,v1 };

    MultiplyMatrixVector(this1.p[0], o.p[0], m);
    MultiplyMatrixVector(this1.p[1], o.p[1], m);
    MultiplyMatrixVector(this1.p[2], o.p[2], m);
    return o;
}

float Norm(const vec3d& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
void normalize(vec3d& v) {
    float l = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    v.x /= l;
    v.y /= l;
    v.z /= l;
}

float dot(const vec3d& v1, const vec3d& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


vec3d cross(const vec3d& v1, const vec3d& v2)
{
    vec3d v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    return v;
}

mat4x4 Identity()
{
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 MatrixInverse(mat4x4& m) // Only for Rotation/Translation Matrices --- taken from the internet
{
    mat4x4 matrix;
    matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
    matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
    matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
    matrix.m[3][3] = 1.0f;
    return matrix;
}


mat4x4 MultiplyMatrix(mat4x4& m1, mat4x4& m2)
{
    mat4x4 matrix;
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
    return matrix;
}

mat4x4 Translate(float x, float y, float z)
{
    mat4x4 matrix;

    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    return matrix;
}

mat4x4 lookAt(const vec3d& eye, const vec3d& center, const vec3d& up)
{
    vec3d  f = center - eye;
    normalize(f);
    vec3d  s = cross(f, up);
    normalize(s);
    vec3d  u = cross(s, f);

    mat4x4 Result = Identity();
    Result.m[0][0] = s.x;
    Result.m[1][0] = s.y;
    Result.m[2][0] = s.z;
    Result.m[0][1] = u.x;
    Result.m[1][1] = u.y;
    Result.m[2][1] = u.z;
    Result.m[0][2] = -f.x;
    Result.m[1][2] = -f.y;
    Result.m[2][2] = -f.z;
    Result.m[3][0] = -dot(s, eye);
    Result.m[3][1] = -dot(u, eye);
    Result.m[3][2] = dot(f, eye);
    return Result;
}

// javidx9 code:
mat4x4 Matrix_MakeRotationY(float fAngleRad)
{
    mat4x4 matrix;
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][2] = sinf(fAngleRad);
    matrix.m[2][0] = -sinf(fAngleRad);
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}


mat4x4 Matrix_MakeRotationX(float fAngleRad)
{
    mat4x4 matRotX;
    matRotX.m[0][0] = 1;
    matRotX.m[1][1] = cosf(fAngleRad);
    matRotX.m[1][2] = sinf(fAngleRad);
    matRotX.m[2][1] =-sinf(fAngleRad);
    matRotX.m[2][2] = cosf(fAngleRad);
    matRotX.m[3][3] = 1;
    return matRotX;
}



mat4x4 Matrix_MakeRotationZ(float fAngleRad) // confirm that these are the correct matrixs
{
    mat4x4 matRotZ;
    matRotZ.m[0][0] = cosf(fAngleRad);
    matRotZ.m[0][1] = sinf(fAngleRad);
    matRotZ.m[1][0] =-sinf(fAngleRad);
    matRotZ.m[1][1] = cosf(fAngleRad);
    matRotZ.m[2][2] = 1;
    matRotZ.m[3][3] = 1;
    return matRotZ;
}

mat4x4 Matrix_Projection(float Screenheight, float Screenwidth) {
    float Near = 0.1f;
    float Far = 1000.0f;
    float Fov = 90.0f;
    float AspectRatio = Screenheight / Screenwidth;
    float FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);

    mat4x4 projMat;
    projMat.m[0][0] = AspectRatio * FovRad;
    projMat.m[1][1] = FovRad;
    projMat.m[2][2] = Far / (Far - Near);
    projMat.m[3][2] = (-Far * Near) / (Far - Near);
    projMat.m[2][3] = 1.0f;
    projMat.m[3][3] = 0.0f;

    return projMat;
}