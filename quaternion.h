#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>

struct Quaternion
{
 private:
  float angle = 0;
  glm::vec3 axis;

 public:
  static inline Quaternion angleAxis(float a, glm::vec3 axis)
  {
   Quaternion r;
   r.angle = cos(a/2);
   float temp = sin(a/2);
   r.axis = temp * axis;
   return r;
  }

  static inline glm::mat4 matrix(Quaternion q)
  {
   glm::mat4 r;

   /* glm::vec4 col0(-1          , 2 * -q.angle , 2 , 0); */
   /* glm::vec4 col1(2 * q.angle , -1           , 0 , 0); */
   /* glm::vec4 col2(0           , 0            , 1 , 0); */
   /* glm::vec4 col3(0           , 0            , 0 , 1); */

   /* r[0] = col0; */
   /* r[1] = col1; */
   /* r[2] = col2; */
   /* r[3] = col3; */


   /* tmat3x3<T, P> Result(T(1)); */
   /* T qxx(q.x * q.x); */
   /* T qyy(q.y * q.y); */
   /* T qzz(q.z * q.z); */
   /* T qxz(q.x * q.z); */
   /* T qxy(q.x * q.y); */
   /* T qyz(q.y * q.z); */
   /* T qwx(q.w * q.x); */
   /* T qwy(q.w * q.y); */
   /* T qwz(q.w * q.z); */

   /* Result[0][0] = T(1) - T(2) * (qyy +  qzz); */
   /* Result[0][1] = T(2) * (qxy + qwz); */
   /* Result[0][2] = T(2) * (qxz - qwy); */

   /* Result[1][0] = T(2) * (qxy - qwz); */
   /* Result[1][1] = T(1) - T(2) * (qxx +  qzz); */
   /* Result[1][2] = T(2) * (qyz + qwx); */

   /* Result[2][0] = T(2) * (qxz + qwy); */
   /* Result[2][1] = T(2) * (qyz - qwx); */
   /* Result[2][2] = T(1) - T(2) * (qxx +  qyy); */
   /* return Result; */
   r[0][0] = -1;
   r[0][1] = 2 * q.angle;
   r[0][2] = 0;
   r[0][3] = 0;

   r[1][0] = 2 * -q.angle;
   r[1][1] = -1;
   r[1][2] = 0;
   r[1][3] = 0;

   r[2][0] = 2;
   r[2][1] = 0;
   r[2][2] = 1;
   r[2][3] = 0;

   r[3][0] = 0;
   r[3][1] = 0;
   r[3][2] = 0;
   r[3][3] = 1;

   return r;
  }

  /* inline Quaternion operator*(Quaternion & right) */
  /* { */
  /*  Quaternion & left = *this; */

  /*  float angle; */

  /*  angle = left.angle * right.angle - 1; */
  /*  glm::vec3 axis(0, 0, angle + angle); */

  /*  Quaternion r; */
  /*  r.angle = angle; */
  /*  r.axis = axis; */

  /*  return r; */
  /* } */

  static inline Quaternion lerp(Quaternion source, Quaternion destination, float t)
  {
   float t2 = 1 - t;

   float angle    = t2 * source.angle + t * destination.angle;
   glm::vec3 axis = t2 * source.axis  + t * destination.axis;

   /* glm::vec3 axis = glm::vec3(0, 0, 1); */

   /* Quaternion r; */
   /* r.angle = angle; */
   /* r.axis = axis; */

   return Quaternion::angleAxis(angle, axis);
  }
};

