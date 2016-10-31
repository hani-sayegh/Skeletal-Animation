#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

struct Quaternion
{
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


  float z = q.axis.z;
  float zz = z * z;
  r[0][0] = 1 - 2 * zz;
  r[0][1] = 2 * q.angle * z;
  r[0][2] = 0;
  r[0][3] = 0;

  r[1][0] = -2 * q.angle * z;
  r[1][1] = 1 - 2 * zz;
  r[1][2] = 0;
  r[1][3] = 0;

  r[2][0] = 0;
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

 static inline glm::mat4 eulerLerp(Quaternion q, Quaternion q2, float t)
 {
  float angleZ = atan2(2 * (q.axis.x * q.axis.y + q.axis.z * q.angle), -q.axis.z * q.axis.z + q.angle * q.angle);
  float angleZ2 = atan2(2 * (q2.axis.x * q2.axis.y + q2.axis.z * q2.angle), -q2.axis.z * q2.axis.z + q2.angle * q2.angle);
  float t2 = 1 - t;

  float finalAngle = t2 * angleZ + t * angleZ2;

  return glm::rotate(glm::mat4(1.f), finalAngle, glm::vec3(0, 0, 1));
 }

 static inline glm::mat4 slerp(Quaternion source, Quaternion destination, float t)
 {
  float angle = acos(
    source.angle * destination.angle + 
    source.axis.x * destination.axis.x + 
    source.axis.y * destination.axis.y + 
    source.axis.z * destination.axis.z
    );

  float sinAngle = sin(angle);
  /* std::cout << angle << std::endl; */

  float t2 = 1 - t;

  //angle between quaternions is 0
  //why isnan only happens when loading file?
  if(sinAngle == 0 || std::isnan(sinAngle))
  {
   //avoid division by 0, and return current quaternion (rotation)
   return Quaternion::matrix(source);
  }

  float sourceC = sin(angle * t2) / sinAngle;
  float destC = sin(angle * t) / sinAngle;

  Quaternion finalQuaternion;
  finalQuaternion.angle = sourceC * source.angle + destC * destination.angle;

  finalQuaternion.axis.x = sourceC * source.axis.x + destC * destination.axis.x;
  finalQuaternion.axis.y = sourceC * source.axis.y + destC * destination.axis.y;
  finalQuaternion.axis.z = sourceC * source.axis.z + destC * destination.axis.z;

  return Quaternion::matrix(finalQuaternion);
 }

 static inline Quaternion lerp(Quaternion source, Quaternion destination, float t)
 {
  float t2 = 1 - t;

  float angle    = t2 * source.angle + t * destination.angle;
  glm::vec3 axis = t2 * source.axis  + t * destination.axis;

  /* glm::vec3 axis = glm::vec3(0, 0, 1); */

  //no need to normalize here since we are already using two normalized
  Quaternion r;
  r.angle = angle;
  r.axis = axis;

  return r;
 }
};
