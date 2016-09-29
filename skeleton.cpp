#include "skeleton.h"
#include "splitstring.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
 * Load skeleton file
 */
void Skeleton::loadSkeleton(std::string skelFileName)
{
 std::string strBone;
 std::ifstream skelFile(skelFileName.c_str());
 if (skelFile.is_open())
 {
  while ( std::getline(skelFile, strBone)) { //Read a line to build a bone
   std::vector<std::string> boneParams;
   splitstring splitStr(strBone);
   boneParams = splitStr.split(' ');
   Joint temp;
   temp.position.x = std::atof(boneParams[1].c_str());
   temp.position.y = std::atof(boneParams[2].c_str());
   temp.position.z = std::atof(boneParams[3].c_str());

   temp.parent=std::atoi(boneParams[4].c_str());

   if (std::atoi(boneParams[0].c_str()) != joints.size())
   {
    std::cout<<"[Warning!!!] Bone index does not match\n";
   }
   joints.push_back(temp);
  }
 }
}

/*
 * Load Animation
 */
void Skeleton::loadAnimation(std::string skelFileName)
{
}


/*
 * Draw skeleton with OpenGL
 */
//this is for the joints
//function is called how often?
void Skeleton::glDrawSkeleton()
{
 //Rigging skeleton
 glDisable(GL_DEPTH_TEST);

 glPushMatrix();
 //why
 glTranslatef(-0.9,-0.9,-0.9);
 //why
 glScalef(1.8,1.8,1.8);
 //why
 glPointSize(1);
 //why
 glColor3f(1,0,0);
 updateScreenCoord();

 for (unsigned i=0; i< joints.size(); i++)
 {
  Joint &j=joints[i];
  Vec3 parentJ=joints[0].position;

  if(j.parent != -1)
  {
   parentJ=joints[j.parent].position;
   glColor3f(1, 0,0);
   glBegin(GL_LINES);
   vertex(j.position);
   vertex(parentJ);
   glEnd();
  }

  if (joints[i].isPicked)
   glColor3f(1.0, 0.0, 0.0);
  else if (joints[i].isHovered)
   glColor3f(0.0, 0.0, 1.0);
  else
   glColor3f(0.0, 1.0, 0.0);

  double angle=joints[i].angle;

  Vec3 diff=joints[i].position - parentJ;
  glm::mat4 tran = glm::translate(glm::mat4(1.f), glm::vec3(diff.x, diff.y, diff.z));
  glm::mat4 rot  = glm::rotate(glm::mat4(1.f), float(angle * 3.14159265 / 180.f), glm::vec3(0, 0, 1));
  glm::mat4 tran2= glm::translate(glm::mat4(1.f), glm::vec3(parentJ.x, parentJ.y, parentJ.z));
  glm::vec4 fp =  tran2 * rot * tran * glm::vec4(0.0, 0.0, 0.0, 1.0);

  /* glm::mat4 tran = glm::translate(glm::mat4(1.f), glm::vec3(diff.x, diff.y, diff.z)); */
  /* glm::mat4 rot  = glm::rotate(tran, float(angle * 3.14159265 / 180.f), glm::vec3(0, 0, 1)); */
  /* glm::mat4 tran2= glm::translate(rot, glm::vec3(parentJ.x, parentJ.y, parentJ.z)); */
  /* glm::vec4 fp =    glm::vec4(0.0, 0.0, 0.0, 1.0) * tran2; */

  glTranslated(fp.x, fp.y, fp.z);

  glutSolidSphere(0.015, 15, 15);

  glTranslated(-fp.x, -fp.y, -fp.z);

  /* //translate to original position of parent. */
  /* glTranslated( parentJ.x,  parentJ.y,  parentJ.z); */
  /* //rotate about parent which is at origin. */	
  /* glRotatef(angle,0, 0, 1); */
  /* //translate point so that parent is on origin. */
  /* glTranslated(joints[i].position.x - parentJ.x, joints[i].position.y - parentJ.y, joints[i].position.z - parentJ.z); */


  /* glTranslated(-joints[i].position.x + parentJ.x, -joints[i].position.y + parentJ.y, -joints[i].position.z + parentJ.z); */
  /* glRotatef(-angle,0, 0, 1); */
  /* glTranslated( -parentJ.x,  -parentJ.y,  -parentJ.z); */
 }
 glPopMatrix();

 glEnable(GL_DEPTH_TEST);
}

void Skeleton::updateScreenCoord()
{
 GLint viewport[4];
 GLdouble modelview[16];
 GLdouble projection[16];
 GLdouble winX, winY, winZ;

 glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
 glGetDoublev( GL_PROJECTION_MATRIX, projection );
 glGetIntegerv( GL_VIEWPORT, viewport );
 for (unsigned i=0; i<joints.size(); i++)
 {
  gluProject((GLdouble)joints[i].position.x, (GLdouble)joints[i].position.y, (GLdouble)joints[i].position.z,
    modelview, projection, viewport,
    &winX, &winY, &winZ );
  joints[i].screenCoord.x = winX;
  joints[i].screenCoord.y = (double)glutGet(GLUT_WINDOW_HEIGHT)-winY;
 }
}
void Skeleton::checkHoveringStatus(int x, int y)
{
 double distance = 0.0f;
 double minDistance = 1000.0f;
 int hoveredJoint = -1;
 for(unsigned i=0; i < joints.size(); i++)
 {
  joints[i].isHovered = false;
  distance = sqrt((x - joints[i].screenCoord.x)*(x - joints[i].screenCoord.x) 
    + (y - joints[i].screenCoord.y)*(y - joints[i].screenCoord.y));
  if (distance > 50) continue;
  if (distance < minDistance)
  {
   hoveredJoint = i;
   minDistance = distance;
  }
 }
 if (hoveredJoint != -1) joints[hoveredJoint].isHovered = true;
}

void Skeleton::release()
{
 hasJointSelected = false;
 for (unsigned i=0; i<joints.size(); i++)
 {
  joints[i].isPicked = false;
 }
}



std::ostream& operator<<(std::ostream& os, const Joint& j)
{
 Vec3 pos=j.position;
 os << pos.x << ' ' << pos.y << ' ' << pos.z;
 return os;
}


void Skeleton::selectOrReleaseJoint()
{
 bool hasHovered=false;
 for (unsigned i=0; i<joints.size(); i++)
 {
  joints[i].isPicked = false;
  if (joints[i].isHovered)
  {
   hasHovered = true;
   joints[i].isPicked = true;
   hasJointSelected = true;
   selectedJoint=i;
  }
 }
 if (!hasHovered)    //Release joint
  hasJointSelected = false;
}

