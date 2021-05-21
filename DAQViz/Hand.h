#pragma once

#include <gltools.h>	// OpenGL toolkit
#include <glframe.h>   // Frame Class
#include "glm.h"
#include "GL/glew.h"
#include "GL/freeglut_std.h"

class Hand {
public:
	Hand(GLFrame* obj);
	~Hand();


	void setJointIndex(int num);
	void fingerRotatePos(float pos);
	void fingerRotateDown();
	void fingerRotateUp();

	//type1: vertical ; type2 : armRotate
	void plamRotate(int rotateType);
	void Render();

private:
	GLFrame* object;
	GLMmodel* palm;
	GLMmodel* arm;
	float root[4];
	float first[5];
	float second[5];
	float armDeg;
	int index;
};