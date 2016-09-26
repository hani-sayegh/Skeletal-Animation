#ifndef SKELETON_H
#define SKELETON_H
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include "GL/glut.h"
#else
#include <GL/freeglut.h>
#endif
#endif

#include "simpleMath.h"



struct Joint
{
 Vec3 position;
 Vec2 screenCoord;
 bool isHovered;
 bool isPicked;
 int parent;
 double angle=0;

 Joint()
 {
  isHovered = false;
  isPicked = false;
 }
};


inline void vertex(Vec3 &v)
{
 glVertex3f(v.x, v.y, v.z);
}

inline std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
 os << v.x << ' ' << v.y;
 return os;
}
inline std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
 os << v.x << ' ' << v.y << ' ' << v.z;
 return os;
}
class Skeleton
{
 public:
  std::vector<Joint> joints;
  /*Update screen coordinates of joints*/
  void updateScreenCoord();


  /*True if the skeleton has a joint selected*/
  bool hasJointSelected;   
  Joint selectedJoint;
  Skeleton(){hasJointSelected = false;}
  /*
   * Load Skeleton file
   */
  void loadSkeleton(std::string skelFileName);

  /*
   * Load animation file
   */
  void loadAnimation(std::string skelFileName);

  /*
   * Draw skeleton with OpenGL
   */
  void glDrawSkeleton();

  /*
   * Check if any joint is hovered by given mouse coordinate
   */
  void checkHoveringStatus(int x, int y);

  void release();

  void selectOrReleaseJoint();
};

#endif
