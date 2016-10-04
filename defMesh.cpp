#include "defMesh.h"
#include "splitstring.h"
#include <iostream>
using namespace std;

DefMesh::DefMesh()
{
 pmodel = NULL;
 if (!pmodel) 
 {
  /* load up the model */
  char meshFile[] = "./model/cheb.obj";
  pmodel = glmReadOBJ(meshFile);
  if (!pmodel) {
   return;
  }
  glmFacetNormals(pmodel);
  glmVertexNormals(pmodel, 0);
  glmFacetNormals(pmodel);

  std::ifstream wFile("model/weights.out");
  if (wFile.is_open())
  {
   std::string strW;
   while ( std::getline(wFile, strW)) 
   {
    std::vector<std::string> w;
    splitstring splitStr(strW);
    w = splitStr.split(' ');
    for(auto i = 0; i != 17; ++i)
    {
     weights.push_back(std::atof(w[i].c_str()));
    }
   }
  }
  //3 * 6670
 }
 mySkeleton.loadSkeleton("./model/skeleton.out");
}
void DefMesh::glDraw(int type)
{

 switch(type){
  case 0:
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
  case 1:
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
  case 2:
   mySkeleton.glDrawSkeleton(); return;

 }
 glColor3f(0.5, 0.5, 0.5);
 mode = GLM_NONE;
 mode = mode | GLM_SMOOTH;
 glPushMatrix();
 glScalef(2,2,2);
 glTranslatef(-0.5, -0.5, -0.5);
 glmDraw(pmodel, mode);
 glPopMatrix();
 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

 mySkeleton.glDrawSkeleton();
}
