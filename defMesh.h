/*
 * Mesh with skeleton attached
 * You could put attachment here, or create an attachment class to manage weights
 */
#ifndef MESH_H
#define MESH_H
#include "./glm.h"
#include "./skeleton.h"
#include <vector>
class DefMesh
{
 public:
  Skeleton mySkeleton;
  std::vector<double> weights;
  GLMmodel * pmodel;
  std::vector<float> cpy;
  GLuint mode;
  DefMesh(); 
  void glDraw(int type);
};
#endif
