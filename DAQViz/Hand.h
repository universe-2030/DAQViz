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
	
	void WristRotatePos(float pos1);
	void ThumbRotatePos(float pos1, float pos2);
	void IndexRotatePos(float pos1, float pos2, float pos3);
	void MiddleRotatePos(float pos1, float pos2, float pos3);
	void RingRotatePos(float pos1, float pos2, float pos3);
	void PinkyRotatePos(float pos1, float pos2, float pos3);

	void fingerRotatePos(float pos);
	void fingerRotateDown();
	void fingerRotateUp();

	//type1: vertical ; type2 : armRotate
	void plamRotate(int rotateType);
	void Render();

	const float* Get_root_init();
	const float* Get_first_init();
	const float* Get_second_init();

	const float* Get_root_min();
	const float* Get_first_min();
	const float* Get_second_min();

	const float* Get_root_max();
	const float* Get_first_max();
	const float* Get_second_max();

private:
	GLFrame* object;
	GLMmodel* palm;
	GLMmodel* arm;

	float root[4];
	float first[5];
	float second[5];

	float root_init[4];
	float first_init[5];
	float second_init[5];

	float root_min[4];
	float first_min[5];
	float second_min[5];

	float root_max[4];
	float first_max[5];
	float second_max[5];

	float armDeg;
	int index;
	int finger_index;
};